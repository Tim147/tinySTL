#ifndef ST_DEQUE_H
#define ST_DEQUE_H

#include "../include/st_iterator.h"

namespace tinySTL {

template <class T, class Ptr, class Ref, size_t sizeBuf>
class deque_iterator {
    protected:
        typedef deque_iterator<T, T*, T&, sizeBuf> iterator;
        typedef deque_iterator<const T, const T*, const T&, sizeBuf> const_iterator;

        static size_t buffer_size () { return _deque_buf_size (sizeBuf, sizeof(T)); }

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

    public:

        void set_node (map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type (buffer_size());
        }

        reference operator* () const { return *cur; }
};
    inline size_t _deque_buf_size (size_t n, size_t sz) {
        return n != 0 ? n : (sz < 521 ? size_t ( 512 / sz ) : size_t (1));
    }

}

#endif
