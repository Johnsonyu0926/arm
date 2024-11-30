//zephyr-3.7-branch/lib/os/cbprintf_complete.c

	*conv = (struct conversion) {
	   .invalid = false,
	};

	/* Skip over the opening %.  If the conversion specifier is %,
	 * that's the only thing that should be there, so
	 * fast-exit.
	 */
	++sp;
	if (*sp == '%') {
		conv->specifier = *sp;
		++sp;
		return sp;
	}

	sp = extract_flags(conv, sp);
	sp = extract_width(conv, sp);
	sp = extract_prec(conv, sp);
	sp = extract_length(conv, sp);
	sp = extract_specifier(conv, sp);

	return sp;
}

#ifdef CONFIG_64BIT

static void _ldiv5(uint64_t *v)
{
	/* The compiler can optimize this on its own on 64-bit architectures */
	*v /= 5U;
}

#else /* CONFIG_64BIT */

/*
 * Tiny integer divide-by-five routine.  The full 64 bit division
 * implementations in libgcc are very large on some architectures, and
 * currently nothing in Zephyr pulls it into the link.  So it makes
 * sense to define this much smaller special case here to avoid
 * including it just for printf.
 *
 * It works by multiplying v by the reciprocal of 5 i.e.:
 *
 *	result = v * ((1 << 64) / 5) / (1 << 64)
 *
 * This produces a 128-bit result, but we drop the bottom 64 bits which
 * accounts for the division by (1 << 64). The product is kept to 64 bits
 * by summing partial multiplications and shifting right by 32 which on
 * most 32-bit architectures means only a register drop.
 *
 * Here the multiplier is: (1 << 64) / 5 = 0x3333333333333333
 * i.e. a 62 bits value. To compensate for the reduced precision, we
 * add an initial bias of 1 to v. This conveniently allows for keeping
 * the multiplier in a single 32-bit register given its pattern.
 * Enlarging the multiplier to 64 bits would also work but carry handling
 * on the summing of partial mults would be necessary, and a final right
 * shift would be needed, requiring more instructions.
 */
static void _ldiv5(uint64_t *v)
{
	uint32_t v_lo = *v;
	uint32_t v_hi = *v >> 32;
	uint32_t m = 0x33333333;
	uint64_t result;

	/*
	 * Force the multiplier constant into a register and make it
	 * opaque to the compiler, otherwise gcc tries to be too smart
	 * for its own good with a large expansion of adds and shifts.
	 */
	__asm__ ("" : "+r" (m));

	/*
	 * Apply a bias of 1 to v. We can't add it to v as this would overflow
	 * it when at max range. Factor it out with the multiplier upfront.
	 */
	result = ((uint64_t)m << 32) | m;

	/* The actual multiplication. */
	result += (uint64_t)v_lo * m;
	result >>= 32;
	result += (uint64_t)v_lo * m;
	result += (uint64_t)v_hi * m;
	result >>= 32;
	result += (uint64_t)v_hi * m;

	*v = result;
}

#endif /* CONFIG_64BIT */

/* Division by 10 */
static void _ldiv10(uint64_t *v)
{
	*v >>= 1;
	_ldiv5(v);
}

/* Extract the next decimal character in the converted representation of a
 * fractional component.
 */
static char _get_digit(uint64_t *fr, int *digit_count)
{
	char rval;

	if (*digit_count > 0) {
		--*digit_count;
		*fr *= 10U;
		rval = ((*fr >> 60) & 0xF) + '0';
		*fr &= (BIT64(60) - 1U);
	} else {
		rval = '0';
	}

	return rval;
}

static inline size_t conversion_radix(char specifier)
{
	switch (specifier) {
	default:
	case 'd':
	case 'i':
	case 'u':
		return 10;
	case 'o':
		return 8;
	case 'p':
	case 'x':
	case 'X':
		return 16;
	}
}

/* Writes the given value into the buffer in the specified base.
 *
 * Precision is applied *ONLY* within the space allowed.
 *
 * Alternate form value is applied to o, x, and X conversions.
 *
 * The buffer is filled backwards, so the input bpe is the end of the
 * generated representation.  The returned pointer is to the first
 * character of the representation.
 */
