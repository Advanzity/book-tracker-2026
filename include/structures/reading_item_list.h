#pragma once

#include <iostream>
#include <string>

#include "common/container_exception.h"
#include "models/reading_item.h"

struct ReadingItemNode
{
    ReadingItem *data;
    ReadingItemNode *next;

    explicit ReadingItemNode(ReadingItem *data, ReadingItemNode *next = nullptr);
};

class ReadingItemListIterator
{
private:
    ReadingItemNode *current;

public:
    explicit ReadingItemListIterator(ReadingItemNode *start = nullptr);

    bool isValid() const;
    void next();
    ReadingItem *getData() const;
};

class ReadingItemList
{
private:
    ReadingItemNode *head;
    ReadingItemNode *tail;
    int count;

    ReadingItemNode *nodeAt(int index) const;

public:
    ReadingItemList();
    ReadingItemList(const ReadingItemList &) = delete;
    ReadingItemList &operator=(const ReadingItemList &) = delete;
    ~ReadingItemList();

    bool isEmpty() const;
    int size() const;

    ReadingItemListIterator begin() const;

    void insertFront(ReadingItem *item);
    void insertBack(ReadingItem *item);
    bool removeAt(int index);
    bool deleteByTitle(const std::string &title);
    ReadingItem *searchByTitle(const std::string &title) const;
    int findByTitle(const std::string &title) const;
    ReadingItem *at(int index) const;
    void traverse(std::ostream &os = std::cout) const;
    void sortByTitle();
    int binarySearchByTitle(const std::string &title) const;
    void clear();
};
