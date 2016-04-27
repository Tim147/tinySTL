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
        pointer operator-> () const { return cur; }

        difference_type operator- () {
            return difference_type (buffer_size() ) * (node - x.node - 1)
                    + (cur - first) + (x.last - x.cur);
        }

        self& operator++ () {
            ++cur; 
            if (cur == last) {
                set_node (node+1);
                cur = first;
            }
            return *this;
        }

        self& operator++ (int) {
            self tmp = *this;
            ++(*this);
            return tmp;
        }

        self& operator-- () {
            if (cur == first) {
                set_node (node-1);
                cur = last;
            }
            --cur;
            return *this;
        }

        self& operator-- (int) {
            self tmp = *this;
            --*this;
            return tmp;
        }

        self operator+= (difference_type size) {
            difference_type buf_size = buffer_size();
            difference_type slack = size - last + cur;
            difference_type buf_num = (slack + buf_size) / buf_size;
            if (buf_num > 0) {
                set_node (node + buf_num);
                cur = first + slack - (buf_num - 1) * buf_size;
            } else {
                cur += size;
            }
            return *this;
        }

        self operator+ (difference_type size) {
            self tmp = *this;
            tmp += size;
            return tmp;
        }

        self operator-= (difference_type size) {
            return (*this) += -size;
        }

        self operator- (difference_type size) {
            self tmp = *this;
            tmp -= size;
            return tmp;
        }

        reference operator[] (difference_type n) const {
            return *(*this + n);
        }

        bool operator== (const self& x) const { return cur==x.cur;}
        bool operator!= (const self& x) const { return cur != x.cur; }
        bool operator< (const self& x) const { return (x.node == node ? cur < x.cur : node < x.node); }
        bool operator<= (const self& x) const { return *this < x || *this == x; }
        bool operator> (const self& x) const { return !((*this == x) || (*this < x)); }
        bool operator>= ( const self& x) const { return *this > x || *this == x; }
};
    inline size_t _deque_buf_size (size_t n, size_t sz) {
        return n != 0 ? n : (sz < 521 ? size_t ( 512 / sz ) : size_t (1));
    }

}

#endif
