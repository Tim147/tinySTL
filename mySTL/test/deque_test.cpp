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
    std::cout<<n<<std::endl;
    for (int i=0; i< n; ++i) 
        std::cout<<deq[i]<<std::endl;

    tinySTL::deque<int> foo (3,100);   // three ints with a value of 100
    tinySTL::deque<int> bar (2,200);   // two ints with a value of 200

    if (foo==bar) std::cout << "foo and bar are equal\n";
    if (foo!=bar) std::cout << "foo and bar are not equal\n";
    if (foo< bar) std::cout << "foo is less than bar\n";
    if (foo> bar) std::cout << "foo is greater than bar\n";
    if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
    if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";



    return 0;
}

