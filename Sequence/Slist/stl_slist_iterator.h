#ifndef MINISTL_STL_SLIST_ITERATOR_H
#define MINISTL_STL_SLIST_ITERATOR_H

#include <cstdlib>
#include "../../Iterator/stl_iterator.h"

namespace MiniSTL {
    /*------------------------------节点-------------------------*/
    struct slist_node_base {
        slist_node_base *next;
    };

    template<class T>
    struct slist_node : public slist_node_base {
        T data;
    };

    // insert
    inline slist_node_base *slist_make_link(slist_node_base *prev_node,
                                            slist_node_base *new_node) {
        new_node->next = prev_node->next;
        prev_node->next = new_node;
        return new_node;
    }

    // previous node
    inline const slist_node_base *
    slist_previous(const slist_node_base *head, const slist_node_base *node) {
        while (head && head->next != node) {
            head = head->next;
        }
        return head;
    }

    inline slist_node_base *slist_previous(slist_node_base *head, slist_node_base *node) {
        while (head && head->next != node) {
            head = head->next;
        }
        return head;
    }

    // size
    inline size_t slist_size(slist_node_base *base) {
        size_t n = 0;
        while (base) {
            base = base->next;
            ++n;
        }
        return n;
    }

    /*----------------------------iterator----------------------*/
    struct slist_iterator_base {
        // alias declaration
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator_category = forward_iterator_tag;

        // data
        slist_node_base *node;

        // ctor
        slist_iterator_base(slist_node_base *n) : node(n) {}

        void incur() { node = node->next; }

        //等值判断
        bool operator==(const slist_iterator_base &rhs) const {
            return node == rhs.node;
        }

        bool operator!=(const slist_iterator_base &rhs) const {
            return node != rhs.node;
        }
    };

    template<class T, class Ref, class Ptr>
    struct slist_iterator : public slist_iterator_base {
        using iterator = slist_iterator<T, T &, T *>;
        using const_iterator = slist_iterator<T, const T &, const T *>;
        using self = slist_iterator<T, Ref, Ptr>;

        using value_type = T;
        using pointer = Ptr;
        using reference = Ref;

        using list_node = slist_node<T>;

        // ctor
        slist_iterator(list_node *node) : slist_iterator_base(node) {}
        // 调用slit.end()时将触发slist_iterator(nullptr)
        slist_iterator() : slist_iterator_base(nullptr) {}
        slist_iterator(const iterator &rhs) : slist_iterator_base(rhs.node) {}

        // dereference
        reference operator*() { return static_cast<list_node *>(node)->data; }
        pointer operator->() { return &(operator*()); }

        // incur
        self &operator++() {
            incur();
            return *this;
        }

        self operator++(int) {
            self temp = *this;
            incur();
            return temp;
        }
    };

}

#endif //MINISTL_STL_SLIST_ITERATOR_H
