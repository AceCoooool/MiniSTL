#ifndef MINISTL_TEST_ALLOCATOR_H
#define MINISTL_TEST_ALLOCATOR_H

#include "allocator.h"
#include "stl_construct.h"
#include "stl_uninitialized.h"
#include <iostream>
#include <vector>

using namespace std;

void test_allocator() {
    cout << "-----test allocator-----" << endl;
    MiniSTL::allocator<int> alloc;
    int *a = alloc.allocate();
    alloc.construct(a, 10);
    cout << "a=10?: " << *a << endl;
    alloc.deallocate(a);
}

void test_uninitialized() {
    cout << "-----test uninitialized-----" << endl;
    vector<int> a(3);
    vector<int> b = {1, 2, 3};
    MiniSTL::uninitialized_copy(b.begin(), b.end(), a.begin());
    cout << "a={1, 2, 3}?: ";
    for (auto c: a)
        cout << c << " ";
    cout << endl;
    MiniSTL::uninitialized_fill(a.begin(), a.end(), 5);
    cout << "a={5, 5, 5}?: ";
    for (auto c: a)
        cout << c << " ";
    cout << endl;
    MiniSTL::uninitialized_fill_n(a.begin(), 2, 8);
    cout << "a={8, 8, 5}?: ";
    for (auto c: a)
        cout << c << " ";
    cout << endl;
}

#endif //MINISTL_TEST_ALLOCATOR_H
