#ifndef MINISTL_TEST_HASH_H
#define MINISTL_TEST_HASH_H

#include "stl_hash.imp.h"
#include <iostream>
#include "../../Functor/stl_function.h"

using namespace std;

namespace MiniSTL {
    void test_hash() {
        MiniSTL::hashtable<int, int, hash<int>, MiniSTL::identity<int>, equal_to<int>, alloc>
            iht(50, hash<int>(), equal_to<int>());
        cout << "iht.size()=0?: " << iht.size() << endl;
        cout << "iht.bucket_count()=53?: " << iht.bucket_count() << endl;
        cout << "iht.max_bucket_count()=4294967291?: " << iht.max_bucket_count() << endl;

        iht.insert_unique(59);
        iht.insert_unique(63);
        iht.insert_unique(108);
        iht.insert_unique(2);
        iht.insert_unique(53);
        iht.insert_unique(55);
        cout << "iht.size()(after insert_unique 59 63 108 2 53 55)=6?: " << iht.size() << endl;

        cout << "iht=53 55 2 108 59 63?: ";
        for (auto i: iht)
            cout << i << " ";
        cout << endl;
        for (int i = 0; i < iht.bucket_count(); ++i) {
            int n = iht.elems_in_bucket(i);
            if (n != 0)
                cout << "bucket[" << i << "] has " << n << " elems." << endl;
        }

        for (int i = 0; i <= 47; ++i) {
            iht.insert_equal(i);
        }
        cout << "iht.size()(after insert_equal 0~47)=54? " << iht.size() << endl;
        cout << "iht.bucket_count()=97?: " << iht.bucket_count() << endl;
        for (int i = 0; i < iht.bucket_count(); ++i) {
            int n = iht.elems_in_bucket(i);
            if (n != 0)
                cout << "bucket[" << i << "] has " << n << " elems." << endl;
        }

        for (auto i: iht)
            cout << i << " ";
        cout << endl;
        cout << "*(iht.find(2))=2?: " << *(iht.find(2)) << endl;
        cout << "iht.count(2)=2?: " << iht.count(2) << endl;
    }
}

#endif //MINISTL_TEST_HASH_H
