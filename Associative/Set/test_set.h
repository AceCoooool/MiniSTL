#ifndef MINISTL_TEST_SET_H
#define MINISTL_TEST_SET_H

#include "stl_set.imp.h"
#include <iostream>

using namespace std;

namespace MiniSTL {
    void test_set() {
        int ia[5] = {0, 1, 2, 3, 4};
        cout << "int ia[5] = {0, 1, 2, 3, 4}" << endl;
        MiniSTL::set<int> iset(ia, ia + 5);
        cout << "MiniSTL::set<int>(ia, ia + 5)" << endl;
        cout << "iset.size()=5?: " << iset.size() << endl;
        cout << "iset.count(3)=1?: " << iset.count(3) << endl;
        iset.insert(5);
        cout << "iset.size()(after insert 5)=6?: " << iset.size() << endl;
        iset.erase(1);
        cout << "iset.size()(after erase 1)=5?: " << iset.size() << endl;
        cout << "iset = 0 2 3 4 5?: ";
        for (auto t: iset)
            cout << t << " ";
        cout << endl;
        auto ite1 = iset.find(3);
        if (ite1 != iset.end())
            cout << "3 find" << endl;
        ite1 = iset.find(1);
        if (ite1 == iset.end())
            cout << "1 not find" << endl;
    }
}

#endif //MINISTL_TEST_SET_H
