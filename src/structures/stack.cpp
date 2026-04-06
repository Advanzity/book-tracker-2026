#include "structures/stack.h"

Stack::Stack() : topIndex(-1)
{
}

void Stack::push(const std::string &value)
{
    if (isFull())
    {
        throw ContainerException("Stack is full.");
    }

    topIndex++;
    elements[topIndex] = value;
}

void Stack::pop()
{
    if (isEmpty())
    {
        throw ContainerException("Stack is empty.");
    }

    topIndex--;
}

std::string Stack::peek() const
{
    if (isEmpty())
    {
        throw ContainerException("Stack is empty.");
    }

    return elements[topIndex];
}

bool Stack::isEmpty() const
{
    return topIndex < 0;
}

bool Stack::isFull() const
{
    return topIndex >= CAPACITY - 1;
}