static char *encode_uint(uint_value_type value,
			 struct conversion *conv,
			 char *bps,
			 const char *bpe)
{
	bool upcase = isupper((int)conv->specifier) != 0;
	const unsigned int radix = conversion_radix(conv->specifier);
	char *bp = bps + (bpe - bps);

	do {
		unsigned int lsv = (unsigned int)(value % radix);

		--bp;
		*bp = (lsv <= 9) ? ('0' + lsv)
			: upcase ? ('A' + lsv - 10) : ('a' + lsv - 10);
		value /= radix;
	} while ((value != 0) && (bps < bp));

	/* Record required alternate forms.  This can be determined
	 * from the radix without re-checking specifier.
	 */
	if (conv->flag_hash) {
		if (radix == 8) {
			conv->altform_0 = true;
		} else if (radix == 16) {
			conv->altform_0c = true;
		} else {
			;
		}
	}

	return bp;
}

/* Number of bits in the fractional part of an IEEE 754-2008 double
 * precision float.
 */
#define FRACTION_BITS 52

/* Number of hex "digits" in the fractional part of an IEEE 754-2008
 * double precision float.
 */
#define FRACTION_HEX DIV_ROUND_UP(FRACTION_BITS, 4)

/* Number of bits in the exponent of an IEEE 754-2008 double precision
 * float.
 */
#define EXPONENT_BITS 11

/* Mask for the sign (negative) bit of an IEEE 754-2008 double precision
 * float.
 */
#define SIGN_MASK BIT64(63)

/* Mask for the high-bit of a uint64_t representation of a fractional
 * value.
 */
#define BIT_63 BIT64(63)

/* Convert the IEEE 754-2008 double to text format.
 *
 * @param value the 64-bit floating point value.
 *
 * @param conv details about how the conversion is to proceed.  Some fields
 * are adjusted based on the value being converted.
 *
 * @param precision the precision for the conversion (generally digits past
 * the decimal point).
 *
 * @param bps pointer to the first character in a buffer that will hold the
 * converted value.
 *
 * @param bpe On entry this points to the end of the buffer reserved to hold
 * the converted value.  On exit it is updated to point just past the
 * converted value.
 *
 * return a pointer to the start of the converted value.  This may not be @p
 * bps but will be consistent with the exit value of *bpe.
 */
