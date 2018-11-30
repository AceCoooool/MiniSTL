#ifndef MINISTL_STL_LIST_H
#define MINISTL_STL_LIST_H

#include <initializer_list>
#include "../../Allocator/allocator.h"
#include "../../Iterator/iterator_adapters.h"
#include "stl_list_iterator.h"

namespace MiniSTL {
    template<class T, class Alloc=allocator<list_node<T>>>
    class list {
    public:    // alias declarations
        using iterator = list_iterator<T, T &, T *>;
        using const_iterator = list_iterator<T, const T &, const T *>;
        using reverse_iter = reverse_iterator<iterator>;
        using const_reverse_iter = reverse_iterator<const_iterator>;
        using value_type = typename list_iterator<T, T &, T *>::value_type;
        using pointer = typename list_iterator<T, T &, T *>::pointer;
        using reference = typename list_iterator<T, T &, T *>::reference;
        using link_type = typename list_iterator<T, T &, T *>::link_type;
        using size_type = typename list_iterator<T, T &, T *>::size_type;
        using difference_type = typename list_iterator<T, T &, T *>::difference_type;

    private:
        link_type node;   // only data member(tail)

        // 内存管理
        using list_node_allocator = allocator<list_node<T>>;
        link_type get_node() { return list_node_allocator::allocate(); }
        void put_node(link_type p) { list_node_allocator::deallocate(p); }
        link_type create_node(const T &value);
        void destroy_node(link_type p);

        // 辅助函数
        void empty_initialized();
        // Move [first, last) before pos
        void transfer(iterator position, iterator first, iterator last);
        void swap(list &) noexcept;

    public:
        // 构造, 复制, 析构相关函数
        list() { empty_initialized(); }
        explicit list(size_type n, const value_type &value = value_type());
        list(std::initializer_list<T> il);
        list(const list &);
        list(list &&) noexcept;
        list &operator=(const list &) noexcept;
        list &operator=(list &&) noexcept;
        ~list();

        // 比较操作相关
        template <class T1, class Alloc1>
        friend bool operator==(const list<T1, Alloc1> &lhs, const list<T1, Alloc1> &rhs);
        template <class T1, class Alloc1>
        friend bool operator!=(const list<T1, Alloc1> &lhs, const list<T1, Alloc1> &rhs);

        // 迭代器相关
        iterator begin() noexcept { return static_cast<link_type>(node->next); }
        const_iterator cbegin() const noexcept { return static_cast<link_type>(node->next); }
        iterator end() noexcept { return node; }
        const_iterator cend() const noexcept { return node; }
        reverse_iter rbegin() noexcept { return reverse_iter(end()); }
        const_reverse_iter crbegin() const noexcept { return const_reverse_iter(cend()); }
        reverse_iter rend() noexcept { return reverse_iter(begin()); }
        const_reverse_iter crend() const noexcept { return const_reverse_iter(cbegin()); }

        // 访问元素相关
        reference front() noexcept { return *begin(); }
        reference back() noexcept { return *(--end()); }

        // 修改容器相关的操作
        iterator insert(iterator, const value_type &);
        void insert(iterator, size_type, const value_type &);
        template<class InputIterator>
        void insert(iterator, InputIterator, InputIterator);
        iterator erase(iterator);
        iterator erase(iterator, iterator);
        void push_front(const T &value) { insert(begin(), value); }
        void push_back(const T &value) { insert(end(), value); }
        void pop_fornt() { erase(begin()); }
        void pop_back();

        void clear();             // 清楚所有节点.(但最终是empty_initialized的状态)
        void unique();            // 消除相邻重复的节点
        void remove(const T &v);  // 删除链表中等于v的节点
        void merge(list &x);      // 将当前链表和链表x进行合并(两者均有序)
        void reverse();           // 反转链表
        void sort();              // 排序
        void splice(iterator pos, list &x);             // 将链表x加到当前链表的pos位置
        void splice(iterator pos, list &, iterator i);  // 将i加到当前链表的pos位置

        // 与容量相关
        bool empty() const noexcept { return node->next == node; }
        size_type size() const noexcept { return distance(cbegin(), cend()); }

    };
}

#endif //MINISTL_STL_LIST_H
