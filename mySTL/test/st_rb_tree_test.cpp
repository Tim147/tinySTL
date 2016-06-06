#include "../include/st_rb_tree.h"
#include "../include/st_algorithm.h"
#include <iostream>

int main () {
    tinySTL::rb_tree<int, int, tinySTL::identity<int>, tinySTL::less<int> > rbtree; 
    rbtree.insert_equal (10);
    for (int i=0; i < 10; ++i) {
        rbtree.insert_equal(2*i);
    }
    
    return 1;
}

