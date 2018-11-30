#ifndef MINISTL_STL_RB_TREE_ITERATOR_H
#define MINISTL_STL_RB_TREE_ITERATOR_H

#include "../../Iterator/stl_iterator.h"

namespace MiniSTL {
    using rb_tree_color_type = bool;
    const rb_tree_color_type rb_tree_red = false;
    const rb_tree_color_type rb_tree_black = true;

    /*------------------------------节点-------------------------*/
    struct rb_tree_node_base {
        using color_type = rb_tree_color_type;
        using base_ptr = rb_tree_node_base *;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr minimum(base_ptr root);
        static base_ptr maximum(base_ptr root);
    };

    template<class T>
    struct rb_tree_node : public rb_tree_node_base {
        using link_type = rb_tree_node<T> *;
        T value_field;    // 节点值
    };

    /*----------------------------iterator----------------------*/
    struct rb_tree_base_iterator {
        using base_ptr = rb_tree_node_base::base_ptr;
        using iterator_category = bidirectional_iterator_tag;
        using difference_type = ptrdiff_t;

        base_ptr node;   // 用来与容器之间产生一个连结关系

        void increment();
        void decrement();
    };

    template<class T, class Ref, class Ptr>
    struct rb_tree_iterator : public rb_tree_base_iterator {
        using value_type = T;
        using reference = Ref;
        using pointer = Ptr;
        using iterator = rb_tree_iterator<T, T &, T *>;
        using const_iterator = rb_tree_iterator<T, const T &, const T *>;
        using self = rb_tree_iterator<T, Ref, Ptr>;
        using link_type = rb_tree_node<T> *;

        rb_tree_iterator() {}
        rb_tree_iterator(link_type x) { node = x; }
        rb_tree_iterator(const iterator &it) { node = it.node; }

        reference operator*() { return static_cast<link_type>(node)->value_field; }
        pointer operator->() { return &(operator*()); }

        self &operator++();
        self operator++(int);
        self &operator--();
        self operator--(int);

    };
}

#endif //MINISTL_STL_RB_TREE_ITERATOR_H
