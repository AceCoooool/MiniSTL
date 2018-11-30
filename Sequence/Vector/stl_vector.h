#ifndef MINISTL_STL_VECTOR_H
#define MINISTL_STL_VECTOR_H

#include <initializer_list>
#include "../../Allocator/allocator.h"
#include "../../Iterator/iterator_adapters.h"
#include "../../Allocator/stl_uninitialized.h"

namespace MiniSTL {
    /**
     * vector
     */
    template<class T, class Alloc = allocator<T>>
    class vector {
    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using const_reference = const T &;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using iterator = T *;
        using const_iterator = const T *;
        using const_reverse_iter = reverse_iterator<const T *>;
        using reverse_iter = reverse_iterator<T *>;

    private:
        iterator start;
        iterator finish;
        iterator end_of_storage;

        using data_allocator = Alloc;

    public:
        // 构造, 复制, 析构相关函数
        vector() : start(0), finish(0), end_of_storage(0) {}
        explicit vector(const size_type n);
        vector(size_type n, const value_type &value);
        vector(int n, const value_type &value);    // deal with vector<int>(10, 2)
        template<class InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(std::initializer_list<T> il);
        vector(const vector &v);
        vector(vector &&v);
        vector &operator=(const vector &v);
        vector &operator=(vector &&v) noexcept;
        vector &operator=(std::initializer_list<T> il);
        ~vector() noexcept;

        // 比较操作相关
        bool operator==(const vector &v) const;
        bool operator!=(const vector &v) const;

        template<class T1, class Alloc1>
        friend bool operator==(const vector<T1, Alloc1> &v1, const vector<T1, Alloc1> &v2);
        template<class T1, class Alloc1>
        friend bool operator!=(const vector<T1, Alloc1> &v1, const vector<T1, Alloc1> &v2);

        // 迭代器相关
        iterator begin() { return (start); }
        const_iterator begin() const { return (start); }
        const_iterator cbegin() const { return (start); }
        iterator end() { return (finish); }
        const_iterator end() const { return (finish); }
        const_iterator cend() const { return (finish); }
        reverse_iter rbegin() { return reverse_iterator(finish); }
        const_reverse_iter crbegin() const { return const_reverse_iterator(finish); }
        reverse_iter rend() { return reverse_iterator(start); }
        const_reverse_iter crend() const { return const_reverse_iterator(start); }

        // 访问元素相关
        reference operator[](const difference_type i) { return *(begin() + i); }
        const_reference operator[](const difference_type i) const { return *(cbegin() + i); }
        reference front() { return *(begin()); }
        const_reference front() const { return *(begin()); }
        reference back() { return *(end() - 1); }
        pointer data() { return start; }

        // 修改容器相关的操作
        iterator insert(iterator position, const value_type &val);
        void insert(iterator position, const size_type n, const value_type &val);
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void clear();
        void push_back(const value_type &value);
        void pop_back();
        void swap(vector &v);

        // 与容量相关
        difference_type size() const { return finish - start; }
        difference_type capacity() const { return end_of_storage - start; }
        bool empty() const { return start == finish; }
        void resize(size_type n, const value_type &val = value_type());
        void reserve(size_type n);
        void shrink_to_fit();

    private:
        // 内存管理
        iterator allocate_and_fill(const size_type n, const value_type &value);
        void fill_initialize(size_type n, const value_type &value);
        template<class InputIterator>
        iterator allocate_and_copy(InputIterator first, InputIterator last);
        void deallocate() noexcept;
        void destroy_and_deallocate() noexcept;

        // 插入辅助函数
        void insert_aux(iterator position, const size_type n, const value_type &value);

    }; // end of class vector

}

//#include "stl_vector.imp.h"

#endif //MINISTL_STL_VECTOR_H
