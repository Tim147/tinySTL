#include "./include/st_vector.h"
#include <iostream>

int main() {

    tinySTL::vector<int> vec(10,9);
    std::cout<<*(vec.end()-1)<<std::endl;
    for(int i=0; i<vec.size(); ++i)
        std::cout<<vec[i]<<std::endl;
    return 1 ;

}


