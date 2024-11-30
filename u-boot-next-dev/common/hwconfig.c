// Filename: hwconfig.c
// Date: 2024/11/11
// By GST

#ifndef HWCONFIG_TEST
#include <config.h>
#include <common.h>
#include <exports.h>
#include <hwconfig.h>
#include <linux/types.h>
#include <linux/string.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif /* HWCONFIG_TEST */

DECLARE_GLOBAL_DATA_PTR;

static const char *hwconfig_parse(const char *opts, size_t maxlen,
				  const char *opt, const char *stopchs, char eqch,
				  size_t *arglen)
{
	size_t optlen = strlen(opt);
	const char *str;
	const char *start = opts;
	const char *end;

	while ((str = strstr(opts, opt)) != NULL) {
		end = str + optlen;
		if (end - start > maxlen)
			return NULL;

		if ((str == opts || strpbrk(str - 1, stopchs) == str - 1) &&
			(strpbrk(end, stopchs) == end || *end == eqch || *end == '\0')) {
			const char *arg_end;

			if (!arglen)
				return str;

			if (*end != eqch)
				return NULL;

			arg_end = strpbrk(end + 1, stopchs);
			if (!arg_end)
				*arglen = min(maxlen, strlen(end + 1));
			else
				*arglen = arg_end - end - 1;

			return end + 1;
		}
		opts = end;
	}
	return NULL;
}

const char cpu_hwconfig[] __attribute__((weak)) = "";
const char board_hwconfig[] __attribute__((weak)) = "";

static const char *__hwconfig(const char *opt, size_t *arglen,
			      const char *env_hwconfig)
{
	const char *ret;

	/* if we are passed a buffer use it, otherwise try the environment */
	if (!env_hwconfig) {
		if (!(gd->flags & GD_FLG_ENV_READY)) {
			printf("WARNING: Calling __hwconfig without a buffer "
					"and before environment is ready\n");
			return NULL;
		}
		env_hwconfig = env_get("hwconfig");
	}

	if (env_hwconfig) {
		ret = hwconfig_parse(env_hwconfig, strlen(env_hwconfig),
				      opt, ";", ':', arglen);
		if (ret)
			return ret;
	}

	ret = hwconfig_parse(board_hwconfig, strlen(board_hwconfig),
			opt, ";", ':', arglen);
	if (ret)
		return ret;

	return hwconfig_parse(cpu_hwconfig, strlen(cpu_hwconfig),
			opt, ";", ':', arglen);
}

int hwconfig_f(const char *opt, char *buf)
{
	return !!__hwconfig(opt, NULL, buf);
}

const char *hwconfig_arg_f(const char *opt, size_t *arglen, char *buf)
{
	return __hwconfig(opt, arglen, buf);
}

int hwconfig_arg_cmp_f(const char *opt, const char *arg, char *buf)
{
	const char *argstr;
	size_t arglen;

	argstr = hwconfig_arg_f(opt, &arglen, buf);
	if (!argstr || arglen != strlen(arg))
		return 0;

	return !strncmp(argstr, arg, arglen);
}

int hwconfig_sub_f(const char *opt, const char *subopt, char *buf)
{
	size_t arglen;
	const char *arg;

	arg = __hwconfig(opt, &arglen, buf);
	if (!arg)
		return 0;
	return !!hwconfig_parse(arg, arglen, subopt, ",;", '=', NULL);
}

const char *hwconfig_subarg_f(const char *opt, const char *subopt,
			      size_t *subarglen, char *buf)
{
	size_t arglen;
	const char *arg;

	arg = __hwconfig(opt, &arglen, buf);
	if (!arg)
		return NULL;
	return hwconfig_parse(arg, arglen, subopt, ",;", '=', subarglen);
}

int hwconfig_subarg_cmp_f(const char *opt, const char *subopt,
			  const char *subarg, char *buf)
{
	const char *argstr;
	size_t arglen;

	argstr = hwconfig_subarg_f(opt, subopt, &arglen, buf);
	if (!argstr || arglen != strlen(subarg))
		return 0;

	return !strncmp(argstr, subarg, arglen);
}

#ifdef HWCONFIG_TEST
int main()
{
	const char *ret;
	size_t len;

	env_set("hwconfig", "key1:subkey1=value1,subkey2=value2;key2:value3;;;;"
			   "key3;:,:=;key4", 1);

	ret = hwconfig_arg("key1", &len);
	printf("%zd %.*s\n", len, (int)len, ret);
	assert(len == 29);
	assert(hwconfig_arg_cmp("key1", "subkey1=value1,subkey2=value2"));
	assert(!strncmp(ret, "subkey1=value1,subkey2=value2", len));

	ret = hwconfig_subarg("key1", "subkey1", &len);
	printf("%zd %.*s\n", len, (int)len, ret);
	assert(len == 6);
	assert(hwconfig_subarg_cmp("key1", "subkey1", "value1"));
	assert(!strncmp(ret, "value1", len));

	ret = hwconfig_subarg("key1", "subkey2", &len);
	printf("%zd %.*s\n", len, (int)len, ret);
	assert(len == 6);
	assert(hwconfig_subarg_cmp("key1", "subkey2", "value2"));
	assert(!strncmp(ret, "value2", len));

	ret = hwconfig_arg("key2", &len);
	printf("%zd %.*s\n", len, (int)len, ret);
	assert(len == 6);
	assert(hwconfig_arg_cmp("key2", "value3"));
	assert(!strncmp(ret, "value3", len));

	assert(hwconfig("key3"));
	assert(hwconfig_arg("key4", &len) == NULL);
	assert(hwconfig_arg("bogus", &len) == NULL);

	unenv_set("hwconfig");

	assert(hwconfig(NULL) == 0);
	assert(hwconfig("") == 0);
	assert(hwconfig("key3") == 0);

	return 0;
}
#endif /* HWCONFIG_TEST */
//GST