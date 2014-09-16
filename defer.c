/*
  implements the go language defer in C language
  by Robert Oliveira
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DEFER_STACK_SIZE 1024

typedef void (*deferred)(void *);
inline void *get_retp(void) __attribute__((always_inline));
inline void *set_ret(void *) __attribute__((always_inline));
inline void defer(deferred func, void *arg) __attribute__((always_inline));

typedef struct {
	deferred data[DEFER_STACK_SIZE];
	deferred *sp;
	deferred *end;
} defer_stack;

static defer_stack stack = { .sp = 0 };

static void init_stack(void) {
	stack.sp = stack.data;
	stack.end = &(stack.data[DEFER_STACK_SIZE]);
}

static void push(deferred f) {
	if ( stack.sp == 0) init_stack();
	if ( stack.sp >= stack.end ) {
		puts("deferred stack overflow");
		exit(1);
	}
	*(stack.sp) = f;
	stack.sp++;
}

static deferred pop() {
	if ( (stack.sp==0) || (stack.sp <= stack.data) ) {
		puts("deferred stack underflow");
		exit(1);
	}
	stack.sp--;
	return *stack.sp;
}

static void run_deferred(void) {
	void *arg = pop();
	deferred f = pop();
	void *ret = pop();
	f(arg);
	asm("leave;"
		 "push %%rax;"
		 "ret;"
		 ::"a" (ret));
}

inline void defer(deferred func, void *arg) {
	void *(*bp) = (void **) __builtin_frame_address(0);
	push(bp[1]);
	push(func);
	push(arg);
	bp[1] = run_deferred;
}


void test_defer(int i) {
	void *m = malloc(1);
	puts("checkpoint 1");
	defer(free, m);
	puts("checkpoint 2");
	// remove the comment to see a double free happening
	// free(m);
}

int main() {
	test_defer(1);
	return 0;
}
