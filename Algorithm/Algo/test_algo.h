#ifndef MINISTL_TEST_ALGO_H
#define MINISTL_TEST_ALGO_H

#include "stl_algo.h"
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

class even_by_two {
public:
    int operator()() const {
        return x += 2;
    }
private:
    static int x;
};

auto display = [](int x) { cout << x << " "; };
auto even = [](int x) { return x % 2 ? false : true; };


int even_by_two::x = 0;

void test_algo1() {
//    int ia[] = {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
//    vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));
//    cout << "iv = {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8}" << endl;
//    cout << "adjacent_find(iv.begin(), iv.end())=6?: " << *MiniSTL::adjacent_find(iv.begin(), iv.end());
//    cout << "count(iv.begin(), iv.end(), 4)=3?: " << MiniSTL::count(iv.begin(), iv.end(), 4) << endl;
//    cout << "count_if(iv.begin(), iv.end(), [](int x) { return x < 7; }=9?: "
//         << MiniSTL::count_if(iv.begin(), iv.end(), [](int x) { return x < 7; }) << endl;
//    cout << "*MiniSTL::find(iv.begin(), iv.end(), 4)=4?: " << *MiniSTL::find(iv.begin(), iv.end(), 4) << endl;
//    cout << "*MiniSTL::find_if(iv.begin(), iv.end(), [](int x) { return x > 2; }=3?: "
//         << *MiniSTL::find_if(iv.begin(), iv.end(), [](int x) { return x > 2; }) << endl;
//    vector<int> iv2(ia + 6, ia + 8);
//    cout << "iv2 = {6, 6}" << endl;
//    cout << "*(MiniSTL::find_end(iv.begin(), iv.end(), iv2.begin(), iv2.end())+3)=8?: "
//         << *(MiniSTL::find_end(iv.begin(), iv.end(), iv2.begin(), iv2.end()) + 3) << endl;
//    cout << " *(MiniSTL::find_first_of(iv.begin(), iv.end(), iv2.begin(), iv2.end())+3)=7?: "
//         << *(MiniSTL::find_first_of(iv.begin(), iv.end(), iv2.begin(), iv2.end()) + 3) << endl;
//    cout << "MiniSTL::for_each(iv.begin(), iv.end(), [](int x){cout<<x<<\" \";})=0 1 2 3 4 5 6 6 6 7 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//    MiniSTL::generate(iv2.begin(), iv2.end(), even_by_two());
//    cout << "iv2(MiniSTL::generate(iv2.begin(), iv2.end(), even_by_two()))=2 4?: ";
//    MiniSTL::for_each(iv2.begin(), iv2.end(), display);
//    cout << endl;
//    MiniSTL::generate_n(iv.begin(), 3, even_by_two());
//    cout << "iv(after MiniSTL::generate_n(iv.begin(), 3, even_by_two()))=6 8 10 3 4 5 6 6 6 7 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//    MiniSTL::remove(iv.begin(), iv.end(), 6);
//    cout << "iv(after MiniSTL::remove(iv.begin(), iv.end(), 6))=8 10 3 4 5 7 8 6 6 7 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//    vector<int> iv3(12);
//    MiniSTL::remove_copy(iv.begin(), iv.end(), iv3.begin(), 6);
//    cout << "iv3(after MiniSTL::remove_copy(iv.begin(), iv.end(), iv3.begin(), 3))=8 10 3 4 5 7 8 7 8 0 0 0?: ";
//    MiniSTL::for_each(iv3.begin(), iv3.end(), display);
//    cout << endl;
//    MiniSTL::remove_if(iv.begin(), iv.end(), [](int x) { return x < 6; });
//    cout << "iv(after MiniSTL::remove_if(iv.begin(), iv.end(), [](int x) { return x < 6; }))=8 10 7 8 6 6 7 8 6 7 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//    MiniSTL::remove_copy_if(iv.begin(), iv.end(), iv3.begin(), [](int x) { return x < 7; });
//    cout << "iv3(after MiniSTL::remove_copy_if(iv.begin(), iv.end(), iv3.begin(), [](int x) { return x < 7; }))="
//            "8 10 7 8 7 8 7 8 8 0 0 0?: ";
//    MiniSTL::for_each(iv3.begin(), iv3.end(), display);
//    cout << endl;
//    MiniSTL::replace(iv.begin(), iv.end(), 6, 3);
//    cout << "iv(after MiniSTL::replace(iv.begin(), iv.end(), 6, 3))=8 10 7 8 3 3 7 8 3 7 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//    MiniSTL::replace_copy(iv.begin(), iv.end(), iv3.begin(), 3, 5);
//    cout << "iv3(after MiniSTL::replace_copy(iv.begin(), iv.end(), iv3.begin(), 3, 5))="
//            "8 10 7 8 5 5 7 8 5 7 8 0?: ";
//    MiniSTL::for_each(iv3.begin(), iv3.end(), display);
//    cout << endl;
//
//    MiniSTL::replace_if(iv.begin(), iv.end(), [](int x) { return x < 5; }, 2);
//    cout << "iv(after MiniSTL::replace_if(iv.begin(), iv.end(), [](int x) { return x < 5; }, 2))="
//            "8 10 7 8 2 2 7 8 2 7 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//    MiniSTL::replace_copy_if(iv.begin(), iv.end(), iv3.begin(), [](int x) { return x == 8; }, 9);
//    cout << "iv3(after MiniSTL::replace_copy_if(iv.begin(), iv.end(), iv3.begin(), [](int x) { return x == 8; }, 9)="
//            "9 10 7 9 2 2 7 9 2 7 9 0?: ";
//    MiniSTL::for_each(iv3.begin(), iv3.end(), display);
//    cout << endl;
//
//    MiniSTL::reverse(iv.begin(), iv.end());
//    cout << "iv(after  MiniSTL::reverse(iv.begin(), iv.end()))=8 7 2 8 7 2 2 8 7 10 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//    MiniSTL::reverse_copy(iv.begin(), iv.end(), iv3.begin());
//    cout << "iv3(after MiniSTL::reverse_copy(iv.begin(), iv.end(), iv3.begin()))="
//            "8 10 7 8 2 2 7 8 2 7 8 0?: ";
//    MiniSTL::for_each(iv3.begin(), iv3.end(), display);
//    cout << endl;
//
//    MiniSTL::rotate(iv.begin(), iv.begin() + 4, iv.end());
//    cout << "iv(after MiniSTL::rotate(iv.begin(), iv.begin()+4, iv.end()))=7 2 2 8 7 10 8 8 7 2 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//
//    MiniSTL::rotate_copy(iv.begin(), iv.begin() + 5, iv.end(), iv3.begin());
//    cout << "iv3(after MiniSTL::rotate_copy(iv.begin(), iv.begin() + 5, iv.end(), iv3.begin()))="
//            "10 8 8 7 2 8 7 2 2 8 7 0?: ";
//    MiniSTL::for_each(iv3.begin(), iv3.end(), display);
//    cout << endl;
//
//    int ia2[3] = {2, 8};
//    vector<int> iv4(ia2, ia2 + 2);
//    cout << "iv4 = {2, 8}" << endl;
//    auto res = MiniSTL::search(iv.begin(), iv.end(), iv4.begin(), iv4.end());
//    cout << "*MiniSTL::search(iv.begin(), iv.end(), iv4.begin(), iv4.end())=2?: " << *res << endl;
//
//    res = MiniSTL::search_n(iv.begin(), iv.end(), 2, 8);
//    cout << "*MiniSTL::search_n(iv.begin(), iv.end(), 2, 8)=8?: " << *res << endl;
//    res = MiniSTL::search_n(iv.begin(), iv.end(), 3, 8, std::less<int>());
//    cout << "*MiniSTL::search_n(iv.begin(), iv.end(), 3, 8, std::less<int>())=7?: " << *res << endl;
//
//    MiniSTL::swap_ranges(iv4.begin(), iv4.end(), iv.begin());
//    cout << "iv(after MiniSTL::swap_ranges(iv4.begin(), iv4.end(), iv.begin()))="
//            "2 8 2 8 7 10 8 8 7 2 8?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//
//    MiniSTL::transform(iv.begin(), iv.end(), iv.begin(), [](int x) { return x - 2; });
//    cout << "iv(after MiniSTL::transform(iv.begin(), iv.end(), iv.begin(), [](int x) { return x - 2; }))="
//            "0 6 0 6 5 8 6 6 5 0 6?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//
//    MiniSTL::transform(iv.begin(), iv.end(), iv.begin(), iv.begin(), std::plus<int>());
//    cout << "iv(after MiniSTL::transform(iv.begin(), iv.end(), iv.begin(), iv.begin(), std::plus<int>()))="
//            "0 12 0 12 10 16 12 12 10 0 12?: ";
//    MiniSTL::for_each(iv.begin(), iv.end(), display);
//    cout << endl;
//
//    cout << "----------------------------------------------------" << endl;
//    vector<int> iv5(ia, ia + sizeof(ia) / sizeof(int));
//    vector<int> iv6(ia + 4, ia + 8);
//    vector<int> iv7(15);
//    cout << "iv5=";
//    for_each(iv5.begin(), iv5.end(), display);
//    cout << endl;
//    cout << "iv6=";
//    for_each(iv6.begin(), iv6.end(), display);
//    res = MiniSTL::max_element(iv5.begin(), iv5.end());
//    cout << "*MiniSTL::max_element(iv5.begin(), iv5.end())=8?: " << *res << endl;
//    res = MiniSTL::min_element(iv5.begin(), iv5.end());
//    cout << "*MiniSTL::min_element(iv5.begin(), iv5.end())=0?: " << *res << endl;
//    auto flag = MiniSTL::includes(iv5.begin(), iv5.end(), iv6.begin(), iv6.end());
//    cout << "MiniSTL::includes(iv5.begin(), iv5.end(), iv6.begin(), iv6.end())=1?: " << flag << endl;
//
//    MiniSTL::merge(iv5.begin(), iv5.end(), iv6.begin(), iv6.end(), iv7.begin());
//    cout << "iv7(after MiniSTL::merge(iv5.begin(), iv5.end(), iv6.begin(), iv6.end(), iv7.begin()))="
//            "0 1 2 3 4 4 5 5 6 6 6 6 6 7 8?: ";
//    MiniSTL::for_each(iv7.begin(), iv7.end(), display);
//    cout << endl;
//
//    MiniSTL::partition(iv7.begin(), iv7.end(), even);
//    cout << "iv7(after MiniSTL::partition(iv7.begin(), iv7.end(), even))="
//            "0 8 2 6 4 4 6 6 6 6 5 5 3 7 1?: ";
//    MiniSTL::for_each(iv7.begin(), iv7.end(), display);
//    cout << endl;
//
//    MiniSTL::unique(iv5.begin(), iv5.end());
//    cout << "iv5(after MiniSTL::unique(iv5.begin(), iv5.end()))=0 1 2 3 4 5 6 7 8 7 8?: ";
//    MiniSTL::for_each(iv5.begin(), iv5.end(), display);
//    cout << endl;
//
//    MiniSTL::unique_copy(iv5.begin(), iv5.end(), iv7.begin());
//    cout<<"iv7(after MiniSTL::unique_copy(iv5.begin(), iv5.end(), iv7.begin()))="
//          "0 1 2 3 4 5 6 7 8 7 8 5 3 7 1?: ";
//    MiniSTL::for_each(iv7.begin(), iv7.end(), display);
//    cout << endl;

};

