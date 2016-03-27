#include <cstdlib>

namespace tinySTL {

template <int inst>
	class _malloc_alloc {
	private:
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *, size_t);
		static void (* malloc_alloc_oom_handler)();
	public:
		static void *allocate(size_t n) {
			void *result = malloc(n);
			if(!result) {
				result = oom_malloc(n);
			}
			return result;
		}

		static void *reallocate(void *old, size_t n) {
			void *result = realloc(old, n);
			if(!result) {
				result = oom_realloc(old, n);
			}
			return result;
		}

		static void deallocate(void *p) {
			free(p);
		}
	};
}