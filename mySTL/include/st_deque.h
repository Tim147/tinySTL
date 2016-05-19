#ifndef ST_DEQUE_H
#define ST_DEQUE_H

#include "st_iterator.h"
#include "st_allocator.h"
#include "st_algorithm.h"
#include "st_uninitialled.h"

namespace tinySTL {

inline size_t _deque_buf_size (size_t sz) {
    return sz < 521 ? size_t ( 512 / sz ) : size_t (1);
}
template <class T, class Ptr, class Ref>
class deque_iterator {
    protected:
        typedef deque_iterator<T, T*, T&> iterator;
        typedef deque_iterator<const T, const T*, const T&> const_iterator;

        static size_t buffer_size () { return _deque_buf_size (sizeof(T)); }

        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Ptr                         pointer;
        typedef Ref                         reference;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;
        typedef T**                         map_pointer;

        typedef deque_iterator  self;
    public:
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

        difference_type operator- (self x) {
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
            difference_type slack = size - (last - cur);
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

        reference operator[] (difference_type n) {
            return *(*this + n);
        }
           
        bool operator== (const self& x) const { return cur==x.cur;}
        bool operator!= (const self& x) const { return cur != x.cur; }
        bool operator< (const self& x) const { return (x.node == node ? cur < x.cur : node < x.node); }
        bool operator<= (const self& x) const { return *this < x || *this == x; }
        bool operator> (const self& x) const { return !((*this == x) || (*this < x)); }
        bool operator>= ( const self& x) const { return *this > x || *this == x; }
};

template <class T, class Alloc = SimpleAlloc>
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
            typedef deque_iterator<value_type, pointer, reference> iterator;
            typedef deque_iterator<const T, const T*, const T&> const_iterator;
        
        protected:
            typedef pointer* map_pointer;

        protected:
            iterator    start;
            iterator    finish;

            map_pointer map;
            size_type map_size;
            
            simple_alloc<value_type, Alloc>   data_allocator;
            simple_alloc<pointer, Alloc>      map_allocator;
        private:

            size_type buf_size () const { return _deque_buf_size (sizeof (T)); }

            pointer allocate_node () {
                return data_allocator.allocate(buf_size());
            }
            //deallocate a node buffer
            void deallocate_node (pointer buff_ptr) {
                data_allocator.deallocate (buff_ptr);
            }

            void create_map_and_node (const size_type& num_elems) {
                size_type node_num = num_elems / buf_size() + 1;
                
                map_size = max((size_type)8, node_num+2);
                map = map_allocator.allocate(map_size);

                map_pointer nstart = map + (map_size - node_num)/2;
                map_pointer nend = nstart + node_num - 1;

                for (map_pointer cur = nstart; cur != nend + 1; ++cur)
                    *cur = allocate_node();

                start.set_node (nstart);
                finish.set_node (nend);

                start.cur = start.first;
                finish.cur = finish.first + num_elems % buf_size();
            }
            // iniilize n nodes
            void fill_initiallize (size_type n, const value_type& value) {
                create_map_and_node (n);

                for (map_pointer tcur = start.node; tcur != finish.node; ++tcur) 
                   uninitialled_fill (*tcur, *tcur + buf_size(), value);
                uninitialled_fill (finish.first, finish.cur, value);
            }

            void reallocate_map (size_type node_to_add, bool add_front) {
                size_type old_node_num = finish.node - start.node + 1;
                size_type new_node_num = old_node_num + node_to_add;
                map_pointer new_start;
                if (map_size > 2 * new_node_num) {
                    new_start = map + (map_size - new_node_num) / 2
                                    + (add_front?0:node_to_add);
                    if (new_start < start.node)
                        copy (start.node, finish.node + 1, new_start);
                    else
                        backward_copy (start.node, finish.node+1, new_start);
                } else {
                    size_type new_map_size = map_size + max (map_size, new_node_num) + 2;
                    map_pointer new_map = map_allocator.allocate (new_map_size);
                    new_start = new_map + (new_map_size - new_node_num) / 2
                                        + (add_front ? 0 : node_to_add);
                    copy (start.node, finish.node+1, new_start);
                    map_allocator.deallocate(map);
                    map = new_map;
                    map_size = new_map_size;
                }
                start.set_node(new_start);
                finish.set_node (new_start + old_node_num - 1);
            }

            void reserve_map_at_back (size_type node_to_add = 1) {
                if (node_to_add + 1 > map_size - (finish.node - start.node + 1) )
                    reallocate_map (node_to_add, false);
            }

            void reserve_map_at_front (size_type node_to_add = 1) {
                if (node_to_add > start.node - map) 
                    reallocate_map (node_to_add, true);
            }
             void push_back_aux (const value_type& value) {
                 reserve_map_at_back ();
                 *(finish.node + 1) = data_allocator.allocate ();
                 construct (finish.cur, value);
                 finish.set_node (finish.node + 1);
                 finish.cur = finish.first;
             }

