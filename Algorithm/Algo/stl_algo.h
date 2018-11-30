#ifndef MINISTL_STL_ALGO_H
#define MINISTL_STL_ALGO_H

#include "../../Iterator/stl_iterator.h"
#include "../../Iterator/iterator_adapters.h"
#include "../Algobase/stl_algobase.h"
#include "../Heap/stl_heap.h"

namespace MiniSTL {
    // 查找第一次出现的相邻重复元素
    template<class ForwardIterator>
    ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
        if (first == last) return last;
        ForwardIterator next = first;
        while (++next != last) {
            if (*first == *next) return first;
            else first = next;
        }
        return last;
    }

    template<class ForwardIterator, class BinaryPredicate>
    ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
        if (first == last) return last;
        ForwardIterator next = first;
        while (++next != last) {
            if (binary_pred(*first, *next)) return first;
            else first = next;
        }
        return last;
    }

    // 返回[first, last)与value相等的元素个数
    template<class InputIterator, class T>
    typename iterator_traits<InputIterator>::difference_type
    count(InputIterator first, InputIterator last, const T &value) {
        difference_type_t<InputIterator> count = 0;
        for (; first != last; ++first)
            if (*first == value)
                ++count;
        return count;
    }

    // 返回[first, last)内使得pred(x)为true的个数
    template<class InputIterator, class Predicate>
    typename iterator_traits<InputIterator>::difference_type
    count_if(InputIterator first, InputIterator last, Predicate pred) {
        difference_type_t<InputIterator> count = 0;
        for (; first != last; ++first)
            if (pred(*first))
                ++count;
        return count;
    }

    // 返回[first, last)第一个等于value的迭代器
    template<class InputIterator, class T>
    InputIterator find(InputIterator first, InputIterator last, const T &value) {
        while (first != last && *first != value) ++first;
        return first;
    }

    // 返回[first, last)第一个使得pred(*iter)为true的迭代器
    template<class InputIterator, class Predicate>
    InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
        while (first != last && !pred(*first)) ++first;
        return first;
    }

    // 在区间S1中查找区间S2首次出现的位置, 若不匹配则返回last1
    template<class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                                   ForwardIterator2 first2, ForwardIterator2 last2) {
        auto d1 = MiniSTL::distance(first1, last1);
        auto d2 = MiniSTL::distance(first2, last2);
        if (d1 < d2) return last1;   // 第二个序列长于第一个序列, 显然不能成为其子序列
        auto current1 = first1;
        auto current2 = first2;

        while (current2 != last2) {
            if (*current1 == *current2) {
                ++current1;
                ++current2;
            } else {
                if (d1 == d2)
                    return last1;
                else {
                    current1 = ++first1;
                    current2 = first2;
                    --d1;
                }
            }
        }
        return first1;
    }

    // search_n: 查找“连续count个符合条件之元素”所形成的子序列, 并返回该子序列起点
    template<class ForwardIterator, class Integer, class T>
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T &value) {
        if (count < 0)
            return first;
        else {
            first = MiniSTL::find(first, last, value);     // 找出下一次出现点
            while (first != last) {
                Integer n = count - 1;            // value应当再出现n次
                ForwardIterator i = first;
                ++i;
                while (i != last && n != 0 && *i == value) // 接下来也是value
                    ++i, --n;
                if (n == 0)
                    return first;
                else
                    first = MiniSTL::find(i, last, value); // 以i为起点开始接着寻找, 在i之前均没有正确起点
            }
            return last;
        }
    }

    // search_n:谓词版本
    template<class ForwardIterator, class Integer, class T, class BinaryPredicate>
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count,
                             const T &value, BinaryPredicate binary_pred) {
        if (count < 0)
            return first;
        else {
            while (first != last) {
                if (binary_pred(*first, value)) break;
                ++first;
            }
            while (first != last) {
                Integer n = count - 1;         // value应当再出现n次
                ForwardIterator i = first;
                ++i;
                while (i != last && n != 0 && binary_pred(*i, value)) // 接下来也是value
                    ++i, --n;
                if (n == 0)
                    return first;
                else {
                    while (i != last) {
                        if (binary_pred(*i, value)) break;
                        ++i;
                    }
                    first = i;
                }
            }
            return last;
        }
    }

    // find_end:在区间S1[first1,last1)中找出区间S2[first2,last2)的最后出现点
    // 若不存在, 返回last1
    template<class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                                     ForwardIterator2 first2, ForwardIterator2 last2) {
        using category1 = typename iterator_traits<ForwardIterator1>::iterator_category;
        using category2 = typename iterator_traits<ForwardIterator2>::iterator_category;
        // bidirectional情况
        if constexpr (_is_same<category1, bidirectional_iterator_tag>::value &&
                _is_same<category2, bidirectional_iterator_tag>::value) {
            using reviter1 = reverse_iterator<ForwardIterator1>;
            using reviter2 = reverse_iterator<ForwardIterator2>;

            reviter1 rlast1(first1);
            reviter2 rlast2(first2);
            auto rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);
            if (rresult == rlast1)   // 没找到
                return last1;
            else {
                auto result = rresult.base();
                advance(result, -distance(first2, last2));
                return result;
            }
        } else {  // forward情况
            if (first2 == last2) return last1;
            else {
                auto result = last1;
                while (1) {
                    auto new_result = MiniSTL::search(first1, last1, first2, last2);
                    if (new_result == last1)
                        return result;
                    else {
                        result = new_result;
                        first1 = new_result;
                        ++first1;
                    }
                }
            }
        }
    }

    // 在[first1, last1)中寻找第一个属于[first2, last2)的元素
    template<class InputIterator, class ForwardIterator>
    InputIterator find_first_of(InputIterator first1, InputIterator last1,
                                ForwardIterator first2, ForwardIterator last2) {
        for (; first1 != last1; ++first1)
            for (ForwardIterator iter = first2; iter != last2; ++iter)
                if (*first1 == *iter) return first1;
        return last1;
    }

    template<class InputIterator, class ForwardIterator, class BinaryPredicate>
    InputIterator find_first_of(InputIterator first1, InputIterator last1,
                                ForwardIterator first2, ForwardIterator last2, BinaryPredicate comp) {
        for (; first1 != last1; ++first1)
            for (ForwardIterator iter = first2; iter != last2; ++iter)
                if (comp(*first1, *iter)) return first1;
        return last1;
    }

    // for_each: 将f作用于区间[first, last)每个元素之上, 不能改变元素, 欲改变应当使用transform
    // 具备一个通常被忽略的返回值
    template<class InputIterator, class Function>
    Function for_each(InputIterator first, InputIterator last, Function f) {
        for (; first != last; ++first)
            f(*first);
        return f;
    }

    // generate: 将gen的运算结果填写于[first,last), 采用assignment
    template<class InputIterator, class Generator>
    void generate(InputIterator first, InputIterator last, Generator gen) {
        for (; first != last; ++first)
            *first = gen();
    }

    // generate_n:将gen的运算结果填写于[first, first+n), 采用assignment
    template<class OutputIterator, class Size, class Generator>
    OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
        for (; n > 0; --n, ++first)
            *first = gen();
        return first;
    }

    //include: 判断S2是否被S1所包含 (要求有序, 默认为升序)
    template<class InputIterator1, class InputIterator2>
    bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1)  // 一个不在则不可能包含
                return false;
            else if (*first1 < *first2)
                ++first1;
            else
                ++first1, ++first2;
        }
        return first2 == last2;
    }

    // comp必须满足等价判定表达式 (严格降序或升序)
    template<class InputIterator1, class InputIterator2, class Compare>
    bool includes(InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, InputIterator2 last2, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1))
                return false;
            else if (comp(*first1, *first2))
                ++first1;
            else
                ++first1, ++first2;
        }
        return first2 == last2;
    }

    // max_element:返回指向序列中数值最大元素的迭代器
    template<class ForwardIterator>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
        if (first == last)
            return first;
        ForwardIterator result = first;
        while (++first != last)
            if (*result < *first)
                result = first;
        return result;
    }

    template<class ForwardIterator, class Compare>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
        if (first == last) return first;
        ForwardIterator result = first;
        while (++first != last)
            if (comp(*result, *first))
                result = first;
        return result;
    }

    // min_element:返回指向序列中数值最小元素的迭代器
    template<class ForwardIterator>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
        if (first == last)
            return first;
        ForwardIterator result = first;
        while (++first != last)
            if (*first < *result)
                result = first;
        return result;
    }

    template<class ForwardIterator, class Compare>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
        if (first == last)
            return first;
        ForwardIterator result = first;
        while (++first != last)
            if (comp(*first, *result))
                result = first;
        return result;
    }

    // merge:将有序的S1与S2合并置于另一段空间 (合并结果仍然有序,返回一个迭代器指向新区间的last)
    template<class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1)
                *result++ = *first2, ++first2;
            else
                *result++ = *first1, ++first1;
        }
        return MiniSTL::copy(first2, last2, MiniSTL::copy(first1, last1, result));
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
                         InputIterator2 last2, OutputIterator result, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1))
                *result++ = *first2, ++first2;
            else
                *result++ = *first1, ++first1;
        }
        return MiniSTL::copy(first2, last2, copy(first1, last1, result));
    }

    // merge_backward
    template<class BidirectionalIter1, class BidirectionalIter2, class BidirectionalIter3>
    BidirectionalIter3 merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
                                      BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter3 result) {
        if (first1 == last1)
            return MiniSTL::copy_backward(first2, last2, result);
        if (first2 == last2)
            return MiniSTL::copy_backward(first1, last1, result);
        --last1;
        --last2;
        while (true) {
            if (*last2 < *last1) {
                *--result = *last1;
                if (first1 == last1)
                    return MiniSTL::copy_backward(first2, ++last2, result);
                --last1;
            } else {
                *--result = *last2;
                if (first2 == last2)
                    return MiniSTL::copy_backward(first1, ++last1, result);
                --last2;
            }
        }
    }

    template<class BidirectionalIter1, class BidirectionalIter2, class BidirectionalIter3, class Compare>
    BidirectionalIter3 merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1, BidirectionalIter2 first2,
                                      BidirectionalIter2 last2, BidirectionalIter3 result, Compare comp) {
        if (first1 == last1)
            return copy_backward(first2, last2, result);
        if (first2 == last2)
            return copy_backward(first1, last1, result);
        --last1;
        --last2;
        while (true) {
            if (comp(*last2, *last1)) {
                *--result = *last1;
                if (first1 == last1)
                    return copy_backward(first2, ++last2, result);
                --last1;
            } else {
                *--result = *last2;
                if (first2 == last2)
                    return copy_backward(first1, ++last1, result);
                --last2;
            }
        }
    }

    // partition:将被pred判定为true的元素都放到前段, 被pred判断为false的元素都放到后段
    template<class BidirectionalIterator, class Predicate>
    BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
        while (true) {
            while (true) {           // 寻找前面第一个使得pred为false的元素
                if (first == last)
                    return first;
                else if (pred(*first))
                    ++first;
                else
                    break;          // 找到了需要被移动的元素
            }
            --last;
            while (true)            // 寻找后面第一个使得pred为true的元素
                if (first == last)
                    return first;
                else if (!pred(*last))
                    --last;
                else
                    break;
            MiniSTL::iter_swap(first, last);
            ++first;
        }
    }

    // remove_copy: 将[first, last)不等于value的结果拷贝到result
    template<class InputIterator, class OutputIterator, class T>
    OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, T value) {
        for (; first != last; ++first)
            if (*first != value)
                *result++ = *first;
        return result;
    }

    // remove: 移除[first, last)等于value的情况. (本身并不做remove操作, 只是将元素后移)
    template<class ForwardIterator, class T>
    ForwardIterator remove(ForwardIterator first, ForwardIterator last, T value) {
        first = MiniSTL::find(first, last, value);    // 循序找出第一个相等元素
        ForwardIterator next = first;
        return first == last ? first : MiniSTL::remove_copy(++next, last, first, value);
    }

    // remove_copy_if: 在remove_copy的基础上加入了谓词
    template<class InputIterator, class OutputIterator, class Predicate>
    OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred) {
        for (; first != last; ++first)
            if (!pred(*first))
                *result++ = *first;
        return result;
    }

    // remove_if: 将[first, last)使得pred(*iter)移除掉
    template<class ForwardIterator, class Predicate>
    ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
        first = MiniSTL::find_if(first, last, pred);
        ForwardIterator next = first;
        return first == last ? first : MiniSTL::remove_copy_if(++next, last, first, pred);
    }

    // replace:将[first,last)区间内所有old_value替换为new_value
    template<class ForwardIterator, class T>
    void replace(ForwardIterator first, ForwardIterator last, const T &old_value, const T &new_value) {
        for (; first != last; ++first)
            if (*first == old_value) *first = new_value;
    }

    // replace_copy: 同replace, 只是将结果拷贝到result
    template<class InputIterator, class OutputIterator, class T>
    OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result,
                                const T &old_value, const T &new_value) {
        for (; first != last; ++first, ++result)
            *result = *first == old_value ? new_value : *first;
        return result;
    }

    // replace_if: [first, last)所有被pred判定为true的元素用new_value取代
    template<class ForwardIterator, class Predicate, class T>
    void replace_if(ForwardIterator first, ForwardIterator last,
                    Predicate pred, const T &new_value) {
        for (; first != last; ++first)
            if (pred(*first)) *first = new_value;

    }

    // replace_copy_if:在replace_copy的基础上加入了谓词
    template<class InputIterator, class OutputIterator, class Predicate, class T>
    OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator result,
                                   Predicate pred, const T &new_value) {
        for (; first != last; ++first, ++result)
            *result = pred(*first) ? new_value : *first;
        return result;
    }

    // reverse: 将[first, last)的元素在容器中颠倒重排
    template<class BidirectionalIterator>
    inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
        if constexpr (_is_same<iterator_category_t<BidirectionalIterator>, random_access_iterator_tag>::value) {
            while (first < last)    // 只有random_access才能有此判断
                MiniSTL::iter_swap(first++, --last);
        } else {
            while (true) {
                if (first == last || first == --last) {  // --last在这里就操作了
                    return;
                } else {
                    MiniSTL::iter_swap(first++, last);
                }
            }
        }
    }

    // reverse_copy: 将[first, last)元素颠倒, 放置在result中
    template<class BidirectionalIterator, class OutputIterator>
    OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
        while (first != last) {
            --last;
            *result = *last;
            ++result;
        }
        return result;
    }

    // gcd: 求m和n的最大公因子 (采用辗转相除法)
    template<class EuclideanRingElement>
    EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
        while (n != 0) {
            EuclideanRingElement t = m % n;
            m = n;
            n = t;
        }
        return m;
    }

    // rotate:将区间[first,middle)与[middle,last)置换 (比如[1 2 3 4 5]以3为middle, 则结果为[3 4 5 1 2])
    template<class ForwardIterator>
    inline void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
        if (first == middle || middle == last)
            return;
        if constexpr (_is_same<iterator_category_t<ForwardIterator>, random_access_iterator_tag>::value) {
            auto n = __gcd(last - first, middle - first);
            auto shift = middle - first;
            auto initial = first + n, ptr1 = initial, ptr2 = ptr1 + shift;
            auto value = *(first + n);
            while (ptr2 != initial) {
                *ptr1 = *ptr2;
                ptr1 = ptr2;
                if (last - ptr2 > shift)
                    ptr2 += shift;
                else
                    ptr2 = first + (shift - (last - ptr2));
            }
            *ptr1 = value;
        } else if constexpr (_is_same<iterator_category_t<ForwardIterator>, bidirectional_iterator_tag>::value) {
            reverse(first, middle);
            reverse(middle, last);
            reverse(first, last);
        } else {
            for (ForwardIterator i = middle;;) {
                MiniSTL::iter_swap(first, i);      // 一一交换前端与后端元素
                ++first;
                ++i;
                // 判断前后段谁先结束
                if (first == middle) {    // 前段结束
                    if (i == last)        // 如果前端结束的同时后端也结束
                        return;
                    middle = i;           // 更新区间
                } else if (i == last)     // 后段先结束
                    i = middle;           // 更新区间
            }
        }
    }

    // rotate_copy:并不需要执行rotate, 只需要copy的时候注意次序即可
    template<class ForwardIterator, class OutputIterator>
    OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
                               OutputIterator result) {
        return MiniSTL::copy(first, middle, MiniSTL::copy(middle, last, result));
    }

    // swap_ranges:将S1区间[first1,last1)内的元素与以first2为起点, 长度与S1相同的区间作交换,
    // 返回指向S2中最后一个交换的元素的下一位 (若S2实际长度较小 或 S1 S2重叠, 执行结果未可预期)
    template<class ForwardIterato1, class ForwardIterator2>
    ForwardIterator2 swap_ranges(ForwardIterato1 first1, ForwardIterato1 last1, ForwardIterator2 first2) {
        for (; first1 != last1; ++first1, ++first2)
            MiniSTL::iter_swap(first1, first2);
        return first2;
    }

    // transform:
    // 第一版本: 以仿函数op作用于[first,last),并以其结果产生一个新序列
    // 第二版本: 以仿函数binary_op作用于一双元素之上, 其中一个来自[first1,last), 另一个来自[first2,...)
    template<class InputIterator, class OutputIterator, class UnaryOperator>
    OutputIterator transform(InputIterator first1, InputIterator last1, OutputIterator result, UnaryOperator op) {
        for (; first1 != last1; ++first1, ++result)
            *result = op(*first1);
        return result;
    }

    template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperator>
    OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
                             OutputIterator result, BinaryOperator binary_op) {
        for (; first1 != last1; ++first1, ++first2, ++result)
            *result = binary_op(*first1, *first2);
        return result;
    }

    // unique_copy: [first, last)不重复的元素复制到result中
    // 根据输出迭代器性质作不同的处理
    template<class InputIterator, class OutputIterator>
    OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
        if (first == last)
            return result;
        if constexpr (_is_same<iterator_category_t<OutputIterator>, output_iterator_tag>::value) {
            auto value = *first;
            *result = value;
            while (++first != last) {
                if (value != *first) {
                    value = *first;
                    *++result = value;
                }
            }
        } else {
            *result = *first;
            while (++first != last)
                if (*result != *first) *++result = *first;
            return ++result;
        }
    }

    // unique: 去除[first, last)内重复元素
    template<class ForwardIterator>
    ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
        first = MiniSTL::adjacent_find(first, last);      // 找到相邻重复元素的起点
        return MiniSTL::unique_copy(first, last, first);
    }

    /*------------------------------6.7.2~6.7.12---------------------------*/
    // lower_bound和upper_bound的区别主要出现在value存在在[first, last)的起来
    // lower_bound: 二分查找的一个版本, 返回指向第一个不小于value的元素的迭代器. 亦可认为是第一个可插入位置
    template<class ForwardIterator, class T>
    inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T &value) {
        int half;
        ForwardIterator middle;
        if constexpr (_is_same<iterator_category_t<ForwardIterator>, random_access_iterator_tag>::value) {
            auto len = last - first;
            while (len > 0) {
                half = len >> 1;
                middle = first + half;
                if (*middle < value) {
                    first = middle + 1;
                    len = len - half - 1;
                } else
                    len = half;
            }
        } else {
            auto len = MiniSTL::distance(first, last);
            while (len > 0) {
                half = len >> 1;
                middle = first;
                MiniSTL::advance(middle, half);    // 令middle指向中间位置
                if (*middle < value) {
                    first = middle;
                    ++first;              // 令first指向middle下一位置
                    len = len - half - 1;
                } else
                    len = half;
            }
        }
        return first;
    }

    //upper_bound:二分查找的一个版本, 返回可插入的最后一个位置
    template<class ForwardIterator, class T>
    inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T &value) {
        int half;
        ForwardIterator middle;
        if constexpr (_is_same<iterator_category_t<ForwardIterator>, random_access_iterator_tag>::value) {
            auto len = last - first;
            while (len > 0) {
                half = len >> 1;
                middle = first + half;
                if (value < *middle)
                    len = half;
                else {
                    first = middle + 1;
                    len = len - half - 1;
                }
            }
        } else {
            auto len = MiniSTL::distance(first, last);
            while (len > 0) {
                half = len >> 1;
                middle = first;
                MiniSTL::advance(middle, half);    // 令middle指向中间位置
                if (value < *middle)
                    len = half;
                else {
                    first = middle;
                    ++first;
                    len = len - half - 1;
                }
            }
        }
        return first;
    }

    //binary_search:在区间内查找元素,存在则返回true,否则返回false (本质上只需要调用lower_bound即可)
    template<class ForwardIterator, class T>
    bool binary_search(ForwardIterator first, ForwardIterator last, const T &value) {
        ForwardIterator i = MiniSTL::lower_bound(first, last, value);
        return i != last && !(value < *i);       //这里用的是等价判定而非相等判定
    }

    // next_permutation:字典序下的下一个排列
    // 算法精要: 从后向前, 找出一组相邻元素, 记第一元素为*i, 第二元素为*ii, 此二者满足*i<*ii
    // 再次从后向前, 找出第一个大于*i的元素, 记为*j, 将i与j对调, 再将ii之后的元素颠倒重排即可
    template<class BidirectionIterator>
    bool next_permutation(BidirectionIterator first, BidirectionIterator last) {
        if (first == last)
            return false;
        BidirectionIterator i = first;
        ++i;
        if (i == last)//仅有一个元素
            return false;

        i = last;
        --i;
        for (;;) {
            BidirectionIterator ii = i;
            --i;
            if (*i < *ii) {
                BidirectionIterator j = last;
                while (!(*i < *--j));//此时j必然存在，最不济也是ii
                MiniSTL::iter_swap(i, j);
                MiniSTL::reverse(ii, last);
                return true;
            }
            if (i == first) { // i已移动至队首且尚未找到ii, 直接颠倒全部
                MiniSTL::reverse(first, last);
                return false;
            }
        }
    }

    // pre_permutation:字典序下的上一个排列
    // 算法精要: 从后向前, 找出一组相邻元素, 记第一元素为*i, 第二元素为*ii, 此二者满足*i>*ii
    // 再次从后向前, 找出第一个小于*i的元素, 记为*j, 将i与j对调, 再将ii之后的元素颠倒重排即可
    template<class BidirectionIterator>
    bool prev_permutation(BidirectionIterator first, BidirectionIterator last) {
        if (first == last)
            return false;
        BidirectionIterator i = first;
        ++i;
        if (i == last)//仅有一个元素
            return false;

        i = last;
        --i;
        for (;;) {
            BidirectionIterator ii = i;
            --i;
            if (*i > *ii) {
                BidirectionIterator j = last;
                while (!(*--j < *i));//此时j必然存在，最不济也是ii
                MiniSTL::iter_swap(i, j);
                MiniSTL::reverse(ii, last);
                return true;
            }
            if (i == first) {//i已移动至队首且尚未找到ii，直接颠倒全部
                MiniSTL::reverse(first, last);
                return false;
            }
        }
    }

    // random_shuffle:将区间[first,last)内的元素随机重排, 即对于存在N个元素的区间/ 从N!的可能性中挑出一种
    // 版本1: 采用内部随机数生成器
    template<class RandomAccessIterator>
    inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        if (first == last) return;
        for (auto i = first + 1; i != last; ++i) {
#ifdef __STL_NO_DRAND48
            iter_swap(i, first+rand()%((i-first)+1));
#else
            iter_swap(i, first + lrand48() % ((i - first) + 1));
#endif
        }
    }

    // 版本2: 产生随机数的仿函数, 值得注意的是仿函数pass-by-reference而非value, 因为该仿函数存在局部状态
    template<class RandomAccessIterator, class RandomNumberGenerator>
    void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator &rand) {
        if (first == last)
            return;
        for (RandomAccessIterator i = first + 1; i != last; ++i)
            iter_swap(i, first + rand(i - first + 1));
    }

    // partial_sort:接收迭代器(first,middle,last), 使序列中的middle-first个元素以递增序置于[first,middle)中
    // 算法精要：将[first,middle)做成最大堆, 然后将[middle, last)中的元素与max-heap中的元素比较
    // 若小于最大值, 交换位置, 并重新维持max-heap  (在算法中的直接体现为pop_heap)
    // 因此当走完[first,last)时较大元素已被抽离[first,middle),最后再次以sort_heap对[first,middle)作出排序
    template<class RandomAccessIterator>
    inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
        MiniSTL::make_heap(first, middle);    // 见stl_heap.h
        using d_type = difference_type_t<RandomAccessIterator>;
        d_type len = middle - first;
        for (RandomAccessIterator i = middle; i < last; ++i)
            if (*i < *first) {
                auto value = *i;
                *i = *first;
                MiniSTL::adjust_heap(first, d_type(0), len, value);
            }

        MiniSTL::sort_heap(first, middle);
    }

    // insertion_sort辅助函数
    template<class RandomAccessIterator, class T>
    void __unguarded_linear_insert(RandomAccessIterator last, T value) {
        RandomAccessIterator next = last;
        --next;
        while (value < *next) {
            *last = *next;
            last = next;
            --next;
        }
        *last = value;
    }

    // insertion_sort:插入排序
    template<class RandomAccessIterator>
    void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (first == last)
            return;
        for (auto i = first + 1; i != last; ++i) {         // 外循环
            auto value = *i;
            auto tmp = i;
            if (value < *first) {
                MiniSTL::copy_backward(first, tmp, tmp + 1);
                *first = value;
            } else {
                MiniSTL::__unguarded_linear_insert(i, value);
            }
        }
    }

    template<class RandomAccessIterator>
    inline void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        for (RandomAccessIterator i = first; i != last; ++i)
            MiniSTL::__unguarded_linear_insert(i, *i);
    }

    // TODO: 再读一遍
    // QuickSort算法精要：假设S代表被处理的序列
    // 1.若S中的元素个数为0或1，结束
    // 2.任取S中的一个元素v作为轴（pivot)
    // 3.将S分割为L,R两段，使L内的每一个元素都小于等于v，R内的任何一个元素都大于等于v
    // 4.对LR递归执行Qucik sort

    // median:求取三点中值
    template<class T>
    inline const T &__median(const T &a, const T &b, const T &c) {
        if (a < b)
            return b < c ? b : (a < c ? c : a);
        else if (a < c)
            return a;          // c>a>=b
        else if (b < c)
            return c;          // a>=b,a>=c,b<c
        else
            return b;
    }

    // partitioining:分割, 其核心思想类似于前文算法partition
    template<class RandomAccessIterator, class T>
    RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot) {
        while (true) {
            while (*first < pivot)
                ++first;
            --last;
            while (pivot < *last)
                --last;
            if (!(first < last))
                return first;
            MiniSTL::iter_swap(first, last);
            ++first;
        }
    }

    // __lg:用以控制分割恶化的情况, 找出2^k<=n的最大值k
    // 举例而言, 当size=40时, __introsort_loop的最后一个参数为2*5, 即最多允许分割10层
    template<class Size>
    inline Size __lg(Size n) {
        Size k;
        for (k = 0; n > 1; n >>= 1)++k;
        return k;
    }

    enum { __stl_threshold = 16 };
    // __introsort_loop: intosort的具体实现
    // 结束排序后, [first,last)内有多个"元素个数少于16"的子序列, 每个子序列有一定程序的排序, 但尚未完全排序
    template<class RandomAccessIterator, class Size>
    void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, Size depth_limit) {
        // __STL_threshold是一个定义为16的全局常数
        while (last - first > __stl_threshold) {
            if (depth_limit == 0) {  // 已经产生了分割恶化
                MiniSTL::partial_sort(first, last, last);    // 改用heap-sort
                return;
            }
            --depth_limit;
            auto cut = __unguarded_partition(first, last, __median(*first, *(first + (last - first) / 2), *(last - 1)));
            __introsort_loop(cut, last, depth_limit);
            last = cut;    // 回归while, 执行左侧排序
        }
    }

    // __finial_insertion_sort: 最终的插入排序
    // 首先判断元素个数是否大于16, 若答案为是, 则将其分为两部分
    template<class RandomAccessIterator>
    void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (last - first > __stl_threshold) {
            __insertion_sort(first, first + __stl_threshold);
            __unguarded_insertion_sort(first + __stl_threshold, last);
        } else
            __insertion_sort(first, last);
    }

    // introsort: 在快排表现良好时使用快排, 在其表现不佳(分割导致了问题的恶化)时则转向使用heapsort, 从而确保O(NlogN)
    template<class RandomAccessIterator>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (first != last) {
            __introsort_loop(first, last, __lg(last - first) * 2);
            __final_insertion_sort(first, last);
        }
    }


    // equal_range:本质上返回了lower_bound与upper_bound组成的pair
    template<class ForwardIterator, class T>
    inline std::pair<ForwardIterator, ForwardIterator>
    equal_range(ForwardIterator first, ForwardIterator last, const T &value) {
        using Distance = difference_type_t<ForwardIterator>;
        Distance half;
        ForwardIterator middle, left, right;

        if (_is_same<iterator_category_t<ForwardIterator>, random_access_iterator_tag>::value) {
            auto len = last - first;
            while (len > 0) {
                half = len >> 1;
                middle = first + half;
                if (*middle < value) {
                    first = middle + 1;
                    len = len - half - 1;
                } else if (value < *middle)
                    len = half;
                else {        // 中央元素等于指定值
                    left = lower_bound(first, middle, value);
                    right = upper_bound(++middle, first + len, value);
                    return std::pair<ForwardIterator, ForwardIterator>(left, right);
                }
            }
        } else {
            auto len = distance(first, last);
            while (len > 0) {
                half = len >> 1;
                middle = first;
                advance(middle, half);
                if (*middle < value) {
                    first = middle;
                    ++first;
                    len = len - half - 1;
                } else if (value < *middle)
                    len = half;
                else {     // 中央元素等于指定值
                    left = lower_bound(first, middle, value);
                    advance(first, len);
                    right = upper_bound(++middle, first, value);
                    return std::pair<ForwardIterator, ForwardIterator>(left, right);
                }
            }
        }
        // 并未找到该元素,指向第一个大于value的元素
        return std::pair<ForwardIterator, ForwardIterator>(first, first);
    }

    template<class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
    BidirectionalIterator1
    __rotate_adaptive(BidirectionalIterator1 first, BidirectionalIterator1 middle, BidirectionalIterator1 last,
                      Distance len1, Distance len2, BidirectionalIterator2 buffer, Distance buffer_size) {
        BidirectionalIterator2 buffer_end;
        if (len1 > len2 && len2 <= buffer_size) {
            // 缓冲区足以安置序列2
            buffer_end = MiniSTL::copy(middle, last, buffer);
            MiniSTL::copy_backward(first, middle, last);
            return MiniSTL::copy(buffer, buffer_end, first);
        } else if (len1 <= buffer_size) {
            // 缓冲区足以安置序列1
            buffer_end = MiniSTL::copy(first, middle, buffer);
            MiniSTL::copy(middle, last, first);
            return MiniSTL::copy_backward(buffer, buffer_end, last);
        } else {
            // 缓冲区仍然不足
            MiniSTL::rotate(first, middle, last);
            MiniSTL::advance(first, len2);
            return first;
        }
    }

    template<class BidirectionalIterator, class Distance, class Pointer>
    void __merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
                          Distance len1, Distance len2, Pointer buffer, Distance buffer_size) {
        if (len1 <= len2 && len1 <= buffer_size) {
            // 缓冲区足以安置序列一
            Pointer end_buffer = MiniSTL::copy(first, middle, buffer);
            MiniSTL::merge(buffer, end_buffer, middle, last, first);
        } else if (len2 <= buffer_size) {
            // 缓冲区足以安置序列二
            Pointer end_buffer = MiniSTL::copy(first, middle, buffer);
            merge_backward(first, middle, buffer, end_buffer, last);
        } else {
            // 缓冲区不足以安置任何一个序列
            BidirectionalIterator first_cut = first;
            BidirectionalIterator second_cut = middle;
            Distance len11 = 0;
            Distance len22 = 0;
            if (len1 > len2) {     // 哪个序列长就分割哪个
                len11 = len1 / 2;
                MiniSTL::advance(first_cut, len11);
                second_cut = MiniSTL::lower_bound(middle, last, *first_cut);
                len22 = MiniSTL::distance(middle, second_cut);
            } else {
                len22 = len2 / 2;
                MiniSTL::advance(second_cut, len22);
                first_cut = MiniSTL::upper_bound(first, middle, *second_cut);
                len11 = MiniSTL::distance(first, first_cut);
            }
            BidirectionalIterator new_middle = MiniSTL::__rotate_adaptive(first_cut, middle, second_cut, len1 - len11,
                                                                          len22, buffer, buffer_size);
            // 针对左端递归
            __merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
            // 针对右端递归
            __merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size);
        }
    }

