#ifndef MINISTL_STL_VECTOR_IMP_H
#define MINISTL_STL_VECTOR_IMP_H

#include <exception>
#include <algorithm>
#include "stl_vector.h"

namespace MiniSTL {
    // simple alias
//    template<class T, class Alloc>
//    using iterator_v = typename vector<T, Alloc>::iterator;

    /***********************辅助函数(private): 放到前面方便理解***********************/
    // ------内存管理-----
    template<class T, class Alloc>
    inline typename vector<T, Alloc>::iterator
    vector<T, Alloc>::allocate_and_fill(const size_type n, const value_type &value) {
        iterator result = data_allocator::allocate(n);
        MiniSTL::uninitialized_fill_n(result, n, value);
        return result;
    }

    template<class T, class Alloc>
    inline void vector<T, Alloc>::fill_initialize(size_type n, const value_type &value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    template<class T, class Alloc>
    template<class InputIterator>
    inline typename vector<T, Alloc>::iterator
    vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
        start = data_allocator::allocate(last - first);
        finish = uninitialized_copy(first, last, start);
        end_of_storage = finish;
        return start;
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::deallocate() noexcept {
        if (start) data_allocator::deallocate(start, end_of_storage - start);
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::destroy_and_deallocate() noexcept {
        data_allocator::destroy(start, finish);
        deallocate();
    }

    // -----插入辅助函数-----
    template<class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const size_type n, const value_type &value) {
        if (n) {
            if (static_cast<size_type>(end_of_storage - finish) >= n) { // needn't expand
                value_type value_copy = value;
                const size_type elems_after = finish - position;
                iterator old_finish = finish;
                if (elems_after > n) {
                    MiniSTL::uninitialized_copy(finish - n, finish, finish);
                    finish += n;
                    // copy_backward needs _SCL_SECURE_NO_WARNINGS
                    std::copy_backward(position, old_finish - n, old_finish); // TODO
                    std::fill(position, position + n, value_copy);  // TODO
                } else {
                    MiniSTL::uninitialized_fill_n(finish, n - elems_after, value_copy);
                    finish += n - elems_after;
                    MiniSTL::uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    std::fill(position, old_finish, value_copy); // TODO
                }
            } else { // expand
                const size_type old_size = size();
                const size_type new_size = old_size + (old_size > n ? old_size : n);
                iterator new_start = data_allocator::allocate(new_size);
                iterator new_finish = new_start;
                try {
                    new_finish = MiniSTL::uninitialized_copy(start, position, new_start);
                    new_finish = MiniSTL::uninitialized_fill_n(new_finish, n, value);
                    new_finish = MiniSTL::uninitialized_copy(position, finish, new_finish);
                } catch (std::exception &) {
                    MiniSTL::destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, new_size);
                    throw;
                }
                destroy_and_deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + new_size;
            }
        }
    }


    /***********************构造, 复制, 析构相关***********************/
    template<class T, class Alloc>
    inline vector<T, Alloc>::vector(const size_type n) {
        fill_initialize(n, value_type());
    }

    template<class T, class Alloc>
    inline vector<T, Alloc>::vector(size_type n, const value_type &value) {
        fill_initialize(n, value);
    }

    template<class T, class Alloc>
    inline vector<T, Alloc>::vector(int n, const value_type &value) {
        fill_initialize(n, value);
    }