             void push_front_aux ( const value_type& value) {
                reserve_map_at_front ();
                *(start.node - 1) = data_allocator.allocate ();
                start.set_node (start.node-1);
                start.cur = start.last - 1;
                construct (start.cur, value);
             }
             
             void pop_back_aux () {
                deallocate_node (finish.first);
                finish.set_node (finish.node -1 );
                finish.cur = finish.last - 1;
                destroy (finish.cur);
             }

             void pop_front_aux () {
                destroy (start.cur);
                deallocate_node (start.first);
                start.set_node (start.node + 1);
                start.cur = start.first;
             }

             iterator insert_aux (iterator pos, const value_type& val) {
                difference_type index = pos - start;

                if (index < (size()>>1) ) {
                    push_front(front());
                    iterator old_front = start;
                    ++old_front;
                    pos = start + index; // update pos in case for overflow of map
                    iterator pos1 = pos;
                    ++pos1;
                    copy (old_front, pos1, start);
                } else {
                    push_back(back());
                    iterator old_finish = finish;
                    --old_finish;
                    pos = start + index;
                    backward_copy (pos, old_finish, old_finish);
                }
                *pos = val;
                return pos;

             }



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
            const_reference front () const { return *start; }

            reference back () { 
                iterator tmp = finish;
                --tmp;
                return *tmp;
            }
            const_reference back () const { 
                iterator tmp = finish;
                --tmp;
                return *tmp;
            }

            size_type size () { return (size_type)(finish - start); }

            size_type max_size () const { return size_type(-1); }

            bool empty () const { return start == finish; }

            deque (size_type n, const value_type& value)
                : start(), finish(), map(0), map_size(0) {
                    fill_initiallize (n, value);
                }

            explicit deque (size_type n) 
                : start(), finish(), map(0), map_size(0) {
                    fill_initiallize (n, value_type());
                }
             ~deque () {
                clear();
                data_allocator.deallocate (*start.node);
                map_allocator.deallocate (map);
             }

            void push_back (const value_type& value) {
                if (finish.cur != finish.last - 1) {
                    construct(finish.cur, value);
                    ++finish.cur;
                } else 
                    push_back_aux (value);
            }

            void push_front (const value_type& value) {
                if (start.cur != start.first) {
                    construct (start.cur, value);
                    --start.cur;
                } else 
                    push_front_aux (value);
            }

            void pop_back () {
                if (finish.cur != finish.first) {
                    destroy (finish.cur);
                    --finish.cur;
                } else 
                    pop_back_aux ();
            }

            void pop_front () {
                if (start.cur != start.last - 1) {
                    destroy (start.cur);
                    ++start.cur;
                } else 
                    pop_front_aux ();
            }
            //come back to the initiallize state
            void clear () {

                for (map_pointer node = start.node + 1; node < finish.node; ++node) {
                    destroy (*node, *node + buf_size());
                    data_allocator.deallocate (*node);
                }

                // reserve a buffer
                if ( start.node != finish.node) {
                    destroy (start.cur, start.last);
                    destroy (finish.first, finish.cur);
                    data_allocator.deallocate (*finish.node);
                } else {
                    destroy (start.cur, start.last);
                    finish = start;
                }

            }
            //erase a single element
            iterator erase (iterator pos) {
                iterator next = pos;
                ++next;
                difference_type index = pos - start;
                if (index < (size()>>1) ) {
                    backward_copy (start, pos, next);
                    pop_front ();
                } else {
                    copy (next, finish, pos);
                    pop_back();
                }
                return start + index;
            }

            iterator erase (iterator first, iterator last) {
                if (first == start && last == finish) {
                    clear();
                    return finish;
                } else {
                    size_type n = last - first;
                    difference_type to_finish = finish - last;
                    difference_type to_start = first - start;
                    if (to_start < to_finish) {
                        backward_copy (start, first, last);
                        iterator new_start = start + n;
                        destroy (start, new_start);
                        for (map_pointer node = start.node; node < new_start.node; ++node)
                            data_allocator.deallocate (*node);
                        start = new_start;
                    } else {
                        copy (last, finish, first);
                        iterator new_finish = finish - n;
                        destroy (new_finish, finish);
                        for (map_pointer node = new_finish.node+1; node <= finish.node; ++node) 
                            data_allocator.deallocate (*node);
                        finish = new_finish;
                    }
                    return  start + to_start;
                }
            }
            
            iterator insert (iterator pos, const value_type& val) {
                if (pos == start) {
                    push_front (val);
                    return start;
                } else if (pos == finish) {
                    push_back (val);
                    return finish;
                } else 
                    return insert_aux (pos, val);
            }
            
    };

}

#endif
