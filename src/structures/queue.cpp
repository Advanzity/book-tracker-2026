#include "structures/queue.h"

Queue::Queue() : head(nullptr), tail(nullptr)
{
}

Queue::~Queue()
{
    while (!isEmpty())
    {
        dequeue();
    }
}

void Queue::enqueue(const std::string &value)
{
    QueueNode *node = new QueueNode{value, nullptr};

    if (tail == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->next = node;
        tail = node;
    }
}

void Queue::dequeue()
{
    if (isEmpty())
    {
        throw ContainerException("Queue is empty.");
    }

    QueueNode *oldHead = head;
    head = head->next;
    if (head == nullptr)
    {
        tail = nullptr;
    }

    delete oldHead;
}

std::string Queue::front() const
{
    if (isEmpty())
    {
        throw ContainerException("Queue is empty.");
    }

    return head->data;
}

bool Queue::isEmpty() const
{
    return head == nullptr;
}