static char *encode_float(double value,
			  struct conversion *conv,
			  int precision,
			  char *sign,
			  char *bps,
			  const char **bpe)
{
	union {
		uint64_t u64;
		double dbl;
	} u = {
		.dbl = value,
	};
	bool prune_zero = false;
	char *buf = bps;

	/* Prepend the sign: '-' if negative, flags control
	 * non-negative behavior.
	 */
	if ((u.u64 & SIGN_MASK) != 0U) {
		*sign = '-';
	} else if (conv->flag_plus) {
		*sign = '+';
	} else if (conv->flag_space) {
		*sign = ' ';
	} else {
		;
	}

	/* Extract the non-negative offset exponent and fraction.  Record
	 * whether the value is subnormal.
	 */
	char c = conv->specifier;
	int expo = (u.u64 >> FRACTION_BITS) & BIT_MASK(EXPONENT_BITS);
	uint64_t fract = u.u64 & BIT64_MASK(FRACTION_BITS);
	bool is_subnormal = (expo == 0) && (fract != 0);

	/* Exponent of all-ones signals infinity or NaN, which are
	 * text constants regardless of specifier.
	 */
	if (expo == BIT_MASK(EXPONENT_BITS)) {
		if (fract == 0) {
			if (isupper((unsigned char)c) != 0) {
				buf[0] = 'I';
				buf[1] = 'N';
				buf[2] = 'F';
				buf += 3;
			} else {
				buf[0] = 'i';
				buf[1] = 'n';
				buf[2] = 'f';
				buf += 3;
			}
		} else {
			if (isupper((unsigned char)c) != 0) {
				buf[0] = 'N';
				buf[1] = 'A';
				buf[2] = 'N';
				buf += 3;
			} else {
				buf[0] = 'n';
				buf[1] = 'a';
				buf[2] = 'n';
				buf += 3;
			}
		}

		/* No zero-padding with text values */
		conv->flag_zero = false;

		*bpe = buf;
		return bps;
	}

	/* The case of an F specifier is no longer relevant. */
	if (c == 'F') {
		c = 'f';
	}

	/* Handle converting to the hex representation. */
	if (IS_ENABLED(CONFIG_CBPRINTF_FP_A_SUPPORT)
	    && (IS_ENABLED(CONFIG_CBPRINTF_FP_ALWAYS_A)
		|| conv->specifier_a)) {
		buf[0] = '0';
		buf[1] = 'x';
		buf += 2;

		/* Remove the offset from the exponent, and store the
		 * non-fractional value.  Subnormals require increasing the
		 * exponent as first bit isn't the implicit bit.
		 */
		expo -= 1023;
		if (is_subnormal) {
			*buf = '0';
			++buf;
			++expo;
		} else {
			*buf = '1';
			++buf;
		}

		/* If we didn't get precision from a %a specification then we
		 * treat it as from a %a specification with no precision: full
		 * range, zero-pruning enabled.
		 *
		 * Otherwise we have to cap the precision of the generated
		 * fraction, or possibly round it.
		 */
		if (!(conv->specifier_a && conv->prec_present)) {
			precision = FRACTION_HEX;
			prune_zero = true;
		} else if (precision > FRACTION_HEX) {
			conv->pad0_pre_exp = precision - FRACTION_HEX;
			conv->pad_fp = true;
			precision = FRACTION_HEX;
		} else if ((fract != 0)
			   && (precision < FRACTION_HEX)) {
			size_t pos = 4 * (FRACTION_HEX - precision) - 1;
			uint64_t mask = BIT64(pos);

			/* Round only if the bit that would round is
			 * set.
			 */
			if ((fract & mask) != 0ULL) {
				fract += mask;
			}
		}

		/* Record whether we must retain the decimal point even if we
		 * can prune zeros.
		 */
		bool require_dp = ((fract != 0) || conv->flag_hash);

		if (require_dp || (precision != 0)) {
			*buf = '.';
			++buf;
		}

		/* Get the fractional value as a hexadecimal string, using x
		 * for a and X for A.
		 */
		struct conversion aconv = {
			.specifier = isupper((unsigned char)c) != 0 ? 'X' : 'x',
		};
		const char *spe = *bpe;
		char *sp = bps + (spe - bps);

		if (fract != 0) {
			sp = encode_uint(fract, &aconv, buf, spe);
		}

		/* Pad out to full range since this is below the decimal
		 * point.
		 */
		while ((spe - sp) < FRACTION_HEX) {
			--sp;
			*sp = '0';
		}

		/* Append the leading significant "digits". */
		while ((sp < spe) && (precision > 0)) {
			*buf = *sp;
			++buf;
			++sp;
			--precision;
		}

		if (prune_zero) {
			while (*--buf == '0') {
				;
			}
			if ((*buf != '.') || require_dp) {
				++buf;
			}
		}

		*buf = 'p';
		++buf;
		if (expo >= 0) {
			*buf = '+';
			++buf;
		} else {
			*buf = '-';
			++buf;
			expo = -expo;
		}

		aconv.specifier = 'i';
		sp = encode_uint(expo, &aconv, buf, spe);

		while (sp < spe) {
			*buf = *sp;
			++buf;
			++sp;
		}

		*bpe = buf;
		return bps;
	}

	/* Remainder of code operates on a 64-bit fraction, so shift up (and
	 * discard garbage from the exponent where the implicit 1 would be
	 * stored).
	 */
	fract <<= EXPONENT_BITS;
	fract &= ~SIGN_MASK;

	/* Non-zero values need normalization. */
	if ((expo | fract) != 0) {
		if (is_subnormal) {
			/* Fraction is subnormal.  Normalize it and correct
			 * the exponent.
			 */
			for (fract <<= 1; (fract & BIT_63) == 0; fract <<= 1) {
				expo--;
			}
		}
		/* Adjust the offset exponent to be signed rather than offset,
		 * and set the implicit 1 bit in the (shifted) 53-bit
		 * fraction.
		 */
		expo -= (1023 - 1);	/* +1 since .1 vs 1. */
		fract |= BIT_63;
	}

	/*
	 * Let's consider:
	 *
	 *	value = fract * 2^expo * 10^decexp
	 *
	 * Initially decexp = 0. The goal is to bring exp between
	 * 0 and -2 as the magnitude of a fractional decimal digit is 3 bits.
	 */
	int decexp = 0;

	while (expo < -2) {
		/*
		 * Make room to allow a multiplication by 5 without overflow.
		 * We test only the top part for faster code.
		 */
		do {
			fract >>= 1;
			expo++;
		} while ((uint32_t)(fract >> 32) >= (UINT32_MAX / 5U));

		/* Perform fract * 5 * 2 / 10 */
		fract *= 5U;
		expo++;
		decexp--;
	}

	while (expo > 0) {
		/*
		 * Perform fract / 5 / 2 * 10.
		 * The +2 is there to do round the result of the division
		 * by 5 not to lose too much precision in extreme cases.
		 */
		fract += 2;
		_ldiv5(&fract);
		expo--;
		decexp++;

		/* Bring back our fractional number to full scale */
		do {
			fract <<= 1;
			expo--;
		} while (!(fract & BIT_63));
	}

	/*
	 * The binary fractional point is located somewhere above bit 63.
	 * Move it between bits 59 and 60 to give 4 bits of room to the
	 * integer part.
	 */
	fract >>= (4 - expo);

	if ((c == 'g') || (c == 'G')) {
		/* Use the specified precision and exponent to select the
		 * representation and correct the precision and zero-pruning
		 * in accordance with the ISO C rule.
		 */
		if ((decexp < (-4 + 1)) || (decexp > precision)) {
			c += 'e' - 'g';  /* e or E */
			if (precision > 0) {
				precision--;
			}
		} else {
			c = 'f';
			precision -= decexp;
		}
		if (!conv->flag_hash && (precision > 0)) {
			prune_zero = true;
		}
	}

	int decimals;
	if (c == 'f') {
		decimals = precision + decexp;
		if (decimals < 0) {
			decimals = 0;
		}
	} else {
		decimals = precision + 1;
	}

	int digit_count = 16;

	if (decimals > 16) {
		decimals = 16;
	}

	/* Round the value to the last digit being printed. */
	uint64_t round = BIT64(59); /* 0.5 */
	while (decimals-- != 0) {
		_ldiv10(&round);
	}
	fract += round;
	/* Make sure rounding didn't make fract >= 1.0 */
	if (fract >= BIT64(60)) {
		_ldiv10(&fract);
		decexp++;
	}

	if (c == 'f') {
		if (decexp > 0) {
			/* Emit the digits above the decimal point. */
			while ((decexp > 0) && (digit_count > 0)) {
				*buf = _get_digit(&fract, &digit_count);
				++buf;
				decexp--;
			}

			conv->pad0_value = decexp;

			decexp = 0;
		} else {
			*buf = '0';
			++buf;
		}

		/* Emit the decimal point only if required by the alternative
		 *
		 * format, or if more digits are to follow.
		 */
		if (conv->flag_hash || (precision > 0)) {
			*buf = '.';
			++buf;
		}
		if ((decexp < 0) && (precision > 0)) {
			conv->pad0_value = -decexp;
			if (conv->pad0_value > precision) {
				conv->pad0_value = precision;
			}
			precision -= conv->pad0_value;
			conv->pad_postdp = (conv->pad0_value > 0);
		}
	} else { /* e or E */
		/* Emit the one digit before the decimal.  If it's not zero,
		 * this is significant so reduce the base-10 exponent.
		 */
		*buf = _get_digit(&fract, &digit_count);
		if (*buf++ != '0') {
			decexp--;
		}
		/* Emit the decimal point only if required by the alternative
		 * format, or if more digits are to follow.
		 */
		if (conv->flag_hash || (precision > 0)) {
			*buf = '.';
			++buf;
		}
	}
	while ((precision > 0) && (digit_count > 0)) {
		*buf = _get_digit(&fract, &digit_count);
		++buf;
		precision--;
	}
	conv->pad0_pre_exp = precision;
	if (prune_zero) {
		conv->pad0_pre_exp = 0;
		do {
			--buf;
		} while (*buf == '0');
		if (*buf != '.') {
			++buf;
		}
	}
	/* Emit the explicit exponent, if format requires it. */
	if ((c == 'e') || (c == 'E')) {
		*buf = c;
		++buf;
		if (decexp < 0) {
			decexp = -decexp;
			*buf = '-';
			++buf;
		} else {
			*buf = '+';
			++buf;
		}
		/* At most 3 digits to the decimal.  Spit them out. */
		if (decexp >= 100) {
			*buf = (decexp / 100) + '0';
			++buf;
			decexp %= 100;
		}
		buf[0] = (decexp / 10) + '0';
		buf[1] = (decexp % 10) + '0';
		buf += 2;
	}
	/* Cache whether there's padding required */
	conv->pad_fp = (conv->pad0_value > 0)
		|| (conv->pad0_pre_exp > 0);
	/* Set the end of the encoded sequence, and return its start.  Also
	 * store EOS as a non-digit/non-decimal value so we don't have to
	 * check against bpe when iterating in multiple places.
	 */
	*bpe = buf;
	*buf = 0;
	return bps;
}
/* Store a count into the pointer provided in a %n specifier.
 *
 * @param conv the specifier that indicates the size of the value into which
 * the count will be stored.
 *
 * @param dp where the count should be stored.
 *
 * @param count the count to be stored.
 */
