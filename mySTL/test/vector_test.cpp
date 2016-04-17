#include "./include/st_vector.h"
#include <iostream>

int main() {

    tinySTL::vector<int> vec(10,9);
    const tinySTL::vector<int> vec1(8,8);
    vec = vec1;
    tinySTL::vector<int> vec2(vec1);

    tinySTL::vector<int>::iterator it = vec2.insert(vec2.begin(), 4);
    vec2.insert(it, (size_t)3, 5);
    vec2.insert(vec2.begin()+2, vec1.begin(),vec1.end());

    vec2.push_back(67);
    vec2.pop_back();

    vec2.erase(vec2.begin()+2);
    vec2.swap(vec);

    std::cout<<vec2.capacity()<<std::endl;
    
    vec2.reserve(40);
    vec2.shrink_to_fit();


    vec2.resize(18);

    vec2.assign((size_t)10,20);
    std::cout<<vec2.size()<<std::endl;
   // std::cout<<*(vec.end()-1)<<std::endl;
   // for(int i=0; i<vec.size(); ++i)
   //     std::cout<<vec[i]<<"\t";
   // std::cout<<std::endl;
    for(int i=0; i<vec2.size(); ++i) {
        std::cout<<vec2[i]<<"\t";
    }
    std::cout<<std::endl;
    return 1 ;

}


