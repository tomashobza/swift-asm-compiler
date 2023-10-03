#include <assert.h>
#include <stdio.h>
#include "stack.h" // include your stack header

void test_stack_init() {
    Stack *s = stack_init();
    assert(s != NULL);
    assert(stack_is_empty(s));
    assert(s->size == 0);
    stack_free(s);
}

void test_stack_push_and_pop() {
    Stack *s = stack_init();
    symtable_t *t1 = init_symtable();
    symtable_t *t2 = init_symtable();

    stack_push(s, t1);
    assert(!stack_is_empty(s));
    assert(s->size == 1);

    stack_push(s, t2);
    assert(!stack_is_empty(s));
    assert(s->size == 2);

    assert(stack_pop(s) == t2);
    assert(s->size == 1);

    assert(stack_pop(s) == t1);
    assert(s->size == 0);

    stack_free(s);
}

void test_stack_free() {
    Stack *s = stack_init();
    symtable_t *t1 = init_symtable();
    stack_push(s, t1);

    stack_free(s); // This should also free the tables in the stack, depending on your implementation
    // No asserts here, this is just to validate no leaks or invalid accesses
}

int main() {
    test_stack_init();
    test_stack_push_and_pop();
    test_stack_free();

    printf("All stack tests passed.\n");

    return 0;
}
