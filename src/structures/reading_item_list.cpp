#include "structures/reading_item_list.h"

ReadingItemNode::ReadingItemNode(ReadingItem *data, ReadingItemNode *next)
    : data(data), next(next) {}

ReadingItemListIterator::ReadingItemListIterator(ReadingItemNode *start)
    : current(start) {}

bool ReadingItemListIterator::isValid() const
{
    return current != nullptr;
}

void ReadingItemListIterator::next()
{
    if (current != nullptr)
    {
        current = current->next;
    }
}

ReadingItem *ReadingItemListIterator::getData() const
{
    if (!isValid())
    {
        throw ContainerException("Iterator is out of range.");
    }

    return current->data;
}

ReadingItemList::ReadingItemList()
    : head(nullptr), tail(nullptr), count(0) {}

ReadingItemList::~ReadingItemList()
{
    clear();
}

bool ReadingItemList::isEmpty() const
{
    return count == 0;
}

int ReadingItemList::size() const
{
    return count;
}

ReadingItemListIterator ReadingItemList::begin() const
{
    return ReadingItemListIterator(head);
}

void ReadingItemList::insertFront(ReadingItem *item)
{
    if (item == nullptr)
    {
        throw ContainerException("Cannot insert a null item.");
    }

    head = new ReadingItemNode(item, head);
    if (tail == nullptr)
    {
        tail = head;
    }

    count++;
}

void ReadingItemList::insertBack(ReadingItem *item)
{
    if (item == nullptr)
    {
        throw ContainerException("Cannot insert a null item.");
    }

    ReadingItemNode *newNode = new ReadingItemNode(item);
    if (isEmpty())
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    count++;
}

bool ReadingItemList::removeAt(int index)
{
    if (index < 0 || index >= count)
    {
        return false;
    }

    ReadingItemNode *nodeToDelete = head;
    if (index == 0)
    {
        head = head->next;
        if (head == nullptr)
        {
            tail = nullptr;
        }
    }
    else
    {
        ReadingItemNode *previous = nodeAt(index - 1);
        nodeToDelete = previous->next;
        previous->next = nodeToDelete->next;
        if (nodeToDelete == tail)
        {
            tail = previous;
        }
    }

    delete nodeToDelete->data;
    delete nodeToDelete;
    count--;
    return true;
}

bool ReadingItemList::deleteByTitle(const std::string &title)
{
    if (isEmpty())
    {
        return false;
    }

    if (head->data->getTitle() == title)
    {
        return removeAt(0);
    }

    ReadingItemNode *previous = head;
    ReadingItemNode *current = head->next;

    while (current != nullptr)
    {
        if (current->data->getTitle() == title)
        {
            previous->next = current->next;
            if (current == tail)
            {
                tail = previous;
            }

            delete current->data;
            delete current;
            count--;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

ReadingItem *ReadingItemList::searchByTitle(const std::string &title) const
{
    for (ReadingItemListIterator it = begin(); it.isValid(); it.next())
    {
        if (it.getData()->getTitle() == title)
        {
            return it.getData();
        }
    }

    return nullptr;
}

int ReadingItemList::findByTitle(const std::string &title) const
{
    int index = 0;
    for (ReadingItemListIterator it = begin(); it.isValid(); it.next(), index++)
    {
        if (it.getData()->getTitle() == title)
        {
            return index;
        }
    }

    return -1;
}

ReadingItem *ReadingItemList::at(int index) const
{
    ReadingItemNode *node = nodeAt(index);
    if (node == nullptr)
    {
        throw ContainerException("Index out of range.");
    }

    return node->data;
}

void ReadingItemList::traverse(std::ostream &os) const
{
    if (isEmpty())
    {
        os << "List is empty.\n";
        return;
    }

    int nodeNumber = 1;
    for (ReadingItemListIterator it = begin(); it.isValid(); it.next(), nodeNumber++)
    {
        os << "Node " << nodeNumber << ": " << it.getData()->displayName() << "\n";
    }
}

void ReadingItemList::sortByTitle()
{
    if (count < 2)
    {
        return;
    }

    bool swapped;
    do
    {
        swapped = false;
        ReadingItemNode *current = head;

        while (current != nullptr && current->next != nullptr)
        {
            if (current->data->getTitle() > current->next->data->getTitle())
            {
                ReadingItem *temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = true;
            }

            current = current->next;
        }
    } while (swapped);
}

int ReadingItemList::binarySearchByTitle(const std::string &title) const
{
    if (isEmpty())
    {
        return -1;
    }

    ReadingItem **snapshot = new ReadingItem *[count];
    int snapshotIndex = 0;
    for (ReadingItemListIterator it = begin(); it.isValid(); it.next())
    {
        snapshot[snapshotIndex++] = it.getData();
    }

    int result = -1;
    int low = 0;
    int high = count - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (snapshot[mid]->getTitle() == title)
        {
            result = mid;
            break;
        }

        if (snapshot[mid]->getTitle() < title)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    delete[] snapshot;
    return result;
}

void ReadingItemList::clear()
{
    ReadingItemNode *current = head;
    while (current != nullptr)
    {
        ReadingItemNode *next = current->next;
        delete current->data;
        delete current;
        current = next;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}

ReadingItemNode *ReadingItemList::nodeAt(int index) const
{
    if (index < 0 || index >= count)
    {
        return nullptr;
    }

    ReadingItemNode *current = head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    return current;
}