static inline void store_count(const struct conversion *conv,
			       void *dp,
			       int count)
{
	switch ((enum length_mod_enum)conv->length_mod) {
	case LENGTH_NONE:
		*(int *)dp = count;
		break;
	case LENGTH_HH:
		*(signed char *)dp = (signed char)count;
		break;
	case LENGTH_H:
		*(short *)dp = (short)count;
		break;
	case LENGTH_L:
		*(long *)dp = (long)count;
		break;
	case LENGTH_LL:
		*(long long *)dp = (long long)count;
		break;
	case LENGTH_J:
		*(intmax_t *)dp = (intmax_t)count;
		break;
	case LENGTH_Z:
		*(size_t *)dp = (size_t)count;
		break;
	case LENGTH_T:
		*(ptrdiff_t *)dp = (ptrdiff_t)count;
		break;
	default:
		/* Add an empty default with break, this is a defensive programming.
		 * Static analysis tool won't raise a violation if default is empty,
		 * but has that comment.
		 */
		break;
	}
}
/* Outline function to emit all characters in [sp, ep). */
static int outs(cbprintf_cb __out,
		void *ctx,
		const char *sp,
		const char *ep)
{
	size_t count = 0;
	cbprintf_cb_local out = __out;
	while ((sp < ep) || ((ep == NULL) && *sp)) {
		int rc = out((int)*sp, ctx);
		++sp;
		if (rc < 0) {
			return rc;
		}
		++count;
	}
	return (int)count;
}
int z_cbvprintf_impl(cbprintf_cb __out, void *ctx, const char *fp,
		     va_list ap, uint32_t flags)
{
	char buf[CONVERTED_BUFLEN];
	size_t count = 0;
	sint_value_type sint;
	cbprintf_cb_local out = __out;
	const bool tagged_ap = (flags & Z_CBVPRINTF_PROCESS_FLAG_TAGGED_ARGS)
			       == Z_CBVPRINTF_PROCESS_FLAG_TAGGED_ARGS;
/* Output character, returning EOF if output failed, otherwise
 * updating count.
 *
 * NB: c is evaluated exactly once: side-effects are OK
 */
#define OUTC(c) do { \
	int rc = (*out)((int)(c), ctx); \
	\
	if (rc < 0) { \
		return rc; \
	} \
	++count; \
} while (false)
/* Output sequence of characters, returning a negative error if output
 * failed.
 */
