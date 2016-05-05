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

template <class T, class Alloc = simple_alloc, size_t buf_size>
    class deque {
        private:
            typedef T   value_type;
            typedef T*  pointer;
            typedef const T*    const_pointer;
            typedef const T&    const_reference;
            typedef T&  reference;
            typedef size_t  size_type;
            typedef ptrdiff_t    difference_type;
        public:
            typedef deque_iterator<value_type, pointer, reference, buf_size> iterator;
            typedef deque_iterator<const T, const T*, const T&, buf_size> const_iterator;
        
        protected:
            typedef pointer* map_pointer;

        protected:
            iterator    start;
            iterator    finish;

            map_pointer map;
            size_type map_size;

        public:
            iterator begin () { return start; }
            const_iterator begin () const { return start; }
            const_iterator cbegin () const { return start; }

            iterator end () { return finish; }
            const_iterator end () const { return finish; }
            const_iterator cend () const { return finish; }

            reference operator[] (size_type n) {
                return start[difference_type(n)];
            }

            reference front () { return *start; }
            const_reference front () nconst { return *start; }

            reference back () { 
                iterator tmp = finish;
                --tmp;
                return *tmp;
            }
            const_reference back () { 
                iterator tmp = finish;
                --tmp;
                return *tmp;
            }

            size_type size () const { return (finish - start); }

            size_type max_size () const { return size_type(-1); }

            bool empty () const { return start == finish; }



            
    };

}

#endif
