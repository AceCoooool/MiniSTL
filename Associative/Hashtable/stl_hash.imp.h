#ifndef MINISTL_STL_HASH_IMP_H
#define MINISTL_STL_HASH_IMP_H

#include "stl_hash.h"

namespace MiniSTL {
    /*-------------------------------------迭代器-----------------------------------*/
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator &
    hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
        const node *old = cur;
        cur = cur->next;
        if (!cur) {    // 当前链表为尾端, 此时选择下一个bucket节点
            size_type bucket = ht->bkt_num(old->val);
            while (!cur && ++bucket < ht->buckets.size())
                cur = ht->buckets[bucket];
        }
        return *this;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator &
    hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() {
        const node *old = cur;
        cur = cur->next;
        if (!cur) {    // 当前链表为尾端, 此时选择下一个bucket节点
            size_type bucket = ht->bkt_num(old->val);
            while (!cur && ++bucket < ht->buckets.size())
                cur = ht->buckets[bucket];
        }
        return *this;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
    hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) {
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    /*-------------------------------------哈希表-----------------------------------*/

    /**************************辅助函数**************************/
    // 1. 内存管理
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node *
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::new_node(const value_type &v) {
        node *n = get_node();
        n->next = nullptr;
        try {
            construct(&n->val, v);
            return n;
        } catch (...) {
            put_node(n);
            throw;
        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::delete_node(node *n) {
        destroy(&n->val);
        put_node(n);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    initialize_buckets(size_type n) {
        const size_type n_buckets = __stl_next_prime(n);
        // 保留空间, 由于此时vector's size==0, 因此等价于全部置为nullptr
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(), n_buckets, static_cast<node *>(nullptr));
        num_elements = 0;
    }

    // 2. 调整空间大小
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type new_elems) {
        const size_type old_n = buckets.size();
        if (new_elems > old_n) {
            const size_type n = __stl_next_prime(new_elems); // 找到下一个质数
            if (n > old_n) {
                vector<node *> tmp(n, nullptr);
                try {
                    for (auto bkt = 0; bkt < old_n; ++bkt) {
                        node *first = buckets[bkt];
                        while (first) {
                            auto new_bucket = bkt_num(first->val, n);
                            buckets[bkt] = first->next;
                            first->next = tmp[new_bucket];
                            tmp[new_bucket] = first;
                            first = buckets[bkt];
                        }
                    }
                    buckets.swap(tmp);   // 新旧对调
                } catch (std::exception) {
                    for (auto bkt = 0; bkt < tmp.size(); ++bkt) {
                        while (tmp[bkt]) {
                            node *next = tmp[bkt]->next;
                            delete_node(tmp[bkt]);
                            tmp[bkt] = next;
                        }
                    }
                }
            }
        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
        for (auto i = 0; i < buckets.size(); ++i) {
            auto cur = buckets[i];
            while (cur) {
                auto next = cur->next;
                delete_node(cur);
                cur = next;
            }
            buckets[i] = nullptr;
        }
        num_elements = 0;
    }

    // 3. 哈希表复制
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable &ht) {
        buckets.clear();
        buckets.reserve(ht.buckets.size());
        buckets.insert(buckets.end(), ht.buckets.size(), nullptr);
        try {
            for (auto i = 0; i < ht.buckets.size(); ++i) {
                const node *cur = ht.buckets[i];
                if (cur) {
                    auto __copy = new_node(cur->val);
                    buckets[i] = __copy;

                    for (auto next = cur->next; next; cur = next, next = cur->next) {
                        __copy->next = new_node(next->val);
                        __copy = __copy->next;
                    }
                }
            }
            num_elements = ht.num_elements;
        } catch (std::exception) {
            clear();
        }
    }

    // 4. 哈希映射关系
    // 通过键值找位置
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num_key(const key_type &key, size_t n) const {
        return hash(key) % n;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num_key(const key_type &key) const {
        return bkt_num_key(key, buckets.size());
    }
    // 通过值找位置(利用get_key找到值对应的键)
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num(const value_type &obj, size_t n) const {
        return bkt_num_key(get_key(obj), n);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num(const value_type &obj) const {
        return bkt_num_key(get_key(obj));
    }

    // 5. 插入辅助函数
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type &obj) {
        const size_type n = bkt_num(obj);
        node *first = buckets[n];

        for (node *cur = first; cur; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj)))  // 插入点存在的情况
                return std::pair<iterator, bool>(iterator(cur, this), false);
        }
        node *tmp = new_node(obj);   // 头插法
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return std::pair<iterator, bool>(iterator(tmp, this), true);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type &obj) {
        const size_type n = bkt_num(obj);
        node *first = buckets[n];

        for (node *cur = first; cur; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj))) {  // 插入点存在的情况
                auto tmp = new_node(obj);
                tmp->next = cur->next;
                cur->next = tmp;
                ++num_elements;
                return iterator(tmp, this);
            }
        }
        node *tmp = new_node(obj);   // 头插法
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return iterator(tmp, this);
    }

    // 6. 删除辅助函数
    // 删除第n个bucket下从起始到last的所有节点
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase_bucket(
        const size_type n, node *last) {
        auto cur = buckets[n];
        while (cur != last) {
            auto next = cur->next;
            delete_node(cur);
            cur = next;
            buckets[n] = cur;
            --num_elements;
        }
    }

    // 删除第n个bucket[first, last)的节点
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase_bucket(
        const size_type n, node *first, node *last) {
        auto cur = buckets[n];
        if (cur == first)
            erase_bucket(n, last);
        else {
            node *next;
            for (next = cur->next; next != first; cur = next, next = cur->next);
            while (next != last) {
                cur->next = next->next;
                delete_node(next);
                next = cur->next;
                --num_elements;
            }
        }
    }

    /***********************构造, 复制, 析构相关***********************/
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    hashtable(size_type n, const hasher &hf, const key_equal &eql)
        :hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
        initialize_buckets(n);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    hashtable(size_type n, const hasher &hf, const key_equal &eql, const ExtractKey &ext)
        :hash(hf), equals(eql), get_key(ext), num_elements(0) {
        initialize_buckets(n);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    hashtable(const hashtable &ht)
        :hash(ht.hash), equals(ht.equals), get_key(ht.get_key), num_elements(0) {
        copy_from(ht);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> &
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator=(const hashtable &ht) {
        if (&ht != this) {
            clear();
            hash = ht.hash;
            equals = ht.equals;
            get_key = ht.get_key;
            copy_from(ht);
        }
    }

    /******************************迭代器相关*************************/
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() {
        for (auto n = 0; n < buckets.size(); ++n)
            if (buckets[n])
                return iterator(buckets[n], this);
        return end();
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() const {
        for (auto n = 0; n < buckets.size(); ++n)
            if (buckets[n])
                return const_iterator(buckets[n], this);
        return end();
    }


    /**************************容量以及getter相关**********************/
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::elems_in_bucket(size_type bkt) const {
        auto result = 0;
        for (auto cur = buckets[bkt]; cur; cur = cur->next)
            ++result;
        return result;
    }

    /****************************修改容器相关**************************/
    // 2. insert
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const value_type &obj) {
        resize(num_elements + 1);
        return insert_unique_noresize(obj);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const value_type &obj) {
        resize(num_elements + 1);
        return insert_equal_noresize(obj);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    template<class InputIterator>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_unique(InputIterator f, InputIterator l) {
        if constexpr (_is_same<iterator_category_t<InputIterator>, forward_iterator_tag>::value) {
            auto n = distance(f, l);
            resize(num_elements + n);
            for (; n > 0; --n, ++f)
                insert_unique_noresize(*f);
        } else {
            for (; f != l; ++f)
                insert_unique(*f);
        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    template<class InputIterator>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    insert_equal(InputIterator f, InputIterator l) {
        if constexpr (_is_same<iterator_category_t<InputIterator>, forward_iterator_tag>::value) {
            auto n = distance(f, l);
            resize(num_elements + n);
            for (; n > 0; --n, ++f)
                insert_equal_noresize(*f);
        } else {
            for (; f != l; ++f)
                insert_equal(*f);
        }
    }

    // 3. erase
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const key_type &key) {
        auto n = bkt_num_key(key);
        auto first = buckets[n];
        size_type _erased = 0;
        if (first) {
            auto cur = first;
            auto next = cur->next;
            while (next) {
                if (equals(get_key(next->val), key)) {
                    cur->next = next->next;
                    delete_node(next);
                    next = cur->next;
                    ++_erased;
                    --num_elements;
                } else {
                    cur = next;
                    next = cur->next;
                }
            }
            if (equals(get_key(first->val), key)) {
                buckets[n] = first->next;
                delete_node(first);
                ++_erased;
                --num_elements;
            }
        }
        return _erased;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator &it) {
        auto p = it.cur;
        if (p) {
            const size_type n = bkt_num(p->val);
            node *cur = buckets[n];
            if (cur == p) {
                buckets[n] = cur->next;
                delete_node(cur);
                --num_elements;
            } else {
                node *next = cur->next;
                while (next) {
                    if (next == p) {
                        cur->next = next->next;
                        delete_node(next);
                        --num_elements;
                        break;
                    } else {
                        cur = next;
                        next = next->next;
                    }
                }
            }
        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(iterator first, iterator last) {
        size_type f_bucket = first.cur ? bkt_num(first.cur->val) : buckets.size();
        size_type l_bucket = last.cur ? bkt_num(last.cur->val) : buckets.size();

        if (first.cur == last.cur) return;
        else if (f_bucket == l_bucket) {
            erase_bucket(f_bucket, first.cur, last.cur);
        } else {
            erase_bucket(f_bucket, first.cur, nullptr);
            for (auto n = f_bucket + 1; n < l_bucket; ++n)
                erase_bucket(n, nullptr);
            if (l_bucket != buckets.size())
                erase_bucket(l_bucket, last.cur);
        }
    }

    // 4. swap
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::swap(hashtable &ht) {
        std::swap(hash, ht.hash);
        std::swap(equals, ht.equals);
        std::swap(get_key, ht.get_key);
        buckets.swap(ht.buckets);
        std::swap(num_elements, ht.num_elements);
    }

    /****************************访问容器相关**************************/
    // 1. find
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const key_type &key) {
        auto n = bkt_num_key(key);
        node *first;
        for (first = buckets[n]; first && !equals(get_key(first->val), key); first = first->next) {}
        return iterator(first, this);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const key_type &key) const {
        auto n = bkt_num_key(key);
        node *first;
        for (first = buckets[n]; first && !equals(get_key(first->val), key); first = first->next) {}
        return const_iterator(first, this);
    }

    // 2. count
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::count(const key_type &key) const {
        auto n = bkt_num_key(key);
        auto result = 0;
        for (const node *cur = buckets[n]; cur; cur = cur->next)
            if (equals(get_key(cur->val), key)) ++result;
        return result;
    }

    // 3. equal_range
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,
        typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type &key) {
        using pii = std::pair<iterator, iterator>;
        const size_type n = bkt_num_key(key);
        for (auto first = buckets[n]; first; first = first->next) {
            if (equals(get_key(first->val), key)) {
                for (auto cur = first->next; cur; cur = cur->next) {
                    if (!equals(get_key(cur->val), key))
                        return pii(iterator(first, this), iterator(cur, this));
                }
                for (auto m = n + 1; m < buckets.size(); ++m) {
                    if (buckets[m])
                        return pii(iterator(first, this), iterator(buckets[m], this));
                }
                return pii(iterator(first, this), end());
            }
        }
        return pii(end(), end());
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator,
        typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type &key) const {
        using pii = std::pair<const_iterator, const_iterator>;
        const size_type n = bkt_num_key(key);
        for (auto first = buckets[n]; first; first = first->next) {
            if (equals(get_key(first->val), key)) {
                for (auto cur = first->next; cur; cur = cur->next) {
                    if (!equals(get_key(cur->val), key))
                        return pii(const_iterator(first, this), const_iterator(cur, this));
                }
                for (auto m = n + 1; m < buckets.size(); ++m) {
                    if (buckets[m])
                        return pii(const_iterator(first, this), const_iterator(buckets[m], this));
                }
                return pii(const_iterator(first, this), end());
            }
        }
        return pii(end(), end());
    }

    // 4. find_or_insert(其实该函数也改变了容器)
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::reference
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find_or_insert(const value_type &obj) {
        resize(num_elements + 1);
        auto n = bkt_num(obj);
        node *first = buckets[n];
        for (node *cur = first; cur; cur = cur->next)
            if (equals(get_key(cur->val), get_key(obj)))
                return cur->val;
        node *tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return tmp->val;
    }


    /********************************友元函数*****************************/
    template<class _Vl, class _Ky, class _HF, class _Ex, class _Eq, class _Al>
    inline bool operator==(const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &ht1,
                           const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &ht2) {
        if (ht1.buckets.size() != ht2.buckets.size())
            return false;
        for (int n = 0; n < ht1.buckets.size(); ++n) {
            auto cur1 = ht1.buckets[n];
            auto cur2 = ht2.buckets[n];
            for (; cur1 && cur2 && cur1->val == cur2->val; cur1 = cur1->next, cur2 = cur2->next) {}
            if (cur1 || cur2)
                return false;
        }
        return true;
    }

    template<class _Vl, class _Ky, class _HF, class _Ex, class _Eq, class _Al>
    inline bool operator!=(const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &ht1,
                           const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &ht2) {
        return !(ht1 == ht2);
    }

    template<class _Vl, class _Ky, class _HF, class _Ex, class _Eq, class _Al>
    inline void swap(const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &ht1,
                     const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &ht2) {
        ht1.swap(ht2);
    }

}

#endif //MINISTL_STL_HASH_IMP_H
