#ifndef MINISTL_STL_DEQUE_IMP_H
#define MINISTL_STL_DEQUE_IMP_H

#include <iostream>   // TODO: for test
#include <algorithm>
#include <exception>
#include "stl_deque.h"

namespace MiniSTL {
    /***********************辅助函数(private): 放到前面方便理解***********************/
    // 内存相关
    template<class T, class Alloc, size_t BufSize>
    inline typename deque<T, Alloc, BufSize>::value_type *deque<T, Alloc, BufSize>::allocate_node() {
        return data_allocator::allocate(buffer_size);
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::deallocate_node(value_type *p) {
        data_allocator::deallocate(p, buffer_size);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type n) {
        // 所需节点数 (整除则多配置一个)
        size_type num_nodes = n / buffer_size + 1;
        // 一个map至少管理8个节点, 至多管理num_nodes+2个
        map_size = max(initial_map_size, num_nodes + 2);
        map = map_allocator::allocate(map_size);
        // 令nstart与nfinish指向map所拥有的全部node的中部,以便日后扩充头尾
        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;

        map_pointer cur;
        try {  // 为每一个节点配置空间
            for (cur = nstart; cur <= nfinish; ++cur)
                *cur = allocate_node();
        } catch (std::exception &) {
            clear();
        }
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + n % buffer_size; // 若n%buffer_size==0,会多配置一个节点, 此时cur指向该节点头部
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::fill_initialized(size_type n, const value_type &value) {
        create_map_and_nodes(n);
        map_pointer cur;
        try {
            // 为每个缓冲区设定初值
            for (cur = start.node; cur < finish.node; ++cur)
                uninitialized_fill(*cur, *cur + buffer_size, value);
            // 最后一个缓冲区只设定至需要处
            uninitialized_fill(finish.first, finish.cur, value);
        } catch (std::exception &) {
            clear();
        }
    }

    // map大小调整辅助函数
    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_nstart;
        if (map_size > 2 * new_num_nodes) {  // map够大, 只是"不均匀"
            // 某一端出现失衡, 因此释放存储区完成重新中央分配，
            // 规定新的nstart, 若添加在前端则向后多移动n个单位
            new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            if (new_nstart < start.node)     // 整体前移
                std::copy(start.node, finish.node + 1, new_nstart);
            else                             // 整体后移
                std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
        } else {
            size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
            // 分配新空间
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            // 拷贝原有内容
            std::copy(start.node, finish.node + 1, new_nstart);
            // 释放原map
            map_allocator::deallocate(map, map_size);
            // 重新设定map
            map = new_map;
            map_size = new_map_size;
        }

        //设定start与finish
        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num_nodes - 1);  // 注意并非new_num,接下来的设定转交其他函数处理
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::reserve_map_at_back(size_type nodes_to_add) {
        // map_size-(finish.node-map+1) == 后端剩余node个数
        if (nodes_to_add > map_size - (finish.node - map + 1))
            reallocate_map(nodes_to_add, false);
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::reserve_map_at_front(size_type nodes_to_add) {
        // start.node-map == 前端剩余node个数
        if (nodes_to_add > start.node - map)
            reallocate_map(nodes_to_add, true);
    }

    // 前后增添一定数目元素
    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::new_elements_at_back(size_type n) {
        size_type new_nodes = (n + buffer_size - 1) / buffer_size;
        reserve_map_at_back(new_nodes);
        size_type i;
        try {
            for (i = 1; i <= new_nodes; ++i)
                *(finish.node + i) = allocate_node();
        } catch (std::exception) {
            for (auto j = 1; j < i; ++j)
                deallocate_node(*(finish.node + j));
        }
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::new_elements_at_front(size_type n) {
        size_type new_nodes = (n + buffer_size - 1) / buffer_size;
        reserve_map_at_front(new_nodes);
        size_type i;
        try {
            for (i = 1; i <= new_nodes; ++i)
                *(start.node - i) = allocate_node();
        } catch (std::exception) {
            for (auto j = 1; j < i; ++j)
                deallocate_node(*(start.node - j));
        }
    }

    template<class T, class Alloc, size_t BufSize>
    inline typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::reserve_elements_at_back(size_type n) {
        size_type vacanicies = finish.last - finish.cur - 1;
        if (n > vacanicies)
            new_elements_at_back(n - vacanicies);
        return finish + n;
    }

    template<class T, class Alloc, size_t BufSize>
    inline typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::reserve_elements_at_front(size_type n) {
        size_type vacanicies = start.cur - start.first;
        if (n > vacanicies)
            new_elements_at_front(n - vacanicies);
        return start - n;
    }

    // push&pop辅助
    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::push_back_aux(const value_type &value) {
        value_type value_copy = value;
        reserve_map_at_back();                  // 若符合某条件则重新更换map
        *(finish.node + 1) = allocate_node();   // 配置新节点
        try {
            construct(finish.cur, value_copy);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;          // 更新finish.cur为当前first
        } catch (std::exception &) {
            deallocate_node(*(finish.node + 1));
        }
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::push_front_aux(const value_type &value) {
        value_type value_copy = value;
        reserve_map_at_front();               // 若符合某条件则重新更换map
        *(start.node - 1) = allocate_node();  // 配置新节点
        try {
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, value_copy);
        } catch (std::exception &) {
            start.set_node(start.node + 1);
            start.cur = start.first;
            deallocate_node(*(start.node - 1));
        }
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::pop_back_aux() {
        data_allocator::deallocate(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destroy(finish.cur);
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::pop_front_aux() {
        destroy(start.cur);
        data_allocator::deallocate(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type &value) {
        difference_type index = pos - start;       // 插入点之前的元素个数
        value_type value_copy = value;
        if (index < size() / 2) {                  // 前移更高效
            push_front(front());                   // 在最前端加入与第一元素同值的元素(注意start发生了改变)
            iterator front1 = start + 1;           // 待复制点
            iterator front2 = front1 + 1;          // 复制起始点
            iterator pos1 = front1 + index;        // 复制终止点（可否以pos直接代替pos1？）
            std::copy(front2, pos1, front1);            // 移动元素
        } else {                                   // 过程类似上面
            push_back(back());
            iterator back1 = finish - 1;;
            iterator back2 = back1 - 1;
            pos = start + index;
            std::copy_backward(pos, back2, back1);
        }
        *pos = value_copy;
        return pos;
    }

    template<class T, class Alloc, size_t BufSize>
    template<class ForwardIterator>
    void deque<T, Alloc, BufSize>::insert_aux(iterator pos, const ForwardIterator &first,
                                              const ForwardIterator &last, size_type n) {
        difference_type elem_before = pos - start;
        size_type l = size();
        if (elem_before < l / 2) {
            iterator new_start = reserve_elements_at_front(n);
            iterator old_start = start;
            pos = start + elem_before;
            if (elem_before >= n) {
                iterator start_n = start + n;
                uninitialized_copy(start, start_n, new_start);
                start = new_start;
                std::copy(start_n, pos, old_start);
                std::copy(first, last, pos - static_cast<difference_type>(n));
            } else {
                ForwardIterator mid = first;
                advance(mid, n - elem_before);
                uninitialized_copy_copy(start, pos, first, mid, new_start);
                start = new_start;
                std::copy(mid, last, old_start);
            }
        } else {
            iterator new_finish = reserve_elements_at_back(n);
            iterator old_finish = finish;
            difference_type elem_after = l - elem_before;
            pos = finish - elem_after;
            if (elem_after > n) {
                iterator finish_n = finish_n - n;
                uninitialized_copy(finish_n, finish, finish);
                finish = new_finish;
                std::copy_backward(pos, finish_n, old_finish);
                std::copy(first, last, pos);
            } else {
                iterator mid = first + elem_after;
                uninitialized_copy_copy(mid, last, pos, finish, finish);
                finish = new_finish;
                std::copy(first, mid, pos);
            }
        }
    }

    // 交换操作
    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::swap(deque &x) {
        std::swap(start, x.start);
        std::swap(finish, x.finish);
        std::swap(map, x.map);
        std::swap(map_size, x.map_size);
    }

    /***********************构造, 复制, 析构相关***********************/
    // 构造函数
    template<class T, class Alloc, size_t BufSize>
    template<class InputIterator>
    inline deque<T, Alloc, BufSize>::deque(InputIterator first, InputIterator last) {
        if constexpr (_is_same<iterator_category_t<InputIterator>, input_iterator_tag>::value) {
            for (; first != last; ++first) {
                push_back(*first);
            }
        } else {
            size_type n = distance(first, last);
            create_map_and_nodes(n);
            map_pointer cur_node;
            for (cur_node = start.node; cur_node < finish.node; ++cur_node) {
                InputIterator mid = first;
                advance(mid, buffer_size);
                uninitialized_copy(first, mid, *cur_node);
                first = mid;
            }
            uninitialized_copy(first, last, *cur_node);
        }
    }

    template<class T, class Alloc, size_t BufSize>
    inline deque<T, Alloc, BufSize>::deque(std::initializer_list<T> il) {
        create_map_and_nodes(il.size());
        std::copy(il.begin(), il.end(), start);
    }

    template<class T, class Alloc, size_t BufSize>
    inline deque<T, Alloc, BufSize>::deque(const deque &x) {
        create_map_and_nodes(x.size());
        uninitialized_copy(x.start, x.finish, start);
    }

    // TODO: find a better method
    template<class T, class Alloc, size_t BufSize>
    inline deque<T, Alloc, BufSize>::deque(deque &&x) {
        start = x.start;
        finish = x.finish;
        map = x.map;
        map_size = x.map_size;
        x.start = iterator();
        x.finish = iterator();
        x.map = nullptr;
    }

    template<class T, class Alloc, size_t BufSize>
    inline deque<T, Alloc, BufSize> &deque<T, Alloc, BufSize>::operator=(const deque &x) {
        deque tmp(x);
        swap(tmp);
        return *this;
    }

    template<class T, class Alloc, size_t BufSize>
    inline deque<T, Alloc, BufSize> &deque<T, Alloc, BufSize>::operator=(deque &&x) {
        if (this != &x) {
            swap(x);
            x.start = iterator();
            x.finish = iterator();
            x.map = nullptr;
        }
        return *this;
    }

    template<class T, class Alloc, size_t BufSize>
    inline deque<T, Alloc, BufSize> &deque<T, Alloc, BufSize>::operator=(std::initializer_list<T> il) {
        create_map_and_nodes(il.size());
        std::copy(il.begin(), il.end(), start);
        return *this;
    }

    template<class T, class Alloc, size_t BufSize>
    inline deque<T, Alloc, BufSize>::~deque() {
        destroy(start, finish);
        for (auto temp = start.node; temp != finish.node; ++temp) deallocate_node(*temp);
        if (finish.node) deallocate_node(*finish.node);
        if (map) map_allocator::deallocate(map, map_size);
    }

    /****************************比较操作相关*************************/
    // TODO

    /****************************迭代器相关*************************/
    // 在stl_deque.h中

    /****************************访问元素相关*************************/
    // 在stl_deque.h中

    /****************************修改容器相关*************************/
    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::push_back(const value_type &value) {
        // finish的cur指向最后一个元素的下一个位置, 因此if语句表征至少还有一个备用空间
        if (finish.cur != finish.last - 1) {
            construct(finish.cur, value);
            ++finish.cur;
        } else  // 最终缓冲区已无或仅剩一个空间
            push_back_aux(value);
    }

    template<class T, class Alloc, size_t BufSiz>
    inline void deque<T, Alloc, BufSiz>::push_front(const value_type &value) {
        if (start.cur != start.first) {
            construct(start.cur - 1, value);
            --start.cur;
        } else
            push_front_aux(value);
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::pop_back() {
        if (finish.cur != finish.first) {  // 缓冲区至少存在一个元素
            --finish.cur;
            destroy(finish.cur);
        } else
            pop_back_aux();
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::pop_front() {
        if (start.cur != start.last - 1) {
            destroy(start.cur);
            ++start.cur;
        } else
            pop_front_aux();
    }

    template<class T, class Alloc, size_t BufSize>
    inline void deque<T, Alloc, BufSize>::clear() {
        // 最终要保留一个缓冲区, 这是deque的策略, 也是deque的初始状态
        for (map_pointer node = start.node + 1; node < finish.node; ++node) {  // 内部均存有元素
            destroy(*node, *node + buffer_size);      //析构所有元素
            data_allocator::deallocate(*node, buffer_size);
        }
        if (start.node != finish.node) {    // 存在头尾两个缓冲区
            // 析构其中所有元素
            destroy(start.cur, start.last);
            destroy(finish.first, finish.cur);
            // 保留头部, 释放尾部
            data_allocator::deallocate(finish.first, buffer_size);
        } else
            destroy(start.cur, finish.cur);   // 利用finish.cur标记末尾
        finish = start;   // 调整状态
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::erase(iterator pos) {
        iterator next = pos + 1;
        difference_type index = pos - start;     // 清除点前的元素个数
        if (index < (size() / 2)) {              // 后移开销较低
            std::copy_backward(start, pos, next);
            pop_front();
        } else {                                 // 前移开销较低
            std::copy(next, finish, pos);
            pop_back();
        }
        return start + index;
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::erase(iterator first, iterator last) {
        if (first == start && last == finish) {
            clear();
            return finish;
        } else {
            difference_type n = last - first;               // 清除区间长度
            difference_type elems_before = first - start;   // 前方元素个数
            if (elems_before < (size() - n) / 2) {          // 后移开销较低
                std::copy_backward(start, first, last);
                iterator new_start = start + n;             // 标记新起点
                destroy(start, new_start);                  // 析构多余元素
                // 释放多余缓冲区
                for (map_pointer cur = start.node; cur < new_start.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size);
                start = new_start;
            } else {                                        // 前移开销较低
                std::copy(last, finish, first);
                iterator new_finish = finish - n;           // 标记末尾
                destroy(new_finish, finish);
                // 释放多余缓冲区
                for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size);
                finish = new_finish;
            }
            return start + elems_before;
        }
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::insert(iterator pos, const value_type &value) {
        if (pos.cur == start.cur) {
            push_front(value);
            return start;
        } else if (pos.cur == finish.cur) {
            push_back(value);
            iterator temp = finish - 1;
            return temp;
        } else
            return insert_aux(pos, value);
    }

    template<class T, class Alloc, size_t BufSize>
    template<class InputIterator>
    void deque<T, Alloc, BufSize>::insert(iterator pos, const InputIterator &first, const InputIterator &last) {
        size_type n = last - first;
        if (pos.cur == start.cur) {
            iterator new_start = reserve_elements_at_front(n);
            uninitialized_copy(first, last, new_start);
        } else if (pos.cur == finish.cur) {
            iterator new_finish = reserve_elements_at_back(n);
            uninitialized_copy(first, last, finish);
            finish = new_finish;
        } else {
            insert_aux(pos, first, last, n);
        }
    }
}

#endif //MINISTL_STL_DEQUE_IMP_H