#define OUTS(_sp, _ep) do { \
	int rc = outs(out, ctx, (_sp), (_ep)); \
	\
	if (rc < 0) {	    \
		return rc; \
	} \
	count += rc; \
} while (false)
	while (*fp != 0) {
		if (*fp != '%') {
			OUTC(*fp);
			++fp;
			continue;
		}
		/* Force union into RAM with conversion state to
		 * mitigate LLVM code generation bug.
		 */
		struct {
			union argument_value value;
			struct conversion conv;
		} state = {
			.value = {
				.uint = 0,
			},
		};
		struct conversion *const conv = &state.conv;
		union argument_value *const value = &state.value;
		const char *sp = fp;
		int width = -1;
		int precision = -1;
		const char *bps = NULL;
		const char *bpe = buf + sizeof(buf);
		char sign = 0;
		fp = extract_conversion(conv, sp);
		if (conv->specifier_cat != SPECIFIER_INVALID) {
			if (IS_ENABLED(CONFIG_CBPRINTF_PACKAGE_SUPPORT_TAGGED_ARGUMENTS)
			    && tagged_ap) {
				/* Skip over the argument tag as it is not being
				 * used here.
				 */
				(void)va_arg(ap, int);
			}
		}
		/* If dynamic width is specified, process it,
		 * otherwise set width if present.
		 */
		if (conv->width_star) {
			width = va_arg(ap, int);
			if (width < 0) {
				conv->flag_dash = true;
				width = -width;
			}
		} else if (conv->width_present) {
			width = conv->width_value;
		} else {
			;
		}
		/* If dynamic precision is specified, process it, otherwise
		 * set precision if present.  For floating point where
		 * precision is not present use 6.
		 */
		if (conv->prec_star) {
			int arg = va_arg(ap, int);
			if (arg < 0) {
				conv->prec_present = false;
			} else {
				precision = arg;
			}
		} else if (conv->prec_present) {
			precision = conv->prec_value;
		} else {
			;
		}
		/* Reuse width and precision memory in conv for value
		 * padding counts.
		 */
		conv->pad0_value = 0;
		conv->pad0_pre_exp = 0;
		/* FP conversion requires knowing the precision. */
		if (IS_ENABLED(CONFIG_CBPRINTF_FP_SUPPORT)
		    && (conv->specifier_cat == SPECIFIER_FP)
		    && !conv->prec_present) {
			if (conv->specifier_a) {
				precision = FRACTION_HEX;
			} else {
				precision = 6;
			}
		}
		/* Get the value to be converted from the args.
		 *
		 * This can't be extracted to a helper function because
		 * passing a pointer to va_list doesn't work on x86_64.  See
		 * https://stackoverflow.com/a/8048892.
		 */
		enum specifier_cat_enum specifier_cat
			= (enum specifier_cat_enum)conv->specifier_cat;
		enum length_mod_enum length_mod
			= (enum length_mod_enum)conv->length_mod;
		/* Extract the value based on the argument category and length.
		 *
		 * Note that the length modifier doesn't affect the value of a
		 * pointer argument.
		 */
		if (specifier_cat == SPECIFIER_SINT) {
			switch (length_mod) {
			default:
			case LENGTH_NONE:
			case LENGTH_HH:
			case LENGTH_H:
				value->sint = va_arg(ap, int);
				break;
			case LENGTH_L:
				if (WCHAR_IS_SIGNED
				    && (conv->specifier == 'c')) {
					value->sint = (wchar_t)va_arg(ap,
							      WINT_TYPE);
				} else {
					value->sint = va_arg(ap, long);
				}
				break;
			case LENGTH_LL:
				value->sint =
					(sint_value_type)va_arg(ap, long long);
				break;
			case LENGTH_J:
				value->sint =
					(sint_value_type)va_arg(ap, intmax_t);
				break;
			case LENGTH_Z:		/* size_t */
			case LENGTH_T:		/* ptrdiff_t */
				/* Though ssize_t is the signed equivalent of
				 * size_t for POSIX, there is no uptrdiff_t.
				 * Assume that size_t and ptrdiff_t are the
				 * unsigned and signed equivalents of each
				 * other.  This can be checked in a platform
				 * test.
				 */
				value->sint =
					(sint_value_type)va_arg(ap, ptrdiff_t);
				break;
			}
			if (length_mod == LENGTH_HH) {
				value->sint = (signed char)value->sint;
			} else if (length_mod == LENGTH_H) {
				value->sint = (short)value->sint;
			}
		} else if (specifier_cat == SPECIFIER_UINT) {
			switch (length_mod) {
			default:
			case LENGTH_NONE:
			case LENGTH_HH:
			case LENGTH_H:
				value->uint = va_arg(ap, unsigned int);
				break;
			case LENGTH_L:
				if ((!WCHAR_IS_SIGNED)
				    && (conv->specifier == 'c')) {
					value->uint = (wchar_t)va_arg(ap,
							      WINT_TYPE);
				} else {
					value->uint = va_arg(ap, unsigned long);
				}
				break;
			case LENGTH_LL:
				value->uint =
					(uint_value_type)va_arg(ap,
						unsigned long long);
				break;
			case LENGTH_J:
				value->uint =
					(uint_value_type)va_arg(ap,
								uintmax_t);
				break;
			case LENGTH_Z:		/* size_t */
			case LENGTH_T:		/* ptrdiff_t */
				value->uint =
					(uint_value_type)va_arg(ap, size_t);
				break;
			}
			if (length_mod == LENGTH_HH) {
				value->uint = (unsigned char)value->uint;
			} else if (length_mod == LENGTH_H) {
				value->uint = (unsigned short)value->uint;
			}
		} else if (specifier_cat == SPECIFIER_FP) {
			if (length_mod == LENGTH_UPPER_L) {
				value->ldbl = va_arg(ap, long double);
			} else {
				value->dbl = va_arg(ap, double);
			}
		} else if (specifier_cat == SPECIFIER_PTR) {
			value->ptr = va_arg(ap, void *);
		}
		/* We've now consumed all arguments related to this
		 * specification.  If the conversion is invalid, or is
		 * something we don't support, then output the original
		 * specification and move on.
		 */
		if (conv->invalid || conv->unsupported) {
			OUTS(sp, fp);
			continue;
		}
		/* Do formatting, either into the buffer or
		 * referencing external data.
		 */
		switch (conv->specifier) {
		case '%':
			OUTC('%');
			break;
		case 's': {
			bps = (const char *)value->ptr;
			size_t len;
			if (precision >= 0) {
				len = strnlen(bps, precision);
			} else {
				len = strlen(bps);
			}
			bpe = bps + len;
			precision = -1;
			break;
		}
		case 'p':
			/* Implementation-defined: null is "(nil)", non-null
			 * has 0x prefix followed by significant address hex
			 * digits, no leading zeros.
			 */
			if (value->ptr != NULL) {
				bps = encode_uint((uintptr_t)value->ptr, conv,
						  buf, bpe);
				/* Use 0x prefix */
				conv->altform_0c = true;
				conv->specifier = 'x';
				goto prec_int_pad0;
			}
			bps = "(nil)";
			bpe = bps + 5;
			break;
		case 'c':
			bps = buf;
			buf[0] = CHAR_IS_SIGNED ? value->sint : value->uint;
			bpe = buf + 1;
			break;
		case 'd':
		case 'i':
			if (conv->flag_plus) {
				sign = '+';
			} else if (conv->flag_space) {
				sign = ' ';
			}
			/* sint/uint overlay in the union, and so
			 * can't appear in read and write operations
			 * in the same statement.
			 */
			sint = value->sint;
			if (sint < 0) {
				sign = '-';
				value->uint = (uint_value_type)-sint;
			} else {
				value->uint = (uint_value_type)sint;
			}
			__fallthrough;
		case 'o':
		case 'u':
		case 'x':
		case 'X':
			bps = encode_uint(value->uint, conv, buf, bpe);
		prec_int_pad0:
			/* Update pad0 values based on precision and converted
			 * length.  Note that a non-empty sign is not in the
			 * converted sequence, but it does not affect the
			 * padding size.
			 */
			if (precision >= 0) {
				size_t len = bpe - bps;
				/* Zero-padding flag is ignored for integer
				 * conversions with precision.
				 */
				conv->flag_zero = false;
				/* Set pad0_value to satisfy precision */
				if (len < (size_t)precision) {
					conv->pad0_value = precision - (int)len;
				}
			}
			break;
		case 'n':
			if (IS_ENABLED(CONFIG_CBPRINTF_N_SPECIFIER)) {
				store_count(conv, value->ptr, count);
			}
			break;
		case FP_CONV_CASES:
			if (IS_ENABLED(CONFIG_CBPRINTF_FP_SUPPORT)) {
				bps = encode_float(value->dbl, conv, precision,
						   &sign, buf, &bpe);
			}
			break;
		default:
			/* Add an empty default with break, this is a defensive
			 * programming. Static analysis tool won't raise a violation
			 * if default is empty, but has that comment.
			 */
			break;
		}
		/* If we don't have a converted value to emit, move
		 * on.
		 */
		if (bps == NULL) {
			continue;
		}
		/* The converted value is now stored in [bps, bpe), excluding
		 * any required zero padding.
		 *
		 * The unjustified output will be:
		 * * any sign character (sint-only)
		 * * any altform prefix
		 * * for FP:
		 *   * any pre-decimal content from the converted value
		 *   * any pad0_value padding (!postdp)
		 *   * any decimal point in the converted value
		 *   * any pad0_value padding (postdp)
		 *   * any pre-exponent content from the converted value
		 *   * any pad0_pre_exp padding
		 *   * any exponent content from the converted value
		 * * for non-FP:
		 *   * any pad0_prefix
		 *   * the converted value
		 */
		size_t nj_len = (bpe - bps);
		int pad_len = 0;
		if (sign != 0) {
			nj_len += 1U;
		}
		if (conv->altform_0c) {
			nj_len += 2U;
		} else if (conv->altform_0) {
			nj_len += 1U;
		}
		nj_len += conv->pad0_value;
		if (conv->pad_fp) {
			nj_len += conv->pad0_pre_exp;
		}
		/* If we have a width update width to hold the padding we need
		 * for justification.  The result may be negative, which will
		 * result in no padding.
		 *
		 * If a non-negative padding width is present and we're doing
		 * right-justification, emit the padding now.
		 */
		if (width > 0) {
			width -= (int)nj_len;
			if (!conv->flag_dash) {
				char pad = ' ';

				/* If we're zero-padding we have to emit the
				 * sign first.
				 */
				if (conv->flag_zero) {
					if (sign != 0) {
						OUTC(sign);
						sign = 0;
					}
					pad = '0';
				}

				while (width-- > 0) {
					OUTC(pad);
				}
			}
		}

		/* If we have a sign that hasn't been emitted, now's the
		 * time....
		 */
		if (sign != 0) {
			OUTC(sign);
		}

		if (IS_ENABLED(CONFIG_CBPRINTF_FP_SUPPORT) && conv->pad_fp) {
			const char *cp = bps;

			if (conv->specifier_a) {
				/* Only padding is pre_exp */
				while (*cp != 'p') {
					OUTC(*cp);
					++cp;
				}
			} else {
				while (isdigit((unsigned char)*cp) != 0) {
					OUTC(*cp);
					++cp;
				}

				pad_len = conv->pad0_value;
				if (!conv->pad_postdp) {
					while (pad_len-- > 0) {
						OUTC('0');
					}
				}

				if (*cp == '.') {
					OUTC(*cp);
					++cp;
					/* Remaining padding is
					 * post-dp.
					 */
					while (pad_len-- > 0) {
						OUTC('0');
					}
				}
				while (isdigit((unsigned char)*cp) != 0) {
					OUTC(*cp);
					++cp;
				}
			}

			pad_len = conv->pad0_pre_exp;
			while (pad_len-- > 0) {
				OUTC('0');
			}

			OUTS(cp, bpe);
		} else {
			if ((conv->altform_0c | conv->altform_0) != 0) {
				OUTC('0');
			}

			if (conv->altform_0c) {
				OUTC(conv->specifier);
			}

			pad_len = conv->pad0_value;
			while (pad_len-- > 0) {
				OUTC('0');
			}

			OUTS(bps, bpe);
		}

		/* Finish left justification */
		while (width > 0) {
			OUTC(' ');
			--width;
		}
	}

	return count;
#undef OUTS
#undef OUTC
}
//GST