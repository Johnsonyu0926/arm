//zephyr-3.7-branch/lib/libc/minimal/include/stdlib.h
#ifndef ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDLIB_H_
#define ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDLIB_H_

#include <stddef.h>
#include <limits.h>
#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert a string to an unsigned long integer
 *
 * @param nptr Pointer to the string to convert
 * @param endptr Pointer to the end of the parsed string
 * @param base Base of the number in the string
 * @return The converted value
 */
unsigned long strtoul(const char *nptr, char **endptr, int base);

/**
 * @brief Convert a string to a long integer
 *
 * @param nptr Pointer to the string to convert
 * @param endptr Pointer to the end of the parsed string
 * @param base Base of the number in the string
 * @return The converted value
 */
long strtol(const char *nptr, char **endptr, int base);

/**
 * @brief Convert a string to an unsigned long long integer
 *
 * @param nptr Pointer to the string to convert
 * @param endptr Pointer to the end of the parsed string
 * @param base Base of the number in the string
 * @return The converted value
 */
unsigned long long strtoull(const char *nptr, char **endptr, int base);

/**
 * @brief Convert a string to a long long integer
 *
 * @param nptr Pointer to the string to convert
 * @param endptr Pointer to the end of the parsed string
 * @param base Base of the number in the string
 * @return The converted value
 */
long long strtoll(const char *nptr, char **endptr, int base);

/**
 * @brief Convert a string to an integer
 *
 * @param s Pointer to the string to convert
 * @return The converted value
 */
int atoi(const char *s);

/**
 * @brief Allocate memory
 *
 * @param size Number of bytes to allocate
 * @return Pointer to the allocated memory, or NULL if allocation failed
 */
void *malloc(size_t size);

/**
 * @brief Allocate aligned memory
 *
 * @param alignment Alignment requirement
 * @param size Number of bytes to allocate
 * @return Pointer to the allocated memory, or NULL if allocation failed
 */
void *aligned_alloc(size_t alignment, size_t size); /* From C11 */

/**
 * @brief Free allocated memory
 *
 * @param ptr Pointer to the memory to free
 */
void free(void *ptr);

/**
 * @brief Allocate and zero-initialize an array
 *
 * @param nmemb Number of elements
 * @param size Size of each element
 * @return Pointer to the allocated memory, or NULL if allocation failed
 */
void *calloc(size_t nmemb, size_t size);

/**
 * @brief Reallocate memory
 *
 * @param ptr Pointer to the previously allocated memory
 * @param size Number of bytes to allocate
 * @return Pointer to the reallocated memory, or NULL if allocation failed
 */
void *realloc(void *ptr, size_t size);

/**
 * @brief Reallocate an array
 *
 * @param ptr Pointer to the previously allocated memory
 * @param nmemb Number of elements
 * @param size Size of each element
 * @return Pointer to the reallocated memory, or NULL if allocation failed
 */
void *reallocarray(void *ptr, size_t nmemb, size_t size);

/**
 * @brief Perform a binary search
 *
 * @param key Pointer to the key to search for
 * @param array Pointer to the array to search
 * @param count Number of elements in the array
 * @param size Size of each element
 * @param cmp Comparison function
 * @return Pointer to the found element, or NULL if not found
 */
void *bsearch(const void *key, const void *array,
	      size_t count, size_t size,
	      int (*cmp)(const void *key, const void *element));

/**
 * @brief Sort an array with a comparison function and an argument
 *
 * @param base Pointer to the array to sort
 * @param nmemb Number of elements in the array
 * @param size Size of each element
 * @param compar Comparison function
 * @param arg Argument to pass to the comparison function
 */
void qsort_r(void *base, size_t nmemb, size_t size,
	     int (*compar)(const void *, const void *, void *), void *arg);

/**
 * @brief Sort an array
 *
 * @param base Pointer to the array to sort
 * @param nmemb Number of elements in the array
 * @param size Size of each element
 * @param compar Comparison function
 */
void qsort(void *base, size_t nmemb, size_t size,
	   int (*compar)(const void *, const void *));

#define EXIT_SUCCESS 0 /**< Successful exit status */
#define EXIT_FAILURE 1 /**< Failure exit status */

/**
 * @brief Exit the program
 *
 * @param status Exit status
 */
FUNC_NORETURN void _exit(int status);

/**
 * @brief Exit the program
 *
 * @param status Exit status
 */
FUNC_NORETURN static inline void exit(int status)
{
	_exit(status);
}

/**
 * @brief Abort the program
 */
FUNC_NORETURN void abort(void);

#ifdef CONFIG_MINIMAL_LIBC_RAND
#define RAND_MAX INT_MAX /**< Maximum value returned by rand() */

/**
 * @brief Generate a random number
 *
 * @param seed Pointer to the seed
 * @return Random number
 */
int rand_r(unsigned int *seed);

/**
 * @brief Generate a random number
 *
 * @return Random number
 */
int rand(void);

/**
 * @brief Seed the random number generator
 *
 * @param seed Seed value
 */
void srand(unsigned int seed);
#endif /* CONFIG_MINIMAL_LIBC_RAND */

/**
 * @brief Compute the absolute value of an integer
 *
 * @param __n Integer value
 * @return Absolute value
 */
static inline int abs(int __n)
{
	return (__n < 0) ? -__n : __n;
}

/**
 * @brief Compute the absolute value of a long integer
 *
 * @param __n Long integer value
 * @return Absolute value
 */
static inline long labs(long __n)
{
	return (__n < 0L) ? -__n : __n;
}

/**
 * @brief Compute the absolute value of a long long integer
 *
 * @param __n Long long integer value
 * @return Absolute value
 */
static inline long long llabs(long long __n)
{
	return (__n < 0LL) ? -__n : __n;
}

/**
 * @brief Get the value of an environment variable
 *
 * @param name Name of the environment variable
 * @return Value of the environment variable, or NULL if not found
 */
char *getenv(const char *name);

#if _POSIX_C_SOURCE >= 200112L
/**
 * @brief Set the value of an environment variable
 *
 * @param name Name of the environment variable
 * @param val Value of the environment variable
 * @param overwrite Overwrite existing value if non-zero
 * @return 0 on success, or -1 on failure
 */
int setenv(const char *name, const char *val, int overwrite);

/**
 * @brief Unset an environment variable
 *
 * @param name Name of the environment variable
 * @return 0 on success, or -1 on failure
 */
int unsetenv(const char *name);
#endif

#ifdef _BSD_SOURCE
/**
 * @brief Get the value of an environment variable (reentrant)
 *
 * @param name Name of the environment variable
 * @param buf Buffer to store the value
 * @param len Length of the buffer
 * @return 0 on success, or -1 on failure
 */
int getenv_r(const char *name, char *buf, size_t len);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* ZEPHYR_LIB_LIBC_MINIMAL_INCLUDE_STDLIB_H_ */
//@GST