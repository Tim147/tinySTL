#include "../include/st_vector.h"
#include <iostream>

int main() {

    tinySTL::vector<int> vec(10,9);
    std::cout<<*(vec.end()-1)<<std::endl;
    return 1 ;

}


