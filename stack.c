#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "stack.h"

/*
 * creates a new stack and puts an identity matrix at the top
 * returns: the stack
*/

stack *new_stack()
{
    stack *s;
    matrix **m;
    matrix *i;
    s = malloc (sizeof (stack));

    m = malloc (STACK_SIZE * sizeof (matrix *));
    i = mk_matrix (4);
    ident (i);

    s->size = STACK_SIZE;
    s->top = 0;
    s->data = m;
    s->data[s->top] = i;

    return s;
}

/*
 * pushes a copy of the current top matrix to the stack
 * returns: void
*/

void push (stack *s)
{
    matrix *m;
    m = mk_matrix (4);

    if (s->top == s->size - 1)
    {
        s->data = realloc (s->data, (s->size + STACK_SIZE) * sizeof (matrix *));

        s->size = s->size + STACK_SIZE;
    }

    cpy_matrix (s->data[s->top], m);

    s->top++;
    s->data[s->top] = m;
}

/*
 * returns a reference to the top matrix in the stack
 * returns: matrix *, the top matrix in the stack
*/

matrix *peek (stack *s)
{
    return s->data[s->top];
}

/*
 * removes and frees the top matrix in the stack
 * returns: void
*/

void pop (stack *s)
{
    free_matrix (s->data[s->top]);
    s->top--;
}

/*
 * deallocates the stack's memory
 * returns: void
*/

void free_stack (stack *s)
{
    int i;
    for (i = 0; i <= s->top; i++)
        free_matrix (s->data[i]);
    free (s->data);
    free (s);
}

/*
 * prints the stack to std out
 * returns: void
*/

void print_stack (stack *s)
{
    int i;
    for (i = s->top; i >= 0; i--)
        print_matrix (s->data[i]), printf("\n");
}
