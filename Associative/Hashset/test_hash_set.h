#ifndef MINISTL_TEST_HASH_SET_H
#define MINISTL_TEST_HASH_SET_H

#include "stl_hash_set.h"
#include "stl_hash_multiset.h"
#include <iostream>
#include <cstring>

using namespace std;

namespace MiniSTL {
    struct eqstr {
        bool operator()(const char *s1, const char *s2) const {
            return strcmp(s1, s2) == 0;
        }
    };

    void lookup(const MiniSTL::hash_set<const char *, hash<const char *>, eqstr> &Set, const char *word) {
        auto it = Set.find(word);
        cout << " " << word << ": " << (it != Set.end() ? "present" : "not present") << endl;
    }

    void lookup(const MiniSTL::hash_multiset<const char *, hash<const char *>, eqstr> &Set, const char *word) {
        auto it = Set.find(word);
        cout << " " << word << ": " << (it != Set.end() ? "present" : "not present") << endl;
    }

    void test_hast_set() {
        MiniSTL::hash_set<const char *, hash<const char *>, eqstr> Set;
        Set.insert("kiwi");
        Set.insert("plum");
        Set.insert("apple");
        Set.insert("mango");
        Set.insert("apricot");
        Set.insert("banana");

        lookup(Set, "mango");
        lookup(Set, "apple");
        lookup(Set, "durian");

        auto ite1 = Set.begin();
        auto ite2 = Set.end();
        for (; ite1 != ite2; ++ite1)
            cout << *ite1 << " ";
        cout << endl;
    }

    void test_hast_multiset() {
        MiniSTL::hash_multiset<const char *, hash<const char *>, eqstr> Set;
        Set.insert("kiwi");
        Set.insert("plum");
        Set.insert("apple");
        Set.insert("mango");
        Set.insert("apricot");
        Set.insert("banana");

        lookup(Set, "mango");
        lookup(Set, "apple");
        lookup(Set, "durian");

        auto ite1 = Set.begin();
        auto ite2 = Set.end();
        for (; ite1 != ite2; ++ite1)
            cout << *ite1 << " ";
        cout << endl;
    }

}

#endif //MINISTL_TEST_HASH_SET_H
