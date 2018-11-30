#ifndef MINISTL_STL_SLIST_H
#define MINISTL_STL_SLIST_H

#include "../../Allocator/allocator.h"
#include "stl_slist_iterator.h"

namespace MiniSTL {
    template<class T, class Alloc = allocator<T>>
    class slist {
    public:
        // alias declarations
        using value_type = T;
        using pointer = T *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator = slist_iterator<T, T &, T *>;
        using const_iterator = slist_iterator<T, const T &, const T *>;

    private:
        using list_node = slist_node<T>;
        using list_node_base = slist_node_base;
        using iterator_base = slist_iterator_base;
        using list_node_allocator = allocator<list_node>;

    private:
        list_node_base head;

        // 辅助函数(内存管理)
        static list_node *create_node(const value_type &value);
        static void destroy_node(list_node *node);

        // 辅助函数(修改容器)
        list_node *insert_aux(list_node_base *pos, const value_type &x);
        void clear();

    public:
        // 构造, 复制, 析构相关函数
        slist() { head.next = nullptr; }
        ~slist() { clear(); }

        // 迭代器相关
        iterator begin() { return iterator(static_cast<list_node *>(head.next)); }
        iterator end() { return iterator(nullptr); }
        const_iterator cbegin() const noexcept { return iterator(static_cast<list_node *>(head.next)); }
        const_iterator cend() const noexcept { return iterator(nullptr); }

        // 访问元素相关
        reference front() { return static_cast<list_node *>(head.next)->data; }
        const_reference front() const noexcept { return static_cast<list_node *>(head.next)->data; }

        // 修改容器相关
        void push_front(const value_type &value) { slist_make_link(&head, create_node(value)); }
        void pop_front();
        iterator insert_after(iterator pos, const value_type &x);
        iterator insert(iterator pos, const value_type &x);


        // 容量相关
        size_type size() const noexcept { return slist_size(head.next); }
        bool empty() const noexcept { return head.next == nullptr; }

        void swap(slist &rhs); // 两个slist互换, 只要将head互换即可
    };
}

#endif //MINISTL_STL_SLIST_H
