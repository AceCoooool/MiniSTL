#ifndef MINISTL_STL_HASH_MAP_H
#define MINISTL_STL_HASH_MAP_H

#include "../Hashtable/stl_hash.imp.h"
#include "../Hashtable/stl_hash_fun.h"
#include "../../Functor/stl_function.h"

namespace MiniSTL {
    template<class Key, class T, class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
    class hash_map {
    private:
        using HT = hashtable <std::pair<const Key, T>, Key, HashFcn, select1st<std::pair<const Key, T>>, EqualKey, Alloc>;
        HT ht;       // 底层数据结构

    public:
        using key_type = typename HT::key_type;
        using data_type = T;
        using mapped_type = T;
        using value_type = typename HT::value_type;
        using hasher = typename HT::hasher;
        using key_equal = typename HT::key_equal;

        using pointer = typename HT::pointer;
        using const_pointer = typename HT::const_pointer;
        using reference = typename HT::reference;
        using const_reference = typename HT::const_reference;
        using iterator = typename HT::iterator;
        using const_iterator = typename HT::const_iterator;

        using size_type = typename HT::size_type;
        using difference_type = typename HT::difference_type;

    public:
        // 构造, 复制, 析构
        hash_map() : ht(100, hasher(), key_equal()) {}  // 默认 hashtable 大小为 100
        explicit hash_map(size_type n) : ht(n, hasher(), key_equal()) {}
        hash_map(size_type n, const hasher &hf) : ht(n, hf, key_equal()) {}
        hash_map(size_type n, const hasher &hf, const key_equal &eql) : ht(n, hf, eql) {}

        template<class InputIterator>
        hash_map(InputIterator f, InputIterator l): ht(100, hasher(), key_equal()) { ht.insert_unique(f, l); }
        template<class InputIterator>
        hash_map(InputIterator f, InputIterator l, size_type n): ht(n, hasher(), key_equal()) {
            ht.insert_unique(f, l);
        }
        template<class InputIterator>
        hash_map(InputIterator f, InputIterator l, size_type n, const hasher &hf): ht(n, hf, key_equal()) {
            ht.insert_unique(f, l);
        }
        template<class InputIterator>
        hash_map(InputIterator f, InputIterator l, size_type n, const hasher &hf, const key_equal &eql)
            : ht(n, hf, eql) { ht.insert_unique(f, l); }

        // 迭代器相关
        iterator begin() { return ht.begin(); }
        iterator end() { return ht.end(); }
        const_iterator begin() const { return ht.begin(); }
        const_iterator end() const { return ht.end(); }

        // 容量及getter相关
        hasher hash_funct() const { return ht.hash_funct(); }
        key_equal key_eq() const { return ht.key_eq(); }

        size_type size() const { return ht.size(); }
        size_type max_size() const { return ht.max_size(); }
        bool empty() const { return ht.empty(); }

        size_type bucket_count() const { return ht.bucket_count(); }
        size_type max_bucket_count() const { return ht.max_bucket_count(); }
        size_type elems_in_bucket(size_type n) const { return ht.elems_in_bucket(n); }

        // 访问容器相关
        iterator find(const key_type &key) { return ht.find(key); }
        const_iterator find(const key_type &key) const { return ht.find(key); }
        size_type count(const key_type &key) const { return ht.count(key); }

        std::pair<iterator, iterator> equal_range(const key_type &key) { return ht.equal_range(key); }
        std::pair<const_iterator, const_iterator>
        equal_range(const key_type &key) const { return ht.equal_range(key); }


        // 修改容器相关
        // 1. insert
        std::pair<iterator, bool> insert(const value_type &obj) { return ht.insert_unique(obj); }
        template<class InputIterator>
        void insert(InputIterator f, InputIterator l) { ht.insert_unique(f, l); }
        std::pair<iterator, bool> insert_noresize(const value_type &obj) { return ht.insert_unique_noresize(obj); }

        // 2. erase
        size_type erase(const key_type &key) { return ht.erase(key); }
        void erase(iterator it) { ht.erase(it); }
        void erase(iterator f, iterator l) { ht.erase(f, l); }

        // 3. clear & resize
        void clear() { ht.clear(); }
        void resize(size_type hint) { ht.resize(hint); }

        // 4. swap
        void swap(hash_map &hs) { ht.swap(hs.ht); }

        // 5. 下标操作
        T &operator[](const key_type &key) {
            return ht.find_or_insert(value_type(key, T())).second;
        }

        // 友元函数
        template<class _K1, class _T1, class _HF, class _EqK, class _Al>
        friend bool operator==(const hash_map<_K1, _T1, _HF, _EqK, _Al> &,
                               const hash_map<_K1, _T1, _HF, _EqK, _Al> &);

    };

    template<class _Key, class _Tp, class _HashFcn, class _EqlKey, class _Alloc>
    inline bool operator==(const hash_map<_Key, _Tp, _HashFcn, _EqlKey, _Alloc> &hm1,
                           const hash_map<_Key, _Tp, _HashFcn, _EqlKey, _Alloc> &hm2) {
        return hm1.ht == hm2.ht;
    }

    template<class _Key, class _Tp, class _HashFcn, class _EqlKey, class _Alloc>
    inline bool operator!=(const hash_map<_Key, _Tp, _HashFcn, _EqlKey, _Alloc> &hm1,
                           const hash_map<_Key, _Tp, _HashFcn, _EqlKey, _Alloc> &hm2) {
        return !(hm1 == hm2);
    }

    template<class _Key, class _Tp, class _HashFcn, class _EqlKey, class _Alloc>
    inline void swap(hash_map<_Key, _Tp, _HashFcn, _EqlKey, _Alloc> &hm1,
                     hash_map<_Key, _Tp, _HashFcn, _EqlKey, _Alloc> &hm2) {
        hm1.swap(hm2);
    }
}

#endif //MINISTL_STL_HASH_MAP_H