void test_algo2() {
    cout << "------------------------------6.7.2~6.7.12---------------------------" << endl;
    int ia[] = {12, 17, 20, 22, 23, 30, 33, 40};
    vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));
    cout << "iv={12, 17, 20, 22, 23, 30, 33, 40}" << endl;

    auto res = MiniSTL::lower_bound(iv.begin(), iv.end(), 21);
    cout << "*MiniSTL::lower_bound(iv.begin(), iv.end(), 21)=22?: " << *res << endl;
    res = MiniSTL::upper_bound(iv.begin(), iv.end(), 21);
    cout << "*MiniSTL::upper_bound(iv.begin(), iv.end(), 21)=22?: " << *res << endl;
    res = MiniSTL::lower_bound(iv.begin(), iv.end(), 22);
    cout << "*MiniSTL::lower_bound(iv.begin(), iv.end(), 22)=22?: " << *res << endl;
    res = MiniSTL::upper_bound(iv.begin(), iv.end(), 22);
    cout << "*MiniSTL::upper_bound(iv.begin(), iv.end(), 22)=23?: " << *res << endl;

    auto flag = MiniSTL::binary_search(iv.begin(), iv.end(), 33);
    cout << "MiniSTL::binary_search(iv.begin(), iv.end(), 33)=1(true)?: " << flag << endl;
    flag = MiniSTL::binary_search(iv.begin(), iv.end(), 34);
    cout << "MiniSTL::binary_search(iv.begin(), iv.end(), 34)=0(false)?: " << flag << endl;

    MiniSTL::next_permutation(iv.begin(), iv.end());
    cout << "iv(after MiniSTL::next_permutation(iv.begin(), iv.end()))=12 17 20 22 23 30 40 33?: ";
    MiniSTL::for_each(iv.begin(), iv.end(), display);
    cout << endl;

    MiniSTL::prev_permutation(iv.begin(), iv.end());
    cout << "iv(after MiniSTL::prev_permutation(iv.begin(), iv.end()))=12 17 20 22 23 30 33 40?: ";
    MiniSTL::for_each(iv.begin(), iv.end(), display);
    cout << endl;

    random_shuffle(iv.begin(), iv.end());
    cout << "iv(after random_shuffle)=???: ";
    MiniSTL::for_each(iv.begin(), iv.end(), display);
    cout << endl;

    vector<int> iv2 = {33, 12, 30, 20, 17, 23, 22, 40};
    cout << "iv2 = {33, 12, 30, 20, 17, 23, 22, 40}" << endl;

    MiniSTL::partial_sort(iv2.begin(), iv2.begin() + 4, iv2.end());
    cout << "iv2(after MiniSTL::partial_sort(iv2.begin(), iv2.begin()+4, iv2.end()))=12 17 20 22 33 30 23 40?: ";
    MiniSTL::for_each(iv2.begin(), iv2.end(), display);
    cout << endl;

    MiniSTL::sort(iv2.begin(), iv2.end());
    cout << "iv2(after MiniSTL::sort(iv2.begin(), iv2.end()))=12 17 20 22 23 30 33 40?: ";
    MiniSTL::for_each(iv2.begin(), iv2.end(), display);
    cout << endl;

    vector<int> iv3 = {1, 3, 5, 7, 2, 4, 6, 8, 10};
    MiniSTL::inplace_merge(iv3.begin(), iv3.begin()+4, iv3.end());
    MiniSTL::for_each(iv3.begin(), iv3.end(), display);
    cout << endl;

    vector<int> iv4 = {22, 30, 30, 17};
    MiniSTL::mergesort(iv4.begin(), iv4.end());


//    MiniSTL::nth_element(iv4.begin(), iv4.begin()+1, iv4.end());
    MiniSTL::for_each(iv4.begin(), iv4.end(), display);


};

#endif //MINISTL_TEST_ALGO_H
