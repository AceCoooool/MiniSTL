#ifndef MINISTL_TEST_NUMERIC_H
#define MINISTL_TEST_NUMERIC_H

#include "stl_numeric.h"
#include <vector>
#include <iostream>

using namespace std;

void test_numeric() {
    vector<int> a = {1, 2, 3, 4};
    vector<int> c = {2, 3, 4, 5};
    cout << "a={1, 2, 3, 4}" << endl;
    cout << "c={2, 3, 4, 5}" << endl;
    auto res = MiniSTL::accumulate(a.begin(), a.end(), 0);
    cout << "accumulate(a.begin(), a.end(), 0) = 10 ?: " << res << endl;
    vector<int> b(5);
    auto last = MiniSTL::adjacent_difference(a.begin(), a.end(), b.begin());
    cout << "adjacent_difference(a.begin(), a.end(), b.begin()) = 1 1 1 1? ";
    for (auto tmp = b.begin(); tmp < last; ++tmp)
        cout << *tmp << " ";
    cout << endl;
    auto inner = MiniSTL::inner_product(a.begin(), a.end(), c.begin(), 0);
    cout << "inner_product(a.begin(), a.end(), c.begin(), 0) = 40?: " << inner << endl;
    auto last2 = MiniSTL::partial_sum(a.begin(), a.end(), b.begin());
    cout << "partial_sum(a.begin(), a.end(), b.begin()) = 1 3 6 10?: ";
    for (auto tmp = b.begin(); tmp < last2; ++tmp)
        cout << *tmp << " ";
    cout << endl;
    cout << "power(2, 4) = 16?: " << MiniSTL::power(2, 4) << endl;
    MiniSTL::itoa(b.begin(), b.end(), 5);
    cout << "itoa(b.begin(), b.end(), 5) = 5 6 7 8 9?: ";
    for (auto tmp: b)
        cout << tmp << " ";
    cout << endl;
}

#endif //MINISTL_TEST_NUMERIC_H
