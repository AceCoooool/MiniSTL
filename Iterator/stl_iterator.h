#ifndef MINISTL_STL_ITERATOR_H
#define MINISTL_STL_ITERATOR_H

#include <cstddef>  // ptrdiff_t
#include "type_traits.h"

namespace MiniSTL {
    // 五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    // 为了避免遗忘记录traits, 此基类以供自定义iterator继承之
    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T *, class Reference = T &>
    struct iterator {
        using iterator_category = Category;
        using value_type = T;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
    };

    // traits 展现Iterator所有特性
    // 之所以不直接使用别名模板完全替代是因为存在偏特化版本 (似乎别名模板无法偏特化)
    template<class Iterator>
    struct iterator_traits {
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    // 针对raw pointer设计的偏特化版本
    template<class T>
    struct iterator_traits<T *> {
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T *;
        using reference = T &;
    };

    // 针对pointer-to-const设计的偏特化版本
    template<class T>
    struct iterator_traits<const T *> {
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;
    };

    // 以下为模仿C++14 type_traits_t而设定的别名模板
    template<class Iterator>
    using iterator_category_t = typename iterator_traits<Iterator>::iterator_category;

    template<class Iterator>
    using value_type_t = typename iterator_traits<Iterator>::value_type;

    template<class Iterator>
    using difference_type_t = typename iterator_traits<Iterator>::difference_type;

    template<class Iterator>
    using pointer_t = typename iterator_traits<Iterator>::pointer;

    template<class Iterator>
    using reference_t = typename iterator_traits<Iterator>::reference;


    // distance, advance
    template<class InputIterator>
    inline difference_type_t<InputIterator> distance(InputIterator first, InputIterator last) {
        if constexpr (_is_same<iterator_category_t<InputIterator>, random_access_iterator_tag>::value)
            return last - first;
        else {
            difference_type_t<InputIterator> n = 0;
            while (first != last)
                ++first, ++n;
            return n;
        }
    }

    template<class InputIterator, class Distance>
    inline void advance(InputIterator &i, Distance n) {
        if constexpr (_is_same<iterator_category_t<InputIterator>, random_access_iterator_tag>::value)
            i += n;
        else if constexpr (_is_same<iterator_category_t<InputIterator>, bidirectional_iterator_tag>::value) {
            if (n >= 0)
                while (n--) ++i;
            else
                while (n++) --i;
        } else {
            while (n--) ++i;
        }
    }
}

#endif //MINISTL_STL_ITERATOR_H
