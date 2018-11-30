#ifndef MINISTL_STL_ALLOC_IMP_H
#define MINISTL_STL_ALLOC_IMP_H

#include "stl_alloc.h"

namespace MiniSTL {
    char *alloc::start_free = nullptr;
    char *alloc::end_free = nullptr;
    size_t alloc::heap_size = 0;

    // TODO: whether there is a smart way?
    alloc::obj *alloc::free_list[alloc::__NFREELISTS] = {
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr
    };

    void *alloc::allocate(size_t bytes) {
        if (bytes > __MAX_BYTES) {
            return malloc(bytes);
        }
        size_t index = FREELIST_INDEX(bytes);
        obj *list = free_list[index];
        if (list) {                          // 此list还有空间给我们
            free_list[index] = list->next;
            return list;
        } else {                             // 此list没有足够的空间, 需要从内存池里面取空间
            return refill(ROUND_UP(bytes));
        }
    }

    void alloc::deallocate(void *ptr, size_t bytes) {
        if (bytes > __MAX_BYTES) {
            free(ptr);
        } else {
            size_t index = FREELIST_INDEX(bytes);
            obj *node = static_cast<obj *>(ptr);
            node->next = free_list[index];
            free_list[index] = node;
        }
    }

    void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) {
        deallocate(ptr, old_sz);
        ptr = allocate(new_sz);

        return ptr;
    }

    // 返回一个大小为n的对象, 并且有时候会为适当的free list增加节点
    // 假设bytes已经上调为8的倍数
    void *alloc::refill(size_t bytes) {
        size_t nobjs = NOBJS;
        // 从内存池里取
        char *chunk = chunk_alloc(bytes, nobjs);
        obj **my_free_list;
        obj *result;
        obj *current_obj, *next_obj;

        if (nobjs == 1) {    // 取出的空间只够一个对象使用
            return chunk;
        } else {
            my_free_list = free_list + FREELIST_INDEX(bytes);
            result = reinterpret_cast<obj *>(chunk);
            *my_free_list = next_obj = reinterpret_cast<obj *>(chunk + bytes);
            // 将取出的多余的空间加入到相应的free list里面去
            for (int i = 1; i != nobjs - 1; ++i) {
                current_obj = next_obj;
                next_obj = reinterpret_cast<obj *>(reinterpret_cast<char *>(next_obj) + bytes);
                current_obj->next = next_obj;
            }
            next_obj->next = nullptr;
            return result;
        }
    }

    // 假设bytes已经上调为8的倍数
    char *alloc::chunk_alloc(size_t bytes, size_t &nobjs) {
        char *result;
        size_t total_bytes = bytes * nobjs;
        size_t bytes_left = end_free - start_free;

        if (bytes_left >= total_bytes) {   // 内存池剩余空间完全满足需要
            result = start_free;
            start_free = start_free + total_bytes;
            return result;
        } else if (bytes_left >= bytes) {  // 内存池剩余空间不能完全满足需要, 但足够供应一个或以上的区块
            nobjs = bytes_left / bytes;
            total_bytes = nobjs * bytes;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {                           // 内存池剩余空间连一个区块的大小都无法提供
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            if (bytes_left > 0) {          // 当前内存池还有一部分内存, 为了不浪费分配给free_list
                obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
                reinterpret_cast<obj *>( start_free)->next = *my_free_list;
                *my_free_list = reinterpret_cast<obj *>(start_free);
            }
            start_free = static_cast<char *>(malloc(bytes_to_get));
            if (!start_free) {
                obj **my_free_list, *p;
                for (int i = 0; i <= __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (p != nullptr) {   // 存在足以分配的区块
                        *my_free_list = p->next;
                        start_free = reinterpret_cast<char *>(p);
                        end_free = start_free + i;
                        return chunk_alloc(bytes, nobjs); // 递归调用以修正nobjs, 此时必然进入else_if分支
                    }
                }
                end_free = nullptr;       // 到处都找不到内存
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(bytes, nobjs);
        }
    }
}

#endif   // MINISTL_STL_ALLOC_IMP_H
