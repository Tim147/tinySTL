#ifndef ST_VECTOR_H
#define ST_VECTOR_H
#include "st_allocator.h"
#include "st_uninitialled.h"

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
            _capacity = _capacity + n;
        }
        iterator allocate_and_fill(size_type n, const T& x) {
            iterator result = static_cast<iterator>(data_allocator.allocate(n));
            _uninitialed_fill_n(result, n, x);
            return result;
        }
        template< class InputIterator>
        InputIterator reallocate_and_copy(InputIterator begin, InputIterator end, size_type n) {
            iterator result = data_allocator.reallocate(_start, n);
            _start = result;
            _end = tinySTL::uninitialed_copy(begin, end, result);
            _capacity = _end;
            return result;
        }



    public:
        bool empty() {return _end == _start;}
        size_type size() {return (_end - _start);}

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
            int newsize = static_cast<int>(x.size());
            if(size() >= x.size()) {
                for(int i=0; i<size(); ++i) {
                    if(i<x.size())
                        *(begin()+i) = *(x.begin()+i);
                    else *(begin()+i) = value_type();
                }
                _end = begin() + x.size();

            }else if(capacity() >= x.size() ) {
                for(int i=0; i<x.size();++i)
                    *(begin()+i) = *(x.begin()+i);
                _end = begin() + x.size();
            } else {
            _capacity = reallocate_and_copy(x.begin(), x.end(), newsize);
            }

            return *this;
        }

        vector& operator=(vector<T>& x) { //move
            _capacity = reallocate_and_copy(x.begin(), x.end(), x.size());
            destroy(x.begin(), x.end());
            //x.get_allocator().deallocate(x.begin());
            return *this;
        }




    };
}
#endif // ST_VECTOR_H

