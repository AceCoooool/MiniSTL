#ifndef MINISTL_STL_QUEUE_H
#define MINISTL_STL_QUEUE_H

#include "../Deque/stl_deque.imp.h"

namespace MiniSTL {
    template<class T, class Sequence = deque <T> >
    class queue {
    public:// alias declarations
        using value_type = typename Sequence::value_type;
        using size_type = typename Sequence::size_type;
        using reference = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;

    private:// data member
        Sequence c;

    public:
        // 构造, 复制, 析构相关函数
        queue() : c() {}
        explicit queue(const Sequence &rhs) : c(rhs) {}
        queue(const queue &rhs) : c(rhs.c) {}
        queue(queue &&rhs) noexcept: c(std::move(rhs.c)) {}

        queue &operator=(const queue &rhs) { c.operator=(rhs.c); }
        queue &operator=(queue &&rhs) noexcept { c.operator=(std::move(rhs.c)); }

        ~queue() = default;

        // 比较操作相关
        template<class T1, class Sequence1>
        friend bool operator==(const queue<T1, Sequence1> &, const queue<T1, Sequence1> &);
        template<class T1, class Sequence1>
        friend bool operator!=(const queue<T1, Sequence1> &, const queue<T1, Sequence1> &);

        // 迭代器
        // 无

        // 访问元素相关
        reference front() { return c.front(); }
        const_reference front() const { return c.front(); }

        // 修改容器相关的操作
        void push(const value_type &value) { c.push_back(value); }
        void pop() { c.pop_front(); }

        // 与容量相关
        bool empty() const noexcept { return c.empty(); }
        size_type size() const noexcept { return c.size(); }
    };

    template<class T, class Sequence>
    inline bool operator==(const queue<T, Sequence> &lhs, const queue<T, Sequence> &rhs) {
        return lhs.c == rhs.c;
    }

    template<class T, class Sequence>
    inline bool operator!=(const queue<T, Sequence> &lhs, const queue<T, Sequence> &rhs) {
        return !(lhs.c == rhs.c);
    }
}

#endif //MINISTL_STL_QUEUE_H
