#ifndef ST_ALLOCATOR_H
#define ST_ALLOCATOR_H
#include <stddef.h>
#include <stdlib.h>
#include <iostream>

namespace tinySTL {
    template <class T>
    class SimpleAlloc {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
            

    private:
        static pointer  oom_alloc(size_type nbytes);
        static pointer  oom_realloc(pointer begin, size_type nbytes);
        static void (*my_oom_malloc_handler)();
        static void set_my_malloc_handler(void (*f)());
    public:
        pointer allocate(size_type nbytes);
        pointer reallocate(pointer begin, size_type nbytes);
        void deallocate(pointer begin);
    };

    template <class T>
    void (*SimpleAlloc<T>::my_oom_malloc_handler)() = 0; //set by user

    template<class T>
    void SimpleAlloc<T>::set_my_malloc_handler(void (*f)()) {
        void (*handler)() = my_oom_malloc_handler;
        handler = f;
    }
    
    template <class T>
    typename SimpleAlloc<T>::pointer SimpleAlloc<T>::oom_alloc(size_type nbytes) {
        void (*handler)();
        pointer result;
        while(true) {
            handler = my_oom_malloc_handler;
            if(!handler) {
                std::cerr<<"out of memory!" <<std::endl;
                exit(1);
            }
            (*handler)();

            result = static_cast<pointer>(malloc(nbytes*sizeof(T)));
            if(result) {
                return result;
            }
        }
    }

    template <class T>
    typename SimpleAlloc<T>::pointer SimpleAlloc<T>::oom_realloc(pointer begin, size_type nbytes) {
        void (*handler)();
        pointer result;

        while(true) {
            handler = my_oom_malloc_handler;

            if(!handler) {
                std::cerr<< "out of memory!"<<std::endl;
                exit(1);
            }

            (*handler)();

            result = static_cast<pointer>(realloc(begin, nbytes*sizeof(T)));

            if(result)
                return result;
        }
    }

    template <class T>
    typename SimpleAlloc<T>::pointer SimpleAlloc<T>::allocate(size_type nbytes) {
        pointer result = static_cast<pointer>(malloc(nbytes*sizeof(T)));
        if(!result)
            oom_alloc(nbytes*sizeof(T));

        return result;
    }

    template <class T>
    typename SimpleAlloc<T>::pointer SimpleAlloc<T>::reallocate(pointer begin, size_type nbytes) {
        pointer result = static_cast<pointer>(realloc(begin, nbytes*sizeof(T)));

        if(!result)
            oom_realloc(begin, nbytes*sizeof(T));

        return result;
    }

    template <class T>
    void SimpleAlloc<T>::deallocate(pointer begin) {
        free(begin);
    }

}





#endif // ALLOCATOR_H

