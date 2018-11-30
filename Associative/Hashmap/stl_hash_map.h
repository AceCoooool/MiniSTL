#ifndef MINISTL_STL_HASH_MAP_H
#define MINISTL_STL_HASH_MAP_H

#include "../Hashtable/stl_hash.imp.h"
#include "../Hashtable/stl_hash_fun.h"
#include "../../Functor/stl_function.h"

namespace MiniSTL {
    template<class Key, class T, class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
    class hash_map {
        
    };
}

#endif //MINISTL_STL_HASH_MAP_H
