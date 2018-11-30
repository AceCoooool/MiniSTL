#ifndef MINISTL_STL_CONSTRUCT_H
#define MINISTL_STL_CONSTRUCT_H

#include "../Iterator/type_traits.h"

namespace MiniSTL {
    template<class T1, class T2>
    inline void construct(T1 *ptr1, const T2 &value) {
        new(ptr1) T1(value);
    }

    template<typename T>
    inline void destroy(T *ptr) {
        ptr->~T();
    }

    template<typename ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        if constexpr (_is_same<is_POD_type_t<ForwardIterator>, _false_type>::value) {
            for (; first != last; ++first)
                destroy(&*first);
        } else
            return;
    }
}

#endif //MINISTL_STL_CONSTRUCT_H
