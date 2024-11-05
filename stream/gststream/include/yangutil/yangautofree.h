// Filename: YangAutoFree.h
// Score: 98

#ifndef INCLUDE_YANGUTIL_YANGAUTOFREE_H_
#define INCLUDE_YANGUTIL_YANGAUTOFREE_H_

#include <yangutil/yangtype.h>

#ifdef __cplusplus

// Macro for automatic resource management
#define YangAutoFree(className, instance) \
    impl_YangAutoFree<className> _auto_free_##instance(&instance, false)

#define YangAutoFreeA(className, instance) \
    impl_YangAutoFree<className> _auto_free_array_##instance(&instance, true)

// Template class for automatic resource management
template<class T>
class impl_YangAutoFree {
private:
    T** ptr;
    bool is_array;

public:
    impl_YangAutoFree(T** p, bool array) : ptr(p), is_array(array) {}

    virtual ~impl_YangAutoFree() {
        if (ptr == nullptr || *ptr == nullptr) {
            return;
        }

        if (is_array) {
            delete[] *ptr;
        } else {
            delete *ptr;
        }

        *ptr = nullptr;
    }
};

#endif // __cplusplus

#endif /* INCLUDE_YANGUTIL_YANGAUTOFREE_H_ */
// By GST @2024/10/28