#ifndef MINISTL_STL_LIST_ITERATOR_H
#define MINISTL_STL_LIST_ITERATOR_H

#include "../../Iterator/stl_iterator.h"

namespace MiniSTL {
    // the class of node
    template<class T>
    struct list_node {
        list_node<T> *prev;
        list_node<T> *next;
        T data;
    };

    // the class of list iterator
    // 采用三个类型主要保证迭代器可变, 但是其结点可以是常量
    template<class T, class Ref, class Ptr>
    struct list_iterator {
        using iterator_category = bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = Ptr;
        using reference = Ref;
        using size_type = size_t;
        using iterator = list_iterator<T, T &, T *>;
        using self = list_iterator<T, Ref, Ptr>;
        using link_type = list_node<T> *;

        link_type node;    // 迭代器内部存在一个raw pointer指向list_node

        // constructor
        list_iterator() {}
        list_iterator(link_type x) : node(x) {}
        list_iterator(const iterator &x) : node(x.node) {}

        // list_iterator析构函数为trivial

        // dererence & member access
        reference operator*() const { return node->data; }
        pointer operator->() const { return &(operator*()); }

        // 自增
        self &operator++() {
            node = static_cast<link_type>(node->next);  // 将link_node*强制转为迭代器
            return *this;
        }

        self operator++(int i) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        //自减
        self &operator--() {
            node = static_cast<link_type>(node->prev);  // 将__link_node*强制转为迭代器
            return *this;
        }

        self operator--(int i) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        template<class T1, class Ref1, class Ptr1>
        friend bool operator==(const list_iterator<T1, Ref1, Ptr1> &lhs, const list_iterator<T1, Ref1, Ptr1> &rhs);

        template<class T1, class Ref1, class Ptr1>
        friend bool operator!=(const list_iterator<T1, Ref1, Ptr1> &lhs, const list_iterator<T1, Ref1, Ptr1> &rhs);
    };

    template<class T, class Ref, class Ptr>
    bool operator==(const list_iterator<T, Ref, Ptr> &lhs, const list_iterator<T, Ref, Ptr> &rhs) {
        return (lhs.node->prev == rhs.node->prev &&
                lhs.node->next == rhs.node->next && lhs.node->data == rhs.node->data);
    }

    template<class T, class Ref, class Ptr>
    bool operator!=(const list_iterator<T, Ref, Ptr> &lhs, const list_iterator<T, Ref, Ptr> &rhs) {
        return !(lhs == rhs);
    }
}

#endif //MINISTL_STL_LIST_ITERATOR_H
