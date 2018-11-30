#ifndef MINISTL_STL_ALGOBASE_H
#define MINISTL_STL_ALGOBASE_H

#include <cstdlib>
#include <cstring>
#include <utility>
#include "../../Iterator/type_traits.h"
#include "../../Iterator/stl_iterator.h"

namespace MiniSTL {
    // 取两个数中的较大值
    template<class T>
    inline const T &max(const T &a, const T &b) {
        return a < b ? b : a;
    }

    template<class T, class Compare>
    inline const T &max(const T &a, const T &b, Compare comp) {
        return comp(a, b) ? b : a;
    }

    // 取两个数中的较小值
    template<class T>
    inline const T &min(const T &a, const T &b) {
        return b < a ? b : a;
    }

    template<class T, class Compare>
    inline const T &min(const T &a, const T &b, Compare comp) {
        return comp(b, a) ? b : a;
    }

    // 判断[first1, last1)与[first2, first2+(last1-first1))是否相等 (不能保证长度相等)
    template<class InputIterator1, class InputIterator2>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        for (; first1 != last1; ++first1, ++first2)
            if (*first1 != *first2)
                return false;
        return true;
    }

    template<class InputIterator1, class InputIterator2, class BinaryPredicate>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
                      BinaryPredicate binary_pred) {
        for (; first1 != last1; ++first1, ++first2)
            if (!binary_pred(*first1, *first2))
                return false;
        return true;
    }

    // [first, last)用value填充
    template<class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T &value) {
        for (; first != last; ++first)
            *first = value;
    }

    // [first, last)前n个元素改写为value
    template<class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator first, Size n, const T &value) {
        for (; n > 0; --n, ++first)
            *first = value;
        return first;
    }

    // 交换迭代器a, b内的元素
    template<class ForwardIterator1, class ForwardIterator2>
    inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
        auto tmp = *a;
        *a = *b;
        *b = tmp;
    }

    // 调换两个对象内容
    template<class T>
    inline void swap(T &a, T &b) {
        T temp = a;
        a = b;
        b = temp;
    }

    // 以字典排列方式对[first1, last1)和[fist2, last2)进行比较
    template<class InputIterator1, class InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                 InputIterator2 first2, InputIterator2 last2) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (*first1 < *first2)
                return true;
            else if (*first1 > *first2)
                return false;
        }
        return first1 == last1 && first2 != last2; // 若第二序列有余, 返回true, 否则false
    }

    template<class InputIterator1, class InputIterator2, class Compare>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                 InputIterator2 first2, InputIterator2 last2,
                                 Compare comp) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (comp(*first1, *first2))
                return true;
            else if (comp(*first2, *first1))
                return false;
        }
        return first1 == last1 && first2 != last2; // 若第二序列有余, 返回true, 否则false
    }

    inline bool lexicographical_compare(const unsigned char *first1, const unsigned char *last1,
                                        const unsigned char *first2, const unsigned char *last2) {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, min(len1, len2));
        return result != 0 ? result < 0 : len1 < len2;
    }

    inline bool lexicographical_compare(const char *first1, const char *last1,
                                        const char *first2, const char *last2) {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, min(len1, len2));
        return result != 0 ? result < 0 : len1 < len2;
    }

    // 返回[first1, last1)与[first2, last2)各自第一个不匹配的迭代器 (需要保证第二个序列长于第一个序列)
    template<class InputIterator1, class InputIterator2>
    std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
                                                       InputIterator2 first2, InputIterator2 last2) {
        while (first1 != last1 && *first1 == *first2) {
            ++first1, ++first2;
        }
        return std::pair<InputIterator1, InputIterator2>(first1, first2);
    }

    template<class InputIterator1, class InputIterator2, class BinaryPredicate>
    std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
                                                       InputIterator2 first2, InputIterator2 last2,
                                                       BinaryPredicate binary_pred) {
        while (first1 != last1 && binary_pred(*first1, *first2)) {
            ++first1, ++first2;
        }
        return std::pair<InputIterator1, InputIterator2>(first1, first2);
    }

    /* --------------------------------copy 函数----------------------------------- */
    // 类型1.1 完全泛化版本
    template<class InputIterator, class OutputIterator>
    struct __copy_dispatch {       // 仿函数对象
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
            if constexpr (_is_same<iterator_category_t<InputIterator>, random_access_iterator_tag>::value) {
                // 以n决定循环的执行次数, 速度快
                for (auto n = last - first; n > 0; --n, ++result, ++first)
                    *result = *first;
                return result;
            } else {
                // 以迭代器是否等同来决定循环是否继续, 速度慢
                for (; first != last; ++result, ++first)
                    *result = *first;
                return result;
            }
        }
    };

    // 类型1.2: 偏特化版本(1), 两个参数都是T*指针形式
    template<class T>
    struct __copy_dispatch<T *, T *> {
        T *operator()(T *first, T *last, T *result) {
            if constexpr (_is_same<has_trivial_assignment_operator_t<T>, _true_type>::value) {
                memmove(result, first, sizeof(T) * (last - first));
                return result + (last - first);
            } else {
                for (auto n = last - first; n > 0; --n, ++result, ++first)
                    *result = *first;
                return result;
            }
        }
    };

    // 类型1.3: 偏特化版本(2), 一个const T*, 另一个T*
    template<class T>
    struct __copy_dispatch<const T *, T *> {
        T *operator()(const T *first, const T *last, T *result) {
            if constexpr (_is_same<has_trivial_assignment_operator_t<T>, _true_type>::value) {
                memmove(result, first, sizeof(T) * (last - first));
                return result + (last - first);
            } else {
                for (auto n = last - first; n > 0; --n, ++result, ++first)
                    *result = *first;
                return result;
            }
        }
    };

    // 类型1: 完全泛化版本
    template<class InputIterator, class OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }


    // 类型2: 针对原生指针const char*和const wchar_t*进行内存拷贝
    inline char *copy(const char *first, const char *last, char *result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    inline wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    /* -----------------------------copy_backward 函数---------------------------- */
    // 类型1.1 完全泛化版本
    template<class BI1, class BI2>
    struct __copy_backward_dispatch {
        BI2 operator()(BI1 first, BI1 last, BI2 result) {
            if constexpr (_is_same<iterator_category_t<BI1>, random_access_iterator_tag>::value) {
                for (auto n = last - first; n > 0; --n)
                    *--result = *--last;
                return result;
            } else {
                while (first != last)
                    *--result = *--last;
                return result;
            }
        }
    };

    // 类型1.2: 偏特化版本(1), 两个参数都是T*指针形式
    template<class T>
    struct __copy_backward_dispatch<T *, T *> {
        T *operator()(T *first, T *last, T *result) {
            if constexpr (_is_same<has_trivial_assignment_operator_t<T>, _true_type>::value) {
                auto num = last - first;
                memmove(result - num, first, sizeof(T) * num);
                return result - num;
            } else {
                for (auto n = last - first; n > 0; --n)
                    *--result = *--last;
                return result;
            }
        }
    };

    // 类型1.2: 偏特化版本(1), 两个参数都是T*指针形式
    template<class T>
    struct __copy_backward_dispatch<const T *, T *> {
        T *operator()(const T *first, const T *last, T *result) {
            if constexpr (_is_same<has_trivial_assignment_operator_t<T>, _true_type>::value) {
                auto num = last - first;
                memmove(result - num, first, sizeof(T) * num);
                return result - num;
            } else {
                for (auto n = last - first; n > 0; --n)
                    *--result = *--last;
                return result;
            }
        }
    };

    // 类型1: 完全泛化版本
    // Note: BI=BidirectionalIterator
    template<class BI1, class BI2>
    inline BI2 copy_backward(BI1 first, BI1 last, BI2 result) {
        return __copy_backward_dispatch<BI1, BI2>()(first, last, result);
    }
}

#endif //MINISTL_STL_ALGOBASE_H
