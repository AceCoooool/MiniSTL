#ifndef MINISTL_STL_HASH_SET_H
#define MINISTL_STL_HASH_SET_H

#include "../Hashtable/stl_hash.imp.h"
#include "../Hashtable/stl_hash_fun.h"
#include "../../Functor/stl_function.h"

namespace MiniSTL {
    template<class Value, class HashFcn=hash<Value>, class EqualKey=equal_to<Value>, class Alloc=alloc>
    class hash_set {
    private:
        using HT = hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc>;
        HT ht;   // 底层机制hash table

    public:
        // alias declaration
        using key_type = typename HT::key_type;
        using value_type = typename HT::value_type;
        using hasher = typename HT::hasher;
        using key_equal = typename HT::key_equal;
        // 显然, set的键值无法修改，采取const形式
        using pointer = typename HT::const_pointer;
        using const_pointer = typename HT::const_pointer;
        using reference = typename HT::const_reference;
        using const_reference = typename HT::const_reference;
        using iterator = typename HT::const_iterator;
        using const_iterator = typename HT::const_iterator;

        using size_type = typename HT::size_type;
        using difference_type = typename HT::difference_type;


    public:
        // 构造, 复制, 析构
        hash_set() : ht(100, hasher(), key_equal()) {}  // 默认buckets大小为100
        explicit hash_set(size_type n) : ht(n, hasher(), key_equal()) {}
        hash_set(size_type n, const hasher &hf) : ht(n, hf, key_equal()) {}
        hash_set(size_type n, const hasher &hf, const key_equal &eql) : ht(n, hf, eql) {}

        template<class InputIterator>
        hash_set(InputIterator f, InputIterator l): hash_set() { ht.insert_unique(f, l); };
        template<class InputIterator>
        hash_set(InputIterator f, InputIterator l, size_type n): hash_set(n) { ht.insert_unique(f, l); };
        template<class InputIterator>
        hash_set(InputIterator f, InputIterator l, size_type n, const hasher &hf) : hash_set(n, hf) {
            ht.insert_unique(f, l);
        };
        template<class InputIterator>
        hash_set(InputIterator f, InputIterator l, size_type n, const hasher &hf, const key_equal &eql)
            : hash_set(n, hf, eql) { ht.insert_unique(f, l); };


        // 迭代器相关
        iterator begin() const { return ht.begin(); }
        iterator end() const { return ht.end(); }

        // 容量以及getter相关
        hasher hash_funct() const { return ht.hash_funct(); }
        key_equal key_eq() const { return ht.key_eq(); }

        size_type size() const { return ht.size(); }
        size_type max_size() const { return ht.max_size(); }
        bool empty() const { return ht.empty(); }

        // 修改容器相关
        // 1. swap
        void swap(hash_set &hs) { ht.swap(hs.ht); }
        // 2. insert
        std::pair<iterator, bool> insert(const value_type &obj) {
            auto p = ht.insert_unique(obj);
            return std::pair<iterator, bool>(p.first, p.second);
        }

        template<class InputIterator>
        void insert(InputIterator f, InputIterator l) {
            ht.insert_unique(f, l);
        }

        std::pair<iterator, bool> insert_noresize(const value_type &obj) {
            auto p = ht.insert_unique_noresize(obj);
            return std::pair<iterator, bool>(p.first, p.second);
        }
        // 3. erase
        size_type erase(const key_type &key) { return ht.erase(key); }
        void erase(iterator it) { ht.erase(it); }
        void erase(iterator f, iterator l) { ht.erase(f, l); }
        // 4. resize&clear
        void resize(size_type hint) { ht.resize(hint); }
        void clear() { ht.clear(); }

        // 访问容器相关
        iterator find(const key_type &key) const { return ht.find(key); }
        size_type count(const key_type &key) const { return ht.count(key); }
        std::pair<iterator, iterator> equal_range(const key_type &key) const { return ht.equal_range(key); }
        size_type bucket_count() const { return ht.bucket_count(); }
        size_type max_bucket_count() const { return ht.max_bucket_count(); }
        size_type elems_in_bucket(size_type n) const { return ht.elems_in_bucket(n); }


        // 友元函数
        template<class _Val, class _HF, class _EqK, class _Al>
        friend bool operator==(const hash_set<_Val, _HF, _EqK, _Al> &,
                               const hash_set<_Val, _HF, _EqK, _Al> &);

        template<class _Val, class _HF, class _EqK, class _Al>
        friend bool operator!=(const hash_set<_Val, _HF, _EqK, _Al> &,
                               const hash_set<_Val, _HF, _EqK, _Al> &);

        template<class _Val, class _HF, class _EqK, class _Al>
        friend void swap(hash_set<_Val, _HF, _EqK, _Al> &,
                         hash_set<_Val, _HF, _EqK, _Al> &);
    };

    template<class _Value, class _HashFcn, class _EqualKey, class _Alloc>
    inline bool operator==(const hash_set<_Value, _HashFcn, _EqualKey, _Alloc> &hs1,
                           const hash_set<_Value, _HashFcn, _EqualKey, _Alloc> &hs2) {
        return hs1.ht == hs2.ht;
    }

    template<class _Value, class _HashFcn, class _EqualKey, class _Alloc>
    inline bool operator!=(const hash_set<_Value, _HashFcn, _EqualKey, _Alloc> &hs1,
                           const hash_set<_Value, _HashFcn, _EqualKey, _Alloc> &hs2) {
        return !(hs1 == hs2);
    }

    template<class _Val, class _HashFcn, class _EqualKey, class _Alloc>
    inline void swap(hash_set<_Val, _HashFcn, _EqualKey, _Alloc> &hs1,
                     hash_set<_Val, _HashFcn, _EqualKey, _Alloc> &hs2) {
        hs1.swap(hs2);
    }
}

#endif //MINISTL_STL_HASH_SET_H
