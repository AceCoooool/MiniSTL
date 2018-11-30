#ifndef MINISTL_STL_HEAP_H
#define MINISTL_STL_HEAP_H

#include "../../Iterator/stl_iterator.h"
#include <functional>  // TODO


namespace MiniSTL {
    template<class RandomAccessIterator, class Distance, class T, class Compare>
    void push_heap_aux(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value,
                       const Compare &comp = Compare()) {
        Distance parent = (holeIndex - 1) / 2;                          // 找出父节点
        while (holeIndex > topIndex && comp(*(first + parent), value)) { // 尚未到达顶端且其值大于父节点
            *(first + holeIndex) = *(first + parent);                   // 令洞值为父值
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;                               // 更新父节点
        }
        // 结束循环时更新洞值
        *(first + holeIndex) = value;
    }

    // 两个形参表征顺序表头尾, 当前元素已作为顺序表尾部元素 TODO: modify less
    template<class RandomAccessIterator, class Compare = std::less<value_type_t < RandomAccessIterator>>>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp = Compare()) {
        using Distance = difference_type_t<RandomAccessIterator>;
        push_heap_aux(first, Distance((last - first) - 1), Distance(0), *(last - 1), comp);
    }

    // 调整树
    template<class RandomAccessIterator, class Distance, class T, class Compare=std::less<
            value_type_t < RandomAccessIterator>>>
    void adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value,
                     const Compare &comp = Compare()) {
        Distance topIndex = holeIndex;
        Distance secondChild = 2 * holeIndex + 2;   // 洞点右子
        while (secondChild < len) {                 // 当前尚存在右子
            // 自此secondChild代表较大子节点
            if (comp(*(first + secondChild), *(first + (secondChild - 1))))  // 右子小于左子
                secondChild--;
            *(first + holeIndex) = *(first + secondChild);                   // 将洞点赋值为较大子值
            holeIndex = secondChild;                                         // 更新洞点
            secondChild = 2 * (holeIndex + 1);                               // 再次指向洞点右子
        }
        if (secondChild == len) {// 不存在右子,令其左子值为洞值,并将洞号传递至左子(左子必为除首节点外最大者)
            *(first + holeIndex) = *(first + (secondChild - 1));
            holeIndex = secondChild - 1;
        }
        // 将原有的末端元素填入(更新那个洞)
        push_heap_aux(first, holeIndex, topIndex, value, comp);
    }

    // 将heap的根节点（最大值）取走（其实放置于vector尾部）
    template<class RandomAccessIterator, class Compare = std::less<value_type_t < RandomAccessIterator>>>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp = Compare()) {
        using Distance = difference_type_t<RandomAccessIterator>;
        auto value = *(last - 1);
        *(last - 1) = *first;
        adjust_heap(first, Distance(0), Distance(last - first - 1), value, comp);
    }

    // 堆排序（不断调用pop_heap即可）
    template<class RandomAccessIterator, class Compare = std::less<value_type_t < RandomAccessIterator>>>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp = Compare()) {
        while (first != last) {
            MiniSTL::pop_heap(first, last--, comp);  // TODO: 不加MiniSTL出现bug---多重重载
        }
    }

    template<class RandomAccessIterator, class Compare = std::less<value_type_t < RandomAccessIterator>>>
    inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp = Compare()) {
        // 遍历所有有资格为root的点
        if (last - first < 2) return;
        auto len = last - first;
        // 找到需要重排的头部(即为最后一个有资格为root的点)
        auto holeIndex = (len - 2) / 2;
        while (true) {
            // 重排以holeIndex为首的子树
            adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
            if (holeIndex == 0) return;
            holeIndex--;
        }
    }

}

#endif //MINISTL_STL_HEAP_H