//    template<class BidirectionalIterator, class Distance>
//    void __merge_without_buffer(BidirectionalIterator first, BidirectionalIterator middle,
//                                BidirectionalIterator last, Distance len1, Distance len2) {
//        if (len1 == 0 || len2 == 0)
//            return;
//        if (len1 + len2 == 2) {
//            if (*middle < *first)
//                MiniSTL::iter_swap(first, middle);
//            return;
//        }
//        auto first_cut = first;
//        auto second_cut = middle;
//        Distance len11 = 0, len22 = 0;
//        if (len1 > len2) {
//            len11 = len1 / 2;
//            MiniSTL::advance(first_cut, len11);
//            second_cut = MiniSTL::lower_bound(middle, last, *first_cut);
//            len22 = MiniSTL::distance(middle, second_cut);
//        } else {
//            len22 = len2 / 2;
//            MiniSTL::advance(second_cut, len22);
//            first_cut = MiniSTL::upper_bound(first, middle, *second_cut);
//            len11 = MiniSTL::distance(first, first_cut);
//        }
    // TODO: add rotate return type
//        BidirectionalIterator new_middle = MiniSTL::rotate(first_cut, middle, second_cut);
//        __merge_without_buffer(first, first_cut, new_middle, len11, len22);
//        __merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
//    }

    template<class BidirectionalIterator>
    inline void __inplace_merge_aux(BidirectionalIterator first, BidirectionalIterator middle,
                                    BidirectionalIterator last) {
        using T = value_type_t<BidirectionalIterator>;
        using Distance = difference_type_t<BidirectionalIterator>;
        auto len1 = MiniSTL::distance(first, middle);
        auto len2 = MiniSTL::distance(middle, last);

        std::_Temporary_buffer<BidirectionalIterator, T> buf(first, last);   // 临时缓冲区
//        if (buf.begin() == 0)   // 内存分配失败情况
//            __merge_without_buffer(first, middle, last, len1, len2);
//        else
        __merge_adaptive(first, middle, last, len1, len2, buf.begin(), Distance(buf.size()));
    }

    // inplace_merge:stable, 在存在缓冲区的情况下性能较优
    template<class BidirectionalIterator>
    inline void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
        if (first == middle || middle == last)
            return;
        __inplace_merge_aux(first, middle, last);
    }

    // nth_element:重新排序[first,last), 使得nth指向的元素与完全排列后同一位置的元素同值
    // nth_element还保证[nth,last）内的元素必然不大于nth, 但对于[first,nth)与[nth,last)中的序列则毫无保证
    // 由此看来, nth_element更类似于partition而非partial_sort(后者采用heap_sort)
    template<class RandomAccessIterator>
    inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last) {
        while (last - first > 3) {
            // 三点中值法分割
            // 返回一个迭代器, 指向分割后右侧的第一个元素
            RandomAccessIterator cut = __unguarded_partition(
                    first, last, __median(*first, *(first + (last - first) / 2), *(last - 1)));
            if (cut <= nth)
                first = cut;    //右端起点<=nth,再次对右侧分割
            else
                last = cut;     //对左侧分割
        }
        __insertion_sort(first, last);
    }

    // mergesort::调用inplace_merge完成归并排序, 需要额外的缓冲区, 此外在内存不断移动(复制)元素亦需要较高成本, 弱于quick_sort
    template<class BidirectionalIter>
    void mergesort(BidirectionalIter first, BidirectionalIter last) {
        auto n = MiniSTL::distance(first, last);
        if (n == 0 || n == 1)
            return;
        else {
            BidirectionalIter mid = first + n / 2;
            mergesort(first, mid);
            mergesort(mid, last);
            MiniSTL::inplace_merge(first, mid, last);
        }
    }
}


#endif //MINISTL_STL_ALGO_H
