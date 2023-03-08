#ifndef TESTINGFUNCTIONS_H
#define TESTINGFUNCTIONS_H

#include "Config.h"
#include "Stack.h"

#ifndef RELEASE_MODE

void StackPushN(Stack* stack, size_t n)
{
    for (size_t i = 1; i <= n; i++)
        StackPush(stack, (Elem_t) NULL);
}

void FillStack(Stack* stack)
{
    StackPushN(stack, stack->capacity - stack->size);
}

void StackPopN(Stack* stack, size_t n)
{
    for (size_t i = 1; i <= n; i++)
        StackPop(stack);
}

void CleanStack(Stack* stack)
{
    StackPopN(stack, stack->size);
}

#else

void StackPushN(Stack* stack, size_t n) {}
void FillStack(Stack* stack) {}
void StackPopN(Stack* stack, size_t n) {}
void CleanStack(Stack* stack) {}

#endif

#endif
