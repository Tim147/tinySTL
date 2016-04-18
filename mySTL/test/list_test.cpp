#include "../include/st_list.h"
#include <iostream>

int main() {
    tinySTL::list<int> mylist;
    mylist.insert(mylist.begin(), 2);
    tinySTL::list<int>::iterator it;
    mylist.push_back(100);
    
    it = mylist.begin();
    mylist.erase(it);
    it = mylist.begin();
    int value = *it;

    std::cout<<value<<std::endl;
    return 0;
}



