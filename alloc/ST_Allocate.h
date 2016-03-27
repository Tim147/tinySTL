#ifndef ST_ALLOCATE
#define ST_ALLOCATE

#include <stdlib.h>
#include <iostream>

namespace ST_alloc {
	
	class default_alloc {
	private:
		enum {_ALIGN = 8};
		enum Max_byte{_MAX_BYTE = 128};
		enum {_NFREELISTS = _MAX_BYTE/_ALIGN};
	private:

		static size_t round_up(size_t bytes);
		static size_t freelist_index(size_t bytes);

		union obj {
			union obj *freelist_link;
			char client_data[1];
		};
		static obj *free_list[_NFREELISTS];
		static void* refill(size_t n);
		static char* chunk_alloc(size_t size, int &nobjs);
		static char* start_free;
		static char* end_free;
		static size_t heap_size;

	public:
		static void* allocate(size_t n);
		static void deallocate(void* ptr, size_t n);
		static void* reallocate(void* begin, size_t old_size, size_t new_size);

	};

	default_alloc::obj *default_alloc::free_list[_NFREELISTS] = 
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char* default_alloc::start_free = 0;
	char* default_alloc::end_free = 0;
	size_t default_alloc::heap_size = 0;

	size_t default_alloc::round_up(size_t bytes) {
		return (bytes + _ALIGN - 1)& ~(_ALIGN-1);
	}

	size_t default_alloc::freelist_index(size_t bytes) {
		return (size_t)((bytes + _ALIGN - 1) / _ALIGN - 1);
	}

	char* default_alloc::chunk_alloc(size_t size, int& nobjs) {
		char* result;
		size_t tot_bytes = size * nobjs;
		size_t left_bytes = end_free - start_free;

		if(left_bytes >= tot_bytes) {
			result = start_free;
			start_free += tot_bytes;

			return result;
		} else if(left_bytes >= size) {
			result = start_free;
			nobjs = left_bytes / size;
			tot_bytes = nobjs * size;
			start_free += tot_bytes;

			return result;
		} else {
			size_t size_to_get = 2 * tot_bytes + round_up(heap_size >> 4);
			if(left_bytes > 0) {
				obj **my_free_list = free_list + freelist_index(left_bytes);
				((obj *)start_free)->freelist_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}

			start_free = (char*)malloc(size_to_get);
			if(!start_free) {
				obj *p, **my_free_list;
				for(int i = 0; i < _MAX_BYTE; i += _ALIGN) {
					my_free_list = free_list + freelist_index(i);
					p = *my_free_list;
					if(p) {
						*my_free_list = p->freelist_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;
				std::cerr<<"out of memory! "<<std::endl;
				exit(1);
			}
			heap_size += size_to_get;
			end_free = start_free + size_to_get;
			return chunk_alloc(size, nobjs);

		}
	}

	void* default_alloc::refill(size_t n) {
		int nobjs = 20;
		char* chunk = chunk_alloc(n, nobjs);

		if(nobjs == 1) return (obj*)chunk;

		obj* result = (obj*)chunk;

		obj **my_free_list;
		obj *next_obj, *current_obj;

		my_free_list = free_list + freelist_index(n);
		*my_free_list = next_obj = (obj*)(chunk + n);
		for(int i = 1; ;++i) {
			current_obj = next_obj;

			if(i == nobjs - 1){
				current_obj->freelist_link = 0;
				break;
			} else {
				next_obj = (obj*)((char*)next_obj + n);
				current_obj->freelist_link = next_obj;
			}
		}
		return result;
	}

	void *default_alloc::allocate(size_t bytes) {
		if(bytes >= _MAX_BYTE) {
			return malloc(bytes);
		}

		obj **my_free_list;
		obj *result;
		my_free_list = free_list + freelist_index(bytes);
		result = *my_free_list;

		if(!result) {
			return (refill(round_up(bytes)));
		}

		*my_free_list = result->freelist_link;
		return result;
	}

	void *default_alloc::reallocate(void *ptr, size_t old_size, size_t new_size) {
		deallocate(ptr, old_size);
		return allocate(new_size);
	}

	void default_alloc::deallocate(void *ptr, size_t n) {
		if(n >= _MAX_BYTE)
			free(ptr);

		obj **my_free_list;
		obj *tmp_obj;
		my_free_list = free_list + freelist_index(n);
		tmp_obj = (obj*)ptr;

		tmp_obj->freelist_link = *my_free_list;
		*my_free_list = tmp_obj;
	}
} 
#endif