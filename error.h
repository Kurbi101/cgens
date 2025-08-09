#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#ifdef __GNUC__
#define HAS_BACKTRACE
#include <execinfo.h>
#include <unistd.h>
#endif

static inline void print_stack_trace(void) {
#ifdef HAS_BACKTRACE
	void *arr[10];

	size_t size = backtrace(arr, 10);
	char **strings = backtrace_symbols(arr, size);

	fprintf(stderr, "\x1b[1;33mStack trace:\x1b[0m\n");
	for (size_t i = 0; i < size; i++) {
		fprintf(stderr, ">  [%zu] %s\n", i, strings[i]);
	}
#else
	fprintf(stderr, "\x1b[1;33mStack trace not available, please change to a "
					"better platform\x1b[0m\n");
#endif
}

#define ERROR(msg, ...)                                                        \
	do {                                                                       \
		fprintf(stderr,                                                        \
				"\x1b[1;31m<< ERROR in %s:%d (%s): " msg " >>>\x1b[0m\n",      \
				__FILE__, __LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__);      \
		print_stack_trace();                                                   \
		abort();                                                               \
	} while (0)

#endif