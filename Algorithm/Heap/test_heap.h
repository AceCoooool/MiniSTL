#ifndef MINISTL_TEST_HEAP_H
#define MINISTL_TEST_HEAP_H

#include "stl_heap.h"
#include <vector>
#include <iostream>

using namespace std;

namespace MiniSTL {
    void test_heap() {
        vector<int> ivec = {0, 1, 2, 3, 4, 8, 9, 3, 5};
        cout << "ivec = {0, 1, 2, 3, 4, 8, 9, 3, 5}" << endl;
        MiniSTL::make_heap(ivec.begin(), ivec.end());
        cout << "ivec(after make_heap(ivec.begin(), ivec.end()))=9 5 8 3 4 0 2 3 1?: ";
        for (auto t: ivec)
            cout << t << " ";
        cout << endl;

        ivec.push_back(7);
        MiniSTL::push_heap(ivec.begin(), ivec.end());
        cout << "ivec.push_back(7);" << endl;
        cout << "ivec(after push_heap(ivec.begin(), ivec.end()))=9 7 8 3 5 0 2 3 1 4?: ";
        for (auto t: ivec)
            cout << t << " ";
        cout << endl;

        MiniSTL::pop_heap(ivec.begin(), ivec.end());
        cout << "ivec.back(after pop_heap(ivec.begin(), ivec.end()))=9? : " << ivec.back() << endl;
        ivec.pop_back();

        cout << "ivec(after pop_heap(ivec.begin(), ivec.end()))=8 7 4 3 5 0 2 3 1?: ";
        for (auto t: ivec)
            cout << t << " ";
        cout << endl;

        MiniSTL::sort_heap(ivec.begin(), ivec.end());
        cout << "ivec(after sort_heap(ivec.begin(), ivec.end()))=0 1 2 3 3 4 5 7 8?: ";
        for (auto t: ivec)
            cout << t << " ";
        cout << endl;
    }

}

#endif //MINISTL_TEST_HEAP_H
