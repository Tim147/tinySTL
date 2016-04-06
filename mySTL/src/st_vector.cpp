#include "../include/st_vector.h"
namespace tinySTL {

    template <class T, class Alloc = SimpleAlloc>
    void vector<T>::fill_and_initialize(size_type n, const T& x) {
        iterator result = allocate_and_fill(n, x);
            _start = result;
            _end = result + n;
            _capacity = _capacity + n;
    }

    template <class T, class Alloc = SimpleAlloc>
    iterator vector<T>::allocate_and_fill(size_type n, const T& x) {
            iterator result = static_cast<iterator>(data_allocator.allocate(n));
            _uninitialed_fill_n(result, n, x);
            return result;
        
    }



}
