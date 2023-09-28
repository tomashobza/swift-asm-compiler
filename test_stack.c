#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "symtable.h"
#include "stack.c"

void test_createStack() {
    Stack *s = createStack(10);
    assert(s != NULL);
    assert(s->size == 0);
    assert(s->capacity == 10);
    freeStack(s);
}

void test_push_and_pop() {
    Stack *s = createStack(2);
    symtable_t item1 = NULL; // Initialize item1
    symtable_t item2 = NULL; // Initialize item2

    // Test pushing
    push(s, item1);
    assert(s->size == 1);
    assert(s->top == item1);
    
    push(s, item2);
    assert(s->size == 2);
    assert(s->top == item2);

    // Test automatic resizing
    assert(s->capacity == 4);

    // Test popping
    assert(pop(s) == item2);
    assert(s->size == 1);
    assert(s->top == item1);

    assert(pop(s) == item1);
    assert(s->size == 0);
    
    // Test automatic downsizing
    assert(s->capacity == 2);

    freeStack(s);
}

void test_memory_cleanup() {
    Stack *s = createStack(2);
    freeStack(s);
    // Should not lead to errors or segmentation faults
}

int main() {
    test_createStack();
    test_push_and_pop();
    test_memory_cleanup();
    printf("All tests passed.\n");
    return 0;
}