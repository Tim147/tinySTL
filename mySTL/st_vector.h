#ifndef ST_VECTOR_H
#define ST_VECTOR_H


namespace ST_stl {
    template <class T, Alloc = alloc>
    class vector {
    private:
        T* _start;
        T* _end;
        T* _capacity;

        alloc<T> data_allocator;

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
        size_type size() {return (_end - _start;)}
        iterator begin() {return _start; }
        iterator end() { return _end; }
        vector() : _start(0), _end(0), _capacity(0) {}
        vector(size_type n, const value_type& x) { fill_and_initialize(n, value);}

        iterator allocate_and_fill(size_type n, const T& x) {
            iterator result = data_allocator::allocate(n);


        }


    };
};
#endif // ST_VECTOR_H

