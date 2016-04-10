#ifndef ST_ALGORITHM_H
#define ST_ALGORITHM_H

#include "st_iterator.h"

namespace tinySTL {

    template <class ForwardIterator, class Size, class T>
    ForwardIterator fill_n(ForwardIterator start, Size size, const T& x) {
        for(Size i=0; i<size; ++i)
            *(start+i) = x;
        return (start+size);
    }

    template <class ForwardIterator, class InputIterator>
    ForwardIterator copy(InputIterator start, InputIterator end, ForwardIterator result){
        for(int i=0;start + i != end; ++i, ++result) {
            *result = *(start+i);
        }
        return result;
    }

    template <class ForwardIterator, class T>
    ForwardIterator fill(ForwardIterator start, ForwardIterator end, const T& x) {
        for(;start != end; ++start)
            *start = x;
        return start;
    }

    template <class BiDirectionIterator>
    BiDirectionIterator 
    backward_copy(BiDirectionIterator first, BiDirectionIterator last, BiDirectionIterator result) {
        --last;
        for(;last != first - 1; --last, --result)
            *result = *last;
        return result;
    }




}








#endif
