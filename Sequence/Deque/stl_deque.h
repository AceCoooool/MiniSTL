#ifndef MINISTL_STL_DEQUE_H
#define MINISTL_STL_DEQUE_H

#include "../../Allocator/allocator.h"
#include "../../Iterator/iterator_adapters.h"
#include "stl_deque_iterator.imp.h"

namespace MiniSTL {
    template<class T, class Alloc = allocator<T>, size_t BufSize = 0>
    class deque {
    public:
        // alias declarations
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using const_reference = const T &;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using iterator = typename deque_iterator<T, T &, T *, BufSize>::iterator;
        using reverse_iter = reverse_iterator<iterator>;
        using const_iterator = typename deque_iterator<T, T &, T *, BufSize>::const_iterator;
        using const_reverse_iter = reverse_iterator<const_iterator>;

    private:
        // Internal alias declarations
        using map_pointer = pointer *;
        using data_allocator = allocator<value_type>;
        using map_allocator = allocator<pointer>;

        // data member
        iterator start;      // 第一个节点
        iterator finish;     // 最后一个节点
        map_pointer map;     // 指向节点的指针
        size_type map_size;  // map的大小

        // 辅助函数(内存相关)
        size_type initial_map_size = 8U;
        size_type buffer_size = deque_buf_size(BufSize, sizeof(value_type));;
        value_type *allocate_node();
        void deallocate_node(value_type *p);
        void create_map_and_nodes(size_type);
        void fill_initialized(size_type, const value_type &value = value_type());
        // 辅助函数(调整map大小)
        void reallocate_map(size_type, bool);
        void reserve_map_at_back(size_type nodes_to_add = 1);
        void reserve_map_at_front(size_type nodes_to_add = 1);
        // 辅助函数(前后增添个数---调整元素数目)
        void new_elements_at_back(size_type n);
        void new_elements_at_front(size_type n);
        iterator reserve_elements_at_back(size_type n);
        iterator reserve_elements_at_front(size_type n);
        // 辅助函数(push&pop相关)
        void push_back_aux(const value_type &);
        void push_front_aux(const value_type &);
        void pop_back_aux();
        void pop_front_aux();
        iterator insert_aux(iterator, const value_type &);
        template<class ForwardIterator>
        void insert_aux(iterator pos, const ForwardIterator &first, const ForwardIterator &last, size_type n);

        void swap(deque &x);

    public:
        // 构造, 复制, 析构相关函数
        deque() : start(), finish(), map(nullptr), map_size(0) { fill_initialized(0, value_type()); }
        explicit deque(size_type n, const value_type &value = value_type())
                : start(), finish(), map(nullptr), map_size(0) { fill_initialized(n, value); }
        deque(int n, int value) : start(), finish(), map(nullptr), map_size(0) { fill_initialized(n, value); }
        template<class InputIterator>
        deque(InputIterator first, InputIterator last);
        deque(std::initializer_list<T> il);
        deque(const deque &x);
        deque(deque &&x);

        deque &operator=(const deque &x);
        deque &operator=(deque &&x);
        deque &operator=(std::initializer_list<T> il);

        ~deque();

        // 比较操作相关
        template<class T1, class Alloc1>
        friend bool operator==(const deque<T1, Alloc1> &, const deque<T1, Alloc1> &);
        template<class T1, class Alloc1>
        friend bool operator!=(const deque<T1, Alloc1> &, const deque<T1, Alloc1> &);

        // 迭代器相关
        iterator begin() const { return start; }
        iterator end() const { return finish; }
        reverse_iter rbegin() { return reverse_iter(finish); }
        reverse_iter rend() { return reverse_iter(start); }
        const_iterator cbegin() const noexcept { return start; }
        const_iterator cend() const noexcept { return finish; }
        const_reverse_iter crbegin() const noexcept { return const_reverse_iter(finish); }
        const_reverse_iter crend() const noexcept { return const_reverse_iter(start); }

        // 访问元素相关
        reference operator[](size_type n) { return start[n]; }
        const_reference operator[](size_type n) const { return start[n]; }
        reference front() { return *start; }
        reference back() { return *(finish - 1); }

        // 修改容器相关的操作
        void push_back(const value_type &);
        void push_front(const value_type &);
        void pop_back();
        void pop_front();
        void clear();
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        iterator insert(iterator pos, const value_type &value);
        template<class InputIterator>
        void insert(iterator pos, const InputIterator &first, const InputIterator &last);

        // 与容量相关
        size_type size() const noexcept { return finish - start; }
        bool empty() const noexcept { return finish == start; }
    };

    template<class T, class Alloc>
    inline bool operator==(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs) {
        return (lhs.size() == rhs.size()) && (std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }

    template<class T, class Alloc>
    inline bool operator!=(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs) {
        return !(lhs.c == rhs.c);
    }
}

#endif //MINISTL_STL_DEQUE_H