    template<class T, class Alloc>
    template<class InputIterator>
    inline vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
        allocate_and_copy(first, last);
    }

    template<class T, class Alloc>
    inline vector<T, Alloc>::vector(std::initializer_list<T> il) {
        start = allocate_and_copy(il.begin(), il.end());
        finish = end_of_storage = start + (il.end() - il.begin());
    }

    template<class T, class Alloc>
    inline vector<T, Alloc>::vector(const vector &v) {
        allocate_and_copy(v.start, v.finish);
    }

    template<class T, class Alloc>
    inline vector<T, Alloc>::vector(vector &&v) {
        start = v.start;
        finish = v.finish;
        end_of_storage = v.end_of_storage;
        v.start = v.finish = v.end_of_storage = nullptr;
    }

    template<class T, class Alloc>
    inline vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &rhs) {
        vector temp(rhs);
        swap(temp);
        return *this;
    }

    template<class T, class Alloc>
    inline vector<T, Alloc> &vector<T, Alloc>::operator=(vector &&rhs) noexcept {
        if (this != &rhs) {
            destroy_and_deallocate();
            start = rhs.start;
            finish = rhs.finish;
            end_of_storage = rhs.end_of_storage;
            rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
        }
        return *this;
    }

    template<class T, class Alloc>
    inline vector<T, Alloc> &vector<T, Alloc>::operator=(std::initializer_list<T> il) {
        start = allocate_and_copy(il.begin(), il.end());
        finish = end_of_storage = start + (il.end() - il.begin());
        return *this;
    }

    template<class T, class Alloc>
    inline vector<T, Alloc>::~vector() noexcept {
        data_allocator::destroy(start, finish);
        deallocate();
    }

    /***********************比较操作相关***********************/
    template<class T, class Alloc>
    inline bool vector<T, Alloc>::operator==(const vector &v) const {
        if (size() != v.size()) {
            return false;
        } else {
            auto ptr1 = start;
            auto ptr2 = v.start;
            for (; ptr1 != finish; ++ptr1, ++ptr2) {
                if (*ptr1 != *ptr2)
                    return false;
            }
            return true;
        }
    }

    template<class T, class Alloc>
    inline bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
        return lhs.operator==(rhs);
    }

    template<class T, class Alloc>
    inline bool vector<T, Alloc>::operator!=(const vector &v) const {
        return !(*this == v);
    }

    template<class T, class Alloc>
    inline bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
        return !(lhs == rhs);
    }

    /***********************迭代器相关***********************/
    // 在stl_vector.h中

    /***********************访问元素相关***********************/
    // 在stl_vector.h中

    /***********************修改容器相关***********************/
    // insert
    template<class T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, const size_type n, const value_type &value) {
        insert_aux(position, n, value);
    }

    template<class T, class Alloc>
    inline typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type &value) {
        difference_type diff = position - begin();
        insert_aux(position, 1, value);
        return begin() + diff;
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        size_type n = last - first;
        if (static_cast<size_type >(end_of_storage - finish) >= n) {
            if (finish - position > n) {
                uninitialized_copy(finish - n, finish, finish);
                std::copy_backward(position, finish - n, finish);
                std::copy(first, last, position);
            } else {
                uninitialized_copy(position, finish, position + n);
                uninitialized_copy(first, last, position);
            }
            finish += n;
        } else {
            const size_type old_size = size();
            const size_type new_size = old_size + (old_size > n ? old_size : n);
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(start, position, new_start);
                new_finish = uninitialized_copy(first, last, new_finish);
                new_finish = uninitialized_copy(position, finish, new_finish);
            } catch (std::exception &) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
            }
            destroy_and_deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }

    // erase
    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
        iterator i = std::copy(last, finish, first);   // TODO
        destroy(i, finish);
        finish -= (last - first);
        return first;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
        return erase(position, position + 1);
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::clear() {
        erase(start, finish);
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const value_type &value) {
        if (finish != end_of_storage) {
            construct(finish, value);
            ++finish;
        } else
            insert_aux(end(), 1, value);
    }

    template<class T, class Alloc>
    inline void vector<T, Alloc>::pop_back() {
        --finish;
        destroy(finish);
    }

    template<class T, class Alloc>
    inline void vector<T, Alloc>::swap(vector &rhs) {
        std::swap(start, rhs.start);
        std::swap(finish, rhs.finish);
        std::swap(end_of_storage, rhs.end_of_storage);
    }

    /***********************与容量相关***********************/
    template<class T, class Alloc>
    inline void vector<T, Alloc>::resize(size_type new_size, const value_type &value) {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert_aux(end(), new_size - size(), value);
    }

    template<class T, class Alloc>
    inline void vector<T, Alloc>::reserve(size_type n) {
        if (n <= capacity())
            return;
        T *newStart = data_allocator::allocate(n);
        T *newFinish = MiniSTL::uninitialized_copy(begin(), end(), newStart);
        destroy_and_deallocate();

        start = newStart;
        finish = newFinish;
        end_of_storage = start + n;
    }

    // 将capacity缩小到size大小 (注: 其中的temp会通过析构函数释放掉)
    template<class T, class Alloc>
    inline void vector<T, Alloc>::shrink_to_fit() {
        vector temp(*this);
        swap(temp);
    }

}

#endif //MINISTL_STL_VECTOR_IMP_H
