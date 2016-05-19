#include <iostream> 
#include "../include/st_deque.h"

int main () {
    tinySTL::deque<int> deq(3,4);
    deq.push_back (10);
    deq.push_front (7);

    tinySTL::deque<int>::iterator it = deq.erase(deq.begin()+2);
    deq.insert(it, 100);

    deq.pop_front();
    deq.pop_back();
    int n = deq.size();
    for (int i=0; i< n; ++i) 
        std::cout<<deq[i]<<std::endl;


    return 0;
}

