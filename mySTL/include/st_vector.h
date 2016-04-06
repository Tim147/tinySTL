#ifndef ST_VECTOR_H
#define ST_VECTOR_H
#include "allocator.h"
#include "uninitialled.h"

namespace tinySTL {
    template <class T, class Alloc = SimpleAlloc>
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

        
    public:
        size_type size() {return (_end - _start);}
        iterator begin() {return _start; }
        iterator end() { return _end; }
        vector() : _start(0), _end(0), _capacity(0) {}

        void fill_and_initialize(size_type n, const T& x) {
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
        vector(size_type n, const value_type& x) { fill_and_initialize(n, x);}


    };
}
#endif // ST_VECTOR_H

