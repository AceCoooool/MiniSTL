#ifndef MINISTL_TEST_SLIST_H
#define MINISTL_TEST_SLIST_H

#include "stl_slist.imp.h"
#include <iostream>
#include <algorithm>

#include <vector>

using namespace std;

void test_slist() {
    MiniSTL::slist<int> islist;
    cout << "size(empty list)=0?: " << islist.size() << endl;
    islist.push_front(9);
    islist.push_front(1);
    islist.push_front(2);
    islist.push_front(3);
    islist.push_front(4);
    cout << "size(after push_front 9 1 2 3 4)=5?: " << islist.size() << endl;
    cout << "slist = 4 3 2 1 9?: ";
    auto ite = islist.begin();
    auto ite2 = islist.end();
    for (; ite != ite2; ++ite)
        cout << *ite << " ";
    cout << endl;

//    find(islist.begin(), islist.end(), 2);  // TODO: bug
    islist.insert(islist.begin(), 99);
    for (auto i = islist.begin(); i != islist.end(); ++i)
        cout << *i << " ";
    cout << endl;


}

#endif //MINISTL_TEST_SLIST_H
