#ifndef MINISTL_STL_STACK_H
#define MINISTL_STL_STACK_H

#include "../Deque/stl_deque.imp.h"

namespace MiniSTL {
    template<class T, class Sequence = deque <T> >
    class stack {
    public:
        using value_type = typename Sequence::value_type;
        using size_type = typename Sequence::size_type;
        using reference = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;

    private:// data
        Sequence c;

    public:
        // 构造, 复制, 析构相关函数
        stack() : c() {}
        explicit stack(const Sequence &rhs) : c(rhs) {}
        stack(const stack &rhs) : c(rhs.c) {}
        stack(stack &&rhs) noexcept: c(std::move(rhs.c)) {}

        stack &operator=(const stack &rhs) { c.operator=(rhs.c); }
        stack &operator=(stack &&rhs) noexcept { c.operator=(std::move(rhs.c)); }

        ~stack() = default;

        // 比较操作相关
        template<class T1, class Sequence1>
        friend bool operator==(const stack<T1, Sequence1> &, const stack<T1, Sequence1> &);
        template<class T1, class Sequence1>
        friend bool operator!=(const stack<T1, Sequence1> &, const stack<T1, Sequence1> &);

        // 迭代器
        // 无

        // 访问元素相关
        reference top() { return c.back(); }
        const_reference top() const noexcept { return c.back(); }

        // 修改容器相关的操作
        void push(const value_type &value) { c.push_back(value); }
        void pop() { c.pop_back(); }

        // 与容量相关
        bool empty() const noexcept { return c.empty(); }
        size_type size() const noexcept { return c.size(); }
    };

    template<class T, class Sequence>
    inline bool operator==(const stack<T, Sequence> &lhs, const stack<T, Sequence> &rhs) {
        return lhs.c == rhs.c;
    }

    template<class T, class Sequence>
    inline bool operator!=(const stack<T, Sequence> &lhs, const stack<T, Sequence> &rhs) {
        return !(lhs.c == rhs.c);
    }
}

#endif //MINISTL_STL_STACK_H
