#ifndef MINISTL_STL_UNINITIALIZED_H
#define MINISTL_STL_UNINITIALIZED_H

#include <algorithm>
#include "stl_construct.h"
#include "../Iterator/type_traits.h"
#include "../Iterator/stl_iterator.h"

namespace MiniSTL {
    /***************************************************************************/
    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        if constexpr (_is_same<is_POD_type_t<value_type_t<InputIterator>>, _true_type>::value) {
//            memcpy(&*result, &*first, (last - first) * sizeof(*first));
//            return result+(last-first);
            return std::copy(first, last, result);   // TODO: change
        } else {
            ForwardIterator cur = result;
            for (; first != last; ++cur, ++first)
                construct(&*cur, *first);
            return cur;
        }
    }

    // extension
    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy_copy(InputIterator first1, InputIterator last1, InputIterator first2,
                                            InputIterator last2, ForwardIterator result) {
        ForwardIterator mid = uninitialized_copy(first1, last1, result);
        return uninitialized_copy(first2, last2, mid);
    }


    /***************************************************************************/
    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &value) {
        if constexpr (_is_same<is_POD_type_t<value_type_t<ForwardIterator>>, _true_type>::value) {
            std::fill(first, last, value);     // TODO: change
        } else {
            ForwardIterator cur = first;
            for (; cur != last; ++cur)
                construct(&*cur, value);
        }
    }

    /***************************************************************************/
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
                                                Size n, const T &x) {
        if constexpr (_is_same<is_POD_type_t<value_type_t<ForwardIterator>>, _true_type>::value) {
            return std::fill_n(first, n, x);   // TODO: change
        } else {
            ForwardIterator cur = first;
            for (; n > 0; --n, ++cur)
                construct(&*cur, x);
            return cur;
        }
    }
}

#endif //MINISTL_STL_UNINITIALIZED_H
