#ifndef MINISTL_STL_PRIORITY_QUEUE_H
#define MINISTL_STL_PRIORITY_QUEUE_H

#include "../Vector/stl_vector.imp.h"
#include "../../Algorithm/Heap/stl_heap.h"

namespace MiniSTL {
    template<class T, class Sequence = vector<T>, class Compare = std::less<typename Sequence::value_type> >
    class priority_queue {
    public:
        // alias declarations
        using value_type = typename Sequence::value_type;
        using size_type = typename Sequence::size_type;
        using reference = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;
        // no iterators

    private:
        // data
        Sequence c;
        Compare comp;

    public:
        // 构造, 复制, 析构
        priority_queue() : c() {}
        explicit priority_queue(const Compare &compare) : c(), comp(compare) {}
        priority_queue(std::initializer_list<T> il, const Compare &compare = Compare()) : c(il), comp(compare) {
            MiniSTL::make_heap(c.begin(), c.end(), comp);
        }
        template<class InputIterator>
        priority_queue(InputIterator first, InputIterator last, const Compare &compare = Compare())
                : c(first, last), comp(compare) {
            MiniSTL::make_heap(c.begin(), c.end(), comp);
        }
        priority_queue(const priority_queue &rhs) : c(rhs.c), comp(rhs.comp) {}
        priority_queue(priority_queue &&rhs) noexcept : c(std::move(rhs.c)), comp(std::move(rhs.comp)) {}

        priority_queue &operator=(const priority_queue &rhs) {
            c = rhs.c;
            comp = rhs.comp;
            return *this;
        }

        priority_queue &operator=(priority_queue &&rhs) noexcept {
            c = std::move(rhs.c);
            comp = std::move(rhs.comp);
            return *this;
        }

        ~priority_queue() = default;

        // 迭代器相关 --- 无

        // 访问元素相关
        const_reference top() const noexcept { return c.front(); }

        // 修改容器相关
        void push(const value_type &value) {
            try {
                c.push_back(value);
                MiniSTL::push_heap(c.begin(), c.end(), comp);
            } catch (std::exception &) {
                c.clear();
            }
        }
        void pop() {
            try {
                MiniSTL::pop_heap(c.begin(), c.end(), comp);
                c.pop_back();
            } catch (std::exception &) {
                c.clear();
            }
        }

        // 容量相关
        bool empty() const noexcept { return c.empty(); }
        size_type size() const noexcept { return c.size(); }
    };
}

#endif //MINISTL_STL_PRIORITY_QUEUE_H
