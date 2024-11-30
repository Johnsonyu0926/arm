//kernel/userspacea_handler.c
#include <zephyr/kernel.h>
#include <zephyr/internal/syscall_handler.h>
#include <zephyr/kernel_structs.h>
#include <zephyr/toolchain.h>

/**
 * @brief Validate a kernel object
 *
 * This function validates a kernel object based on its type and initialization
 * state.
 *
 * @param obj Pointer to the object
 * @param otype Type of the object
 * @param init Initialization check type
 * @return Pointer to the validated kernel object, or NULL if validation fails
 */
static struct k_object *validate_kernel_object(const void *obj,
					       enum k_objects otype,
					       enum _obj_init_check init)
{
	struct k_object *ko;
	int ret;

	ko = k_object_find(obj);

	/* This can be any kernel object and it doesn't have to be
	 * initialized
	 */
	ret = k_object_validate(ko, K_OBJ_ANY, _OBJ_INIT_ANY);
	if (ret != 0) {
#ifdef CONFIG_LOG
		k_object_dump_error(ret, obj, ko, otype);
#endif /* CONFIG_LOG */
		return NULL;
	}

	return ko;
}

/**
 * @brief Validate any kernel object
 *
 * This function validates any kernel object.
 *
 * @param obj Pointer to the object
 * @return Pointer to the validated kernel object, or NULL if validation fails
 */
static ALWAYS_INLINE struct k_object *validate_any_object(const void *obj)
{
	return validate_kernel_object(obj, K_OBJ_ANY, _OBJ_INIT_ANY);
}

/**
 * @brief Check if a kernel object is valid
 *
 * This function checks if a kernel object is valid based on its type.
 *
 * @param obj Pointer to the object
 * @param otype Type of the object
 * @return true if the object is valid, false otherwise
 */
bool k_object_is_valid(const void *obj, enum k_objects otype)
{
	struct k_object *ko;

	ko = validate_kernel_object(obj, otype, _OBJ_INIT_TRUE);

	return (ko != NULL);
}

/**
 * @brief Grant access to a kernel object
 *
 * This function grants access to a kernel object for a specified thread.
 *
 * @param object Pointer to the object
 * @param thread Pointer to the thread
 */
static inline void z_vrfy_k_object_access_grant(const void *object,
						struct k_thread *thread)
{
	struct k_object *ko;

	K_OOPS(K_SYSCALL_OBJ_INIT(thread, K_OBJ_THREAD));
	ko = validate_any_object(object);
	K_OOPS(K_SYSCALL_VERIFY_MSG(ko != NULL, "object %p access denied",
				    object));
	k_thread_perms_set(ko, thread);
}
#include <zephyr/syscalls/k_object_access_grant_mrsh.c>

/**
 * @brief Release a kernel object
 *
 * This function releases a kernel object for the current thread.
 *
 * @param object Pointer to the object
 */
static inline void z_vrfy_k_object_release(const void *object)
{
	struct k_object *ko;

	ko = validate_any_object(object);
	K_OOPS(K_SYSCALL_VERIFY_MSG(ko != NULL, "object %p access denied", object));
	k_thread_perms_clear(ko, _current);
}
#include <zephyr/syscalls/k_object_release_mrsh.c>

/**
 * @brief Allocate a kernel object
 *
 * This function allocates a kernel object of the specified type.
 *
 * @param otype Type of the object
 * @return Pointer to the allocated object, or NULL if allocation fails
 */
static inline void *z_vrfy_k_object_alloc(enum k_objects otype)
{
	return z_impl_k_object_alloc(otype);
}
#include <zephyr/syscalls/k_object_alloc_mrsh.c>

/**
 * @brief Allocate a kernel object with a specified size
 *
 * This function allocates a kernel object of the specified type and size.
 *
 * @param otype Type of the object
 * @param size Size of the object
 * @return Pointer to the allocated object, or NULL if allocation fails
 */
static inline void *z_vrfy_k_object_alloc_size(enum k_objects otype, size_t size)
{
	return z_impl_k_object_alloc_size(otype, size);
}
#include <zephyr/syscalls/k_object_alloc_size_mrsh.c>
//GST