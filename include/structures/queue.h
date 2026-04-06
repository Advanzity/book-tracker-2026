#pragma once

#include <string>

#include "common/container_exception.h"

struct QueueNode
{
    std::string data;
    QueueNode *next;
};

// Linked-list queue of titles (FIFO).
// Chosen because enqueue/dequeue are O(1) without shifting elements like an array,
// and the queue can grow with the session without a fixed capacity.
class Queue
{
private:
    QueueNode *head;
    QueueNode *tail;

public:
    Queue();
    ~Queue();

    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;

    void enqueue(const std::string &value);
    void dequeue();
    std::string front() const;
    bool isEmpty() const;
};
