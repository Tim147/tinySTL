#include "../include/st_list.h"
#include <iostream>

int main() {
    tinySTL::list<int> mylist;
    tinySTL::list<int> mylist2;
    for (int i=0; i<10; ++i) {
        mylist2.push_back(i);
    }
    mylist.insert(mylist.begin(), 2);
    tinySTL::list<int>::iterator it;
    mylist.push_back(100);
    
    it = mylist.begin();
    mylist.erase(it);
    it = mylist.begin();
    int value = *it;
    std::cout<<value<<std::endl;
    mylist2.assign(mylist.begin(), mylist.end());
    mylist2.assign((size_t)10,3);
    mylist2.pop_back();
    mylist2.push_back(2);
    mylist2.pop_front();
    mylist2.push_front(4);
    //mylist2.clear();
    //mylist2.erase(mylist2.begin(), mylist2.end());
    for (it = mylist2.begin(); it != mylist2.end(); ++it) 
        std::cout<<*it<<"\t";
    std::cout<<std::endl;
    return 0;
}



