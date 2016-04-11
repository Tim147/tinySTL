#ifndef ST_tECTOR_H
#define ST_VECTOR_H
#include "st_allocator.h"
#include "st_uninitialled.h"
#include "st_algorithm.h"
#include <assert.h>

namespace tinySTL {
    template <class T, class Alloc = SimpleAlloc<T> >
class vector {
    private:
        T* _start;
        T* _end;
        T* _capacity;

        Alloc data_allocator;

    public:
        typedef 	 T 				value_type;
        typedef 	 T*				pointer;
        typedef 	 const T* 		const_pointer;
        typedef 	 T& 			reference;
        typedef 	 const T&		const_reference;

        typedef 	 size_t 		size_type;
        typedef 	 ptrdiff_t		difference_type;

        typedef 	 T* 			iterator;
        typedef 	 const T*		const_iterator;

    private:
        void fill_and_initialize(size_type n, const T& x){
            iterator result = allocate_and_fill(n, x);
            _start = result;
            _end = result + n;
            _capacity = _start + n;
        }
        iterator allocate_and_fill(size_type n, const T& x) {
            iterator result = static_cast<iterator>(data_allocator.allocate(n));
            _uninitialed_fill_n(result, n, x);
            return result;
        }

        template< class InputIterator>
        InputIterator reallocate_and_copy(InputIterator begin, InputIterator end) {
            size_type n = end-begin;
            iterator result = data_allocator.reallocate(_start, n);
            uninitialed_copy(begin, end, result);
            return result;
        }

        template <class InputIterator>
        InputIterator allocate_and_copy(InputIterator begin, InputIterator end) {
            size_type n = end - begin;
            iterator result = data_allocator.allocate(n);
            uninitialed_copy(begin, end, result);
            return result;
        }

        template <class InputIterator>
        iterator insert_aux(const_iterator position, InputIterator first, InputIterator last) {
            size_type n = last - first;
            iterator pos = const_cast<iterator>(position);
            if(_end + n < _capacity) {
               for(size_type i = 0; i < n; ++i)
                   construct(_end+i);
               iterator result = backward_copy<iterator>(pos, _end, _end+n-1);
               for(size_type i = 0; i < n; ++i, ++first)
                   *(pos+i) = *first;
               _end += n;
               return (pos+n-1);
            } else {
                size_type oldsize = _capacity - _start;
                size_type newsize = oldsize + max<size_type>(oldsize, n);
                iterator result = data_allocator.allocate(newsize);
                iterator newpos = uninitialed_copy(_start, pos, result);
                iterator respos = newpos;
                newpos = uninitialed_copy(first, last, newpos);
                newpos = uninitialed_copy(pos, _end, newpos);
                
                destroy(_start, _end);
                data_allocator.deallocate(_start);

                _start = result;
                _end = newpos;
                _capacity = _start + newsize;
                return respos;
            }
        }
        
        iterator insert_aux(const_iterator position, const value_type& val) {
            iterator pos = const_cast<iterator>(position);
            if(_end < _capacity) {
                construct(_end, val);
                iterator result = backward_copy<iterator>(pos, _end, _end);
                //uninitialed_copy(position, _end, position+1);
                *pos = val;
                _end++;
                return pos;
            }else {
                size_type oldsize = _capacity - _start;
                size_type newsize = oldsize==0?1:2*oldsize;

                //iterator result = allocate_and_fill(newsize, val);
                iterator result = data_allocator.allocate(newsize);
                iterator newpos = uninitialed_copy(_start, pos, result);
                construct(newpos, val);
                iterator respos = newpos;
                ++newpos;
                newpos = uninitialed_copy(pos, _end, newpos);
                
                destroy(_start, _end);
                data_allocator.deallocate(_start);

                _start = result;
                _end = newpos;
                _capacity = _start + newsize;
                return respos;
            }         
        }
    public:
        bool empty() {return _end == _start;}
        size_type size() const {return (_end - _start);}

        iterator begin() {return _start; }
        const_iterator begin() const {return _start;}
        const_iterator cbegin() const {return _start; }

        iterator end() { return _end; }
        const_iterator end() const {return _end;}
        const_iterator cend() const { return _end; }
        
        iterator rbegin() {return end()-1; }
        const_iterator rbegin() const { return end()-1;}
        const_iterator crbegin() const {return cend()-1;}

        iterator rend() {return begin()-1;}
        const_iterator rend() const { return begin()-1;}
        const_iterator crend() const {return cbegin()-1;}

       vector() : _start(0), _end(0), _capacity(0) {}


        vector(size_type n, const value_type& x) { fill_and_initialize(n, x);}
        
        vector(vector<T>& x) {
            _start = allocate_and_copy<iterator>(x.begin(), x.end());
            _end = _start + x.size();
            _capacity = _start + x.size();
        }

        vector(const vector<value_type>& x) {
            _start = const_cast<iterator>(allocate_and_copy<const_iterator>(x.begin(), x.end()));
            _end = _start + x.size();
            _capacity = _start + x.size();
        }

