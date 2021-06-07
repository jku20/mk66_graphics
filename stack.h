#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 2

typedef struct stack 
{
    int size;
    int top;
    matrix **data;
} stack;

stack *new_stack();
matrix *peek (stack *s);
void push (stack *s);
void pop (stack *s);

void free_stack (stack *);
void print_stack (stack *);

#endif
