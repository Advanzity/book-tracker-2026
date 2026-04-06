#pragma once

#include <string>

#include "common/container_exception.h"

// Array-based stack of titles (LIFO).
// Chosen because push/pop/peek are O(1) with no extra allocations, and a fixed
// capacity makes it easy to detect overflow (full stack) for robustness.
class Stack
{
private:
    static const int CAPACITY = 64;
    std::string elements[CAPACITY];
    int topIndex;

public:
    Stack();

    void push(const std::string &value);
    void pop();
    std::string peek() const;
    bool isEmpty() const;
    bool isFull() const;
};