        ~vector() {
            if(_capacity != 0) {
                destroy(_start, _end); 
                data_allocator.deallocate(_start);
            }
        }

        reference operator[](int i) { return *(begin()+i);}
        const_reference operator[](int i) const { return *(cbegin()+i);}

        reference back() { return *(end()-1); }
        const_reference back() const { return *(cend()-1); }

        reference front() { return *begin();}
        const_reference front() const { return *cbegin();}

        reference at(int i) {return *(begin()+i); }
        const_reference at(int i) const {return *(cbegin()+i); }

        size_type capacity() const {return (_capacity-_start); }
    
        pointer data() { return begin();}
        const_pointer data() const {return cbegin(); }

        Alloc get_allocator() { return data_allocator;}


        vector& operator=(const vector<T>& x) {
            //const int size =x.size();
            int newsize = x.size();
            if(size() >= newsize) {
                for(int i=0; i < newsize; ++i) {
                    if(i < newsize)
                        *(begin()+i) = *(x.begin()+i);
                    else *(begin()+i) = value_type();
                }
                _end = begin() + newsize;

            }else if(capacity() >= newsize ) {
                for(int i=0; i< newsize;++i) {
                    if(i <= capacity())
                        *(begin()+i) = *(x.begin()+i);
                    else {
                        construct(begin()+i, *(x.begin()+i));
                    }
                }
                _end = begin() + newsize;
            } else {
                _start = const_cast<iterator>(reallocate_and_copy<const_iterator>(x.begin(), x.end()));
                _end = _start + newsize;
                _capacity = _end;
            }

            return *this;
        }

        vector& operator=(vector<T>& x) { //move
            _start = reallocate_and_copy(x.begin(), x.end());
            _end = _start + x.size();
            _capacity = _end;
            destroy(x.begin(), x.end());
            //x.get_allocator().deallocate(x.begin());
            return *this;
        }
        
        //inserting new elements before the element at the specified position,
        iterator insert(const_iterator position, const value_type& val) {
            return insert_aux(position, val);
        }
        
        iterator insert(const_iterator position, size_type n, const value_type& val) {
           vector<value_type> tmp(n, val);
           return insert_aux<iterator>(position, tmp.begin(), tmp.end());
        }

        template <class InputIterator>
        iterator insert(const_iterator position, InputIterator first, InputIterator last) {
            return insert_aux<InputIterator>(position, first, last); 
        }

        iterator insert(const_iterator position, value_type& val) {
            return insert_aux(position, val); 
        }
        
        //Add element at the end
        void push_back(const value_type& val) {
            if(_end != _capacity) {
                construct(_end, val);
                ++_end;
            }
            else 
                insert(end(), val);
        }
        
        // erase element at the end
        void pop_back() {
            assert(_end != _start);
            --_end;
            destroy(_end);
        }

        iterator erase(iterator position) {
            if(position+1 != _end)
                copy(position+1, end(), position);
            --_end;
            destroy(_end);
            return position;
        }

        iterator erase(iterator first, iterator last) {
            size_type n = last - first;
            if(last != end() )
                copy(last, _end, first);
            destroy(_end-n, _end);
            _end = _end - n;
            return first;
        }

        void clear(){
            erase(_start, _end);
        }

        //Exchanges the content of the container by the content of x
        void swap(vector<value_type>& x) {
            tinySTL::swap<iterator>(_start, x._start);
            tinySTL::swap<iterator>(_end, x._end);
            tinySTL::swap<iterator>(_capacity, x._capacity);
        }

        //vector capacity be at least enough to contain n elements
        void reserve (size_type n) {
            if(capacity() < n) {
               size_type oldsize = _end - _start;
            
               iterator result = data_allocator.allocate(n);
               iterator newstart = result;
               result = uninitialed_copy(_start, _end, result);
               
               destroy(_start, _end);
               data_allocator.deallocate(_start);

               _start = newstart;
               _end = result;
               _capacity = _start + n; 
            }
        }

        template <class InputIterator>
        void assign (InputIterator first, InputIterator last) {}

        void assign (size_type n, const value_type& val) {}

        void resize (size_type n) {
            resize(n, value_type());
        }

        void resize (size_type n, const value_type& val) {
            if(size() < n) {
                if(capacity() >= n) { 
                    _end = _uninitialed_fill_n(_end, n-size(), val);
                }
                else {
                    iterator result = data_allocator.allocate(n);
                    iterator newstart = result;

                    result = uninitialed_copy(_start, _end, result);
                    result = _uninitialed_fill_n(result, n-size(), val);

                    destroy(_start, _end);
                    data_allocator.deallocate(_start);

                    _start = newstart;
                    _end = result;
                    _capacity = _start + n;
                }
            }else if(size() > n) {
                destroy(_start+n, _end);
                _end = _start + n;
            }
        }

        // reduce its capacity to fit its size
        void shrink_to_fit() {
            reserve(size());
        }

        size_type max_size() const {}






    };
}
#endif // ST_tECTOR_H

