#ifndef MINISTL_TEST_ALGOBASE_H
#define MINISTL_TEST_ALGOBASE_H

#include "stl_algobase.h"
#include <iostream>
#include <vector>
#include <deque>

using namespace std;

void test_algobase() {
    cout << "-----------------------min&max--------------------" << endl;
    int a = 10, b = 20;
    cout << "a=10, b=20" << endl;
    cout << "max(a, b)=20?: " << MiniSTL::max(a, b) << endl;
    cout << "min(a, b)=10?: " << MiniSTL::min(a, b) << endl;
    cout << "-----------------equal, fill, fill_n-----------------" << endl;
    vector<int> v1 = {1, 2, 3, 4};
    vector<int> v2 = {1, 2, 3, 4, 5};
    vector<int> v3 = {1, 3, 4, 5};
    cout << "v1 = {1, 2, 3, 4}" << endl;
    cout << "v2 = {1, 2, 3, 4, 5}" << endl;
    cout << "v3 = {1, 3, 4, 5}" << endl;
    cout << "equal(v1.begin(), v1.end(), v2.begin())=1? : "
         << MiniSTL::equal(v1.begin(), v1.end(), v2.begin()) << endl;
    cout << "equal(v1.begin(), v1.end(), v3.begin())=0? : "
         << MiniSTL::equal(v1.begin(), v1.end(), v3.begin()) << endl;
    MiniSTL::fill(v1.begin(), v1.end(), 6);
    cout << "fill(v1.begin(), v1.end(), 6)=6 6 6 6? : ";
    for (auto t: v1)
        cout << t << " ";
    cout << endl;
    auto out = MiniSTL::fill_n(v3.begin(), 2, 6);
    cout << "fill_n(v3.begin(), 2, 6)=6 6 4 5? : ";
    for (auto t: v3)
        cout << t << " ";
    cout << endl;
    cout << "-----------------swap & iter_swap-----------------" << endl;
    MiniSTL::swap(v2[1], v2[3]);
    cout << "v2(after swap(v2[1], v2[3]))=1 4 3 2 5? : ";
    for (auto t: v2)
        cout << t << " ";
    cout << endl;
    MiniSTL::iter_swap(v2.begin(), v2.begin() + 2);
    cout << "v2(after iter_swap(v2.begin(), v2.begin()+2))=3 4 1 2 5? : ";
    for (auto t: v2)
        cout << t << " ";
    cout << endl;
    cout << "-----------------compare&mismatch-----------------" << endl;
    vector<string> a1 = {"hello", "world", "nice"};
    vector<string> a2 = {"hello", "world", "ok"};
    vector<string> a3 = {"apple"};
    cout << "a1 = {\"hello\", \"world\", \"nice\"}" << endl;
    cout << "a2 = {\"hello\", \"world\"}" << endl;
    cout << "a3 = {\"apple\"}" << endl;
    cout << "lexicographical_compare(a1.begin(), a1.end(), a2.begin(), a2.end()=1? : "
         << MiniSTL::lexicographical_compare(a1.begin(), a1.end(), a2.begin(), a2.end()) << endl;
    cout << "lexicographical_compare(a1.begin(), a1.end(), a3.begin(), a3.end()=0? : "
         << MiniSTL::lexicographical_compare(a1.begin(), a1.end(), a3.begin(), a3.end()) << endl;
    auto res = MiniSTL::mismatch(a1.begin(), a1.end(), a2.begin(), a2.end());
    cout << "mismatch(a1.begin(), a1.end(), a2.begin(), a2.end())? nice, ok? : "
         << *get<0>(res) << " " << *get<1>(res) << endl;
    auto res2 = MiniSTL::mismatch(a1.begin(), a1.end(), a3.begin(), a3.end());
    cout << "mismatch(a1.begin(), a1.end(), a3.begin(), a3.end())? hello, apple? : "
         << *get<0>(res2) << " " << *get<1>(res2) << endl;
    cout << "-----------------copy&copy_backward-----------------" << endl;
    deque<int> l1 = {1, 2, 3, 4, 5};
    deque<int> l2 = {1, 1, 1, 1, 1};
    cout << "deque<int> l1 = {1, 2, 3, 4, 5}" << endl;
    cout << "deque<int> l2 = {1, 1, 1, 1, 1}" << endl;
    MiniSTL::copy(l1.begin(), l1.end(), l2.begin());
    cout << "l2(after copy(l1.begin(), l1.end(), l2.begin()))=1 2 3 4 5? : ";
    for (auto a: l2)
        cout << a << " ";
    cout << endl;
    MiniSTL::copy(l1.begin(), l1.begin() + 3, l1.begin() + 1);
    cout << "l1(after copy(l1.begin(), l1.begin()+3, l1.begin()+1))=1 1 1 1 5? : ";
    for (auto a: l1)
        cout << a << " ";
    cout << endl;

    vector<int> t1 = {1, 2, 3, 4, 5};
    cout << "vector<int> t1 = {1, 2, 3, 4, 5}" << endl;
    MiniSTL::copy_backward(t1.begin(), t1.begin() + 3, t1.end());
    cout << "t1(after copy_backward(t1.begin(), t1.begin() + 3, t1.end()))=1 2 1 2 3? : ";
    for (auto a:t1)
        cout << a << " ";
    cout << endl;

};

#endif //MINISTL_TEST_ALGOBASE_H
