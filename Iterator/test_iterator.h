#ifndef MINISTL_TEST_ITERATOR_H
#define MINISTL_TEST_ITERATOR_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include "iterator_adapters.h"

using namespace std;

namespace MiniSTL {
    auto display = [](int x) { cout << x << " "; };

    void test_iterator_adapters() {
        cout << "---------------------insert iterator----------------------" << endl;
        deque<int> id = {0, 1, 2, 3, 4, 5};
        cout << "id(deque)={0, 1, 2, 3, 4, 5}" << endl;
        auto iter_b = MiniSTL::back_inserter(id);
        iter_b = 1;
        cout << "id(after auto iter_b = MiniSTL::back_inserter(id); iter_b = 1)=0 1 2 3 4 5 1?: ";
        for_each(id.begin(), id.end(), display);
        cout << endl;
        auto iter_f = MiniSTL::front_inserter(id);
        iter_f = 1;
        cout << "id(after auto iter_f = MiniSTL::front_inserter(id); iter_f = 1)=1 0 1 2 3 4 5 1?: ";
        for_each(id.begin(), id.end(), display);
        cout << endl;

        vector<int> v = {1, 2, 3, 4, 5, 6};
        cout << "vector<int> v = {1, 2, 3, 4, 5, 6}" << endl;
        auto iter_i = MiniSTL::inserter(v, v.begin() + 2);
        iter_i = 2;
        cout << "v(after auto iter_i = MiniSTL::inserter(v, v.begin()+2), iter_i = 2)=1 2 2 3 4 5 6?: ";
        for_each(v.begin(), v.end(), display);
        cout << endl;
        cout << "---------------------reverse iterator----------------------" << endl;
        vector<int> v1 = {1, 2, 3, 4, 5};
        cout << "vector<int> v1 = {1, 2, 3, 4, 5}" << endl;
        auto rbegin = MiniSTL::reverse_iterator(v1.end());
        auto rend = MiniSTL::reverse_iterator(v1.begin());
        cout << "(v1.rbegin, v1.rend)=5 4 3 2 1?: ";
        for_each(rbegin, rend, display);
        cout << endl;
        cout << "---------------------iostream iterator----------------------" << endl;
        cout << "please input 10: " << endl;
        auto iiter = MiniSTL::istream_iterator<int>();
        iiter++;
        cout << "iiter(after input 10)=10?: " << *iiter << endl;
        vector<int> v2 = {1, 2, 3, 4, 5};
        MiniSTL::ostream_iterator<int> oiter(cout, ",");
        cout << "print v2(with , as blank character) = 1,2,3,4,5?: ";
        copy(v2.begin(), v2.end(), oiter);
        cout << endl;
    }
}

#endif //MINISTL_TEST_ITERATOR_H
