#ifndef ST_DEQUE_H
#define ST_DEQUE_H

#include "../include/st_iterator.h"

namespace tinySTL {

template <class T, class Ptr, class Ref, size_t sizeBuf>
class deque_iterator {
    protected:
        typedef deque_iterator<T, T*, T&, sizeBuf> iterator;
        typedef deque_iterator<const T, const T*, const T&, sizeBuf> const_iterator;

        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Ptr                         pointer;
        typedef Ref                         reference;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;
        typedef T**                         map_pointer;

        typedef deque_iterator  self;
    private:
        T*  cur;
        T* first;
        T* last;

        map_pointer node;
};

}

#endif
