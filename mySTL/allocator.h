#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>
#include <iostream>

namespace TinySTL {
    class my_malloc_alloc {
    private:
        static void *oom_alloc(size_t nbytes);
        static void *oom_realloc(void *begin, size_t nbytes);
        static void (*my_oom_malloc_handler)();
        static void set_my_malloc_handler(void (*f)());
    public:
        void *allocate(size_t nbytes);
        void *reallocate(void *begin, size_t nbytes);
        void *deallocate(void *begin);
    };


    void (*my_malloc_alloc::my_oom_malloc_handler)() = 0; //set by user

    void my_malloc_alloc::set_my_malloc_handler(void (*f)()) {
        void (*handler)() = my_oom_malloc_handler;
        handler = f;
    }

    void* my_malloc_alloc::oom_alloc(size_t nbytes) {
        void (*handler)();
        void *result;
        while(true) {
            handler = my_oom_malloc_handler;
            if(!handler) {
                std::cerr<<"out of memory!" <<std::endl;
                exit(1);
            }
            (*handler)();

            result = malloc(nbytes);
            if(result) {
                return result;
            }
        }
    }

    void* my_malloc_alloc::oom_realloc(void* begin, size_t nbytes) {
        void (*handler)();
        void *result;

        while(true) {
            handler = my_oom_malloc_handler;

            if(!handler) {
                std::cerr<< "out of memory!"<<std::endl;
                exit(1);
            }

            (*handler)();

            result = realloc(begin, nbytes);

            if(result)
                return result;
        }
    }

    void* my_malloc_alloc::allocate(size_t nbytes) {
        void* result = malloc(nbytes);
        if(!result)
            oom_alloc(nbytes);

        return result;
    }

    void* my_malloc_alloc::reallocate(void* begin, size_t nbytes) {
        void* result = realloc(begin, nbytes);

        if(!result)
            oom_realloc(begin, nbytes);

        return result;
    }

    void* my_malloc_alloc::deallocate(void* begin) {
        free(begin);
    }

}


#endif



#endif // ALLOCATOR_H

