#ifndef ST_ALLOCATOR_H
#define ST_ALLOCATOR_H
#include <stddef.h>
#include <stdlib.h>
#include <iostream>

namespace tinySTL {
    class SimpleAlloc {
    public:
        typedef void           value_type;
        typedef void*          pointer;
       // typedef const void*    const_pointer;
       // typedef T&          reference;
       // typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
            

    private:
        static pointer  oom_alloc(size_type nbytes);
        static pointer  oom_realloc(pointer begin, size_type nbytes);
        static void (*my_oom_malloc_handler)();
        static void set_my_malloc_handler(void (*f)());
    public:
        static pointer allocate(size_type nbytes);
        static pointer reallocate(pointer begin, size_type nbytes);
        static void deallocate(pointer begin);
    };

    void (*SimpleAlloc::my_oom_malloc_handler)() = 0; //set by user

    void SimpleAlloc::set_my_malloc_handler(void (*f)()) {
        void (*handler)() = my_oom_malloc_handler;
        handler = f;
    }
    
    typename SimpleAlloc::pointer SimpleAlloc::oom_alloc(size_type nbytes) {
        void (*handler)();
        pointer result;
        while(true) {
            handler = my_oom_malloc_handler;
            if(!handler) {
                std::cerr<<"out of memory!" <<std::endl;
                exit(1);
            }
            (*handler)();

            result = static_cast<pointer>(malloc(nbytes));
            if(result) {
                return result;
            }
        }
    }

    typename SimpleAlloc::pointer SimpleAlloc::oom_realloc(pointer begin, size_type nbytes) {
        void (*handler)();
        pointer result;

        while(true) {
            handler = my_oom_malloc_handler;

            if(!handler) {
                std::cerr<< "out of memory!"<<std::endl;
                exit(1);
            }

            (*handler)();

            result = static_cast<pointer>(realloc(begin, nbytes));

            if(result)
                return result;
        }
    }

    typename SimpleAlloc::pointer SimpleAlloc::allocate(size_type nbytes) {
        pointer result = static_cast<pointer>(malloc(nbytes));
        if(!result)
            oom_alloc(nbytes);

        return result;
    }

    typename SimpleAlloc::pointer SimpleAlloc::reallocate(pointer begin, size_type nbytes) {
        pointer result = static_cast<pointer>(realloc(begin, nbytes));

        if(!result)
            oom_realloc(begin, nbytes);

        return result;
    }

    void SimpleAlloc::deallocate(pointer begin) {
        free(begin);
    }

template <class T, class Alloc = SimpleAlloc>
    class simple_alloc {
        public:
            static T* allocate (size_t n) {
                return n == 0 ? 0 : (T*) Alloc::allocate(n * sizeof (T)); 
            }
            
            static T* allocate () {
                return (T*) Alloc::allocate (sizeof(T));
            }

            static void deallocate (T* p) {
                Alloc::deallocate (p);
            }

            static T* reallocate (T* p, size_t n) {
                return n == 0 ? 0 : (T*) Alloc::reallocate (p, n);
            }
    };

}






#endif // ALLOCATOR_H

