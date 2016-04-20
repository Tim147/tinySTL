#include "../include/st_list.h"
#include <iostream>

int main () {
    tinySTL::list<int> lst1, lst2;
    for (int i=0; i<10; ++i) {
        lst1.push_back(2*i);
        lst2.push_back(2*i+1);
    }
    lst1.merge(lst2);
    lst1.remove(10);
    lst1.resize(15);
    //lst1.insert(lst1.end(), (size_t)10,9);
    lst1.resize(20,9);
    lst1.reverse();
    tinySTL::list<int>::iterator it = lst1.begin();
    for (; it != lst1.end(); ++it) 
        std::cout<<*it<<"\t";
    std::cout<<std::endl;
    return 1;
}
