#ifndef MINISTL_STL_HASH_H
#define MINISTL_STL_HASH_H

#include "../../Allocator/allocator.h"
#include "../../Iterator/stl_iterator.h"
#include "../../Sequence/Vector/stl_vector.imp.h"

namespace MiniSTL {
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    class hashtable;

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct hashtable_const_iterator;

    /*------------------------------结点----------------------------*/
    template<class Value>
    struct hashtable_node {
        Value val;
        hashtable_node *next;
    };

    /*-----------------------------迭代器----------------------------*/
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct hashtable_iterator {
        using hash_table = hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using iterator = hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using const_iterator = hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using node = hashtable_node<Value>;

        using iterator_category = forward_iterator_tag;   // 前向迭代器
        using value_type = Value;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using reference = Value &;
        using pointer = Value *;

        node *cur;           // 当前指向的节点
        hash_table *ht;      // 保持与hashtable的连接

        // 构造函数
        hashtable_iterator(node *n, hash_table *tab) : cur(n), ht(tab) {}
        hashtable_iterator() = default;

        // 比较操作
        bool operator==(const iterator &rhs) const { return cur == rhs.cur; }
        bool operator!=(const iterator &rhs) const { return cur != rhs.cur; }

        // 运算符重载
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        iterator &operator++();
        iterator operator++(int);
    };

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct hashtable_const_iterator {
        using hash_table = hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using iterator = hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using const_iterator = hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using node = hashtable_node<Value>;

        using iterator_category = forward_iterator_tag;   // 前向迭代器
        using value_type = Value;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using reference = const Value &;
        using pointer = const Value *;

        const node *cur;           // 当前指向的节点
        const hash_table *ht;      // 保持与hashtable的连接

        // 构造函数
        hashtable_const_iterator(const node *n, const hash_table *tab) : cur(n), ht(tab) {}
        hashtable_const_iterator() = default;
        hashtable_const_iterator(const iterator &it) : cur(it.cur), ht(it.ht) {}

        // 比较操作
        bool operator==(const const_iterator &rhs) const { return cur == rhs.cur; }
        bool operator!=(const const_iterator &rhs) const { return cur != rhs.cur; }

        // 运算符重载
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        const_iterator &operator++();
        const_iterator operator++(int);
    };


    /*-------------------------------哈希表----------------------------*/
    // 当size为质数且负载低于0.5时两次查找必然可获取正确位置
    // 预先存储28个质数
    static const int __stl_num_primes = 28;
    static const unsigned long __stl_prime_list[__stl_num_primes] = {
        53ul, 97ul, 193ul, 389ul, 769ul,
        1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
        49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
        1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
        50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
        1610612741ul, 3221225473ul, 4294967291ul
    };

    // 找出大于n的最小质数
    inline unsigned long __stl_next_prime(unsigned long n) {
        const unsigned long *first = __stl_prime_list;
        const unsigned long *last = __stl_prime_list + __stl_num_primes;
        auto pos = std::lower_bound(first, last, n);
        return pos == last ? *(last - 1) : *pos;
    }


    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc=alloc>
    class hashtable {
    public:
        using key_type = Key;
        using value_type = Value;
        using hasher = HashFcn;
        using key_equal = EqualKey;

        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;

        using iterator = hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using const_iterator = hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

        friend struct hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        friend struct hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;


    private:
        // 以下三者均为function object
        hasher hash;
        key_equal equals;
        ExtractKey get_key;            // 从node中取出键值的方法

        using node = hashtable_node<Value>;
        using node_allocator = allocator<node>;

        vector<node *> buckets;  // 以vector表征
        size_type num_elements;

    private:
        // 辅助函数
        // 1. 内存管理
        node *get_node() { return node_allocator::allocate(); }
        void put_node(node *p) { node_allocator::deallocate(p); }
        node *new_node(const value_type &v);
        void delete_node(node *n);
        void initialize_buckets(size_type n);
        // 2. 调整内存大小(为public, 放置在后面)
        // 3. 哈希表复制
        void copy_from(const hashtable &ht);
        // 4. 哈希映射关系
        size_type bkt_num_key(const key_type &key, size_t n) const;
        size_type bkt_num_key(const key_type &key) const;
        size_type bkt_num(const value_type &obj, size_t n) const;
        size_type bkt_num(const value_type &obj) const;

        // 5. 插入辅助函数
        std::pair<iterator, bool> insert_unique_noresize(const value_type &obj);
        iterator insert_equal_noresize(const value_type &obj);

        // 6. 删除辅助函数
        void erase_bucket(const size_type n, node *last);
        void erase_bucket(const size_type n, node *first, node *last);

    public:
        // 构造, 复制, 析构
        hashtable(size_type n, const hasher &hf, const key_equal &eql);
        hashtable(size_type n, const hasher &hf, const key_equal &eql, const ExtractKey &ext);
        hashtable(const hashtable &ht);

        hashtable &operator=(const hashtable &ht);

        ~hashtable() { clear(); }

        // 迭代器相关
        iterator begin();
        iterator end() { return iterator(nullptr, this); }
        const_iterator begin() const;
        const_iterator end() const { return const_iterator(nullptr, this); }

        // 容量以及getter相关
        hasher hash_funct() const { return hash; }
        key_equal key_eq() const { return equals; }
        size_type size() const { return num_elements; }
        size_type max_size() const { return size_type(-1); }
        bool empty() const { return size() == 0; }
        size_type bucket_count() const { return buckets.size(); } // buckets 大小
        size_type max_bucket_count() const { return __stl_prime_list[(int) __stl_num_primes - 1]; }
        size_type elems_in_bucket(size_type bkt) const;

        // 修改容器相关的操作
        // 1. 调整容器大小(这部分放在了辅助函数部分)
        void resize(size_type num_elements_hint);
        void clear();
        // 2. 插入
        std::pair<iterator, bool> insert_unique(const value_type &obj);
        iterator insert_equal(const value_type &obj);
        template<class InputIterator>
        void insert_unique(InputIterator f, InputIterator l);
        template<class InputIterator>
        void insert_equal(InputIterator f, InputIterator l);
        // 3. 删除
        size_type erase(const key_type &key);
        void erase(const iterator &it);
        void erase(iterator first, iterator last);
        // 4. 交换
        void swap(hashtable &ht);

        // 访问容器相关
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;
        size_type count(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key);
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;
        // 先查找要插入节点是否在hash table 中, 有返回, 没有插入新节点
        reference find_or_insert(const value_type &obj);

        // 友元函数
        template<class _Vl, class _Ky, class _HF, class _Ex, class _Eq, class _Al>
        friend bool operator==(const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &,
                               const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &);
        template<class _Vl, class _Ky, class _HF, class _Ex, class _Eq, class _Al>
        friend bool operator!=(const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &,
                               const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &);
        template<class _Vl, class _Ky, class _HF, class _Ex, class _Eq, class _Al>
        friend void swap(const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &,
                         const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al> &);
    };
}

#endif //MINISTL_STL_HASH_H
