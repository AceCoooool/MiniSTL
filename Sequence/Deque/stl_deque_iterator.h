#ifndef MINISTL_STL_DEQUE_ITERATOR_H
#define MINISTL_STL_DEQUE_ITERATOR_H

#include "../../Iterator/stl_iterator.h"

namespace MiniSTL {

    inline size_t deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }

    template<class T, class Ref, class Ptr, size_t BufSize = 0>
    struct deque_iterator {
        // alias declarartions
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using pointer = Ptr;
        using reference = Ref;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using iterator = deque_iterator<T, T &, T *, BufSize>;
        using const_iterator = deque_iterator<T, const T &, const T *, BufSize>;

        using map_pointer = T **;
        using self = deque_iterator<T, Ref, Ptr, BufSize>;

        // 保持与容器的连接
        value_type *cur;     // 当前缓冲区的当前元素
        value_type *first;   // 当前缓冲区头
        value_type *last;    // 当前缓冲区尾(含备用空间）
        map_pointer node;    // 指向管控中心

        difference_type buffer_size = deque_buf_size(BufSize, sizeof(value_type));

        // 构造, 复制, 析构相关函数
        deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
        deque_iterator(pointer x, map_pointer y) : cur(x), first(*y), last(*y + buffer_size), node(y) {}
        deque_iterator(const iterator &rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}
//        deque_iterator(iterator &&rhs);
        self &operator=(const iterator &deq_iter);

        // 辅助函数(跳转缓冲区操作)
        void set_node(map_pointer new_node);

        // 重载运算子(解引用, 加减操作)
        reference operator*() const { return *cur; }
        pointer operator->() const { return &(operator*()); }

        self &operator++();
        self operator++(int);
        self &operator--();
        self operator--(int);
        self &operator+=(difference_type n);
        self &operator-=(difference_type n) { return *this += -n; }
        self operator+(difference_type n);
        self operator-(difference_type n);
        difference_type operator-(const self &rhs) const;

        reference operator[](difference_type n) { return *(*this + n); }

        bool operator==(const self &rhs) { return cur == rhs.cur; }
        bool operator!=(const self &rhs) { return !(*this == rhs); }
        bool operator<(const self &rhs) { return node == rhs.node ? cur < rhs.cur : node < rhs.node; }

        // friend operator
        template<class T1, class Ref1, class Ptr1>
        friend deque_iterator<T1, Ref1, Ptr1> operator+(const deque_iterator<T1, Ref1, Ptr1> &it1, difference_type n);
        template<class T1, class Ref1, class Ptr1>
        friend deque_iterator<T1, Ref1, Ptr1> operator-(const deque_iterator<T1, Ref1, Ptr1> &it1, difference_type n);

        template<class T1, class Ref1, class Ptr1>
        friend difference_type operator-(const deque_iterator<T1, Ref1, Ptr1> &it1,
                                         const deque_iterator<T1, Ref1, Ptr1> &it2);
        template<class T1, class Ref1, class Ptr1>
        friend bool operator==(const deque_iterator<T1, Ref1, Ptr1> &it1, const deque_iterator<T1, Ref1, Ptr1> &it2);
        template<class T1, class Ref1, class Ptr1>
        friend bool operator!=(const deque_iterator<T1, Ref1, Ptr1> &it1, const deque_iterator<T1, Ref1, Ptr1> &it2);

    };

    // 二元友元操作
    template<class T, class Ref, class Ptr>
    inline deque_iterator<T, Ref, Ptr> operator+(const deque_iterator<T, Ref, Ptr> &it, ptrdiff_t n) {
        return it + n;
    }

    template<class T, class Ref, class Ptr>
    inline deque_iterator<T, Ref, Ptr> operator-(const deque_iterator<T, Ref, Ptr> &it, ptrdiff_t n) {
        return it + (-n);
    }

    template<class T, class Ref, class Ptr>
    inline typename deque_iterator<T, Ref, Ptr>::difference_type operator-(const deque_iterator<T, Ref, Ptr> &it1,
                                                                           const deque_iterator<T, Ref, Ptr> &it2) {
        return it1.operator-(it2);
    }

    template<class T, class Ref, class Ptr>
    inline bool operator==(const deque_iterator<T, Ref, Ptr> &it1, const deque_iterator<T, Ref, Ptr> &it2) {
        return const_cast<deque_iterator<T, Ref, Ptr> &>(it1).operator==(it2);
    }

    template<class T, class Ref, class Ptr>
    inline bool operator!=(const deque_iterator<T, Ref, Ptr> &it1, const deque_iterator<T, Ref, Ptr> &it2) {
        return const_cast<deque_iterator<T, Ref, Ptr> &>(it1).operator!=(it2);
    }
}

#endif //MINISTL_STL_DEQUE_ITERATOR_H
