#ifndef MINISTL_STL_SLIST_IMP_H
#define MINISTL_STL_SLIST_IMP_H

#include <exception>
#include "stl_slist.h"

namespace MiniSTL {
    /***********************辅助函数(private): 放到前面方便理解***********************/
    template<class T, class Alloc>
    typename slist<T, Alloc>::list_node *slist<T, Alloc>::create_node(const value_type &value) {
        list_node *node = list_node_allocator::allocate();
        try {
            construct(&node->data, value);
            node->next = 0;
        } catch (std::exception &) {
            list_node_allocator::deallocate(node);
        }
        return node;
    }

    template<class T, class Alloc>
    void slist<T, Alloc>::destroy_node(list_node *node) {
        destroy(&node->data);
        list_node_allocator::deallocate(node);
    }

    template<class T, class Alloc>
    void slist<T, Alloc>::clear() {
        while (head.next) {
            pop_front();
        }
    }

    template<class T, class Alloc>
    typename slist<T, Alloc>::list_node *
    slist<T, Alloc>::insert_aux(list_node_base *pos, const value_type &x) {
        return static_cast<list_node *>(slist_make_link(pos, create_node(x)));
    }

    /***********************构造, 复制, 析构相关***********************/
    // 在stl_slist.h中

    /*****************************迭代器相关**************************/
    // 在stl_slist.h中

    /****************************修改容器相关*************************/
    template<class T, class Alloc>
    void slist<T, Alloc>::pop_front() {
        list_node *node = static_cast<list_node *>(head.next);
        head.next = node->next;
        destroy_node(node);
    }

    template<class T, class Alloc>
    typename slist<T, Alloc>::iterator
    slist<T, Alloc>::insert_after(iterator pos, const value_type &x) {
        return iterator(insert_aux(pos.node, x));
    }

    template<class T, class Alloc>
    typename slist<T, Alloc>::iterator
    slist<T, Alloc>::insert(iterator pos, const value_type &x) {
        auto prev = slist_previous(&head, pos.node);
        return iterator(insert_aux(prev, x));
    }


    template<class T, class Alloc>
    void slist<T, Alloc>::swap(slist &rhs) {
        list_node_base *temp = head.next;
        head.next = rhs.head.next;
        rhs.head.next = temp;
    }


}

#endif //MINISTL_STL_SLIST_IMP_H
