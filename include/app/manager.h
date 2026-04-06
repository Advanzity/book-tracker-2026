#pragma once

#include <string>

#include "common/safe_divide.h"
#include "models/audio_book.h"
#include "models/print_book.h"
#include "structures/queue.h"
#include "structures/reading_item_list.h"
#include "structures/stack.h"

class Manager
{
private:
    ReadingItemList items;
    Stack recentAdditions;
    Queue removedTitles;

    bool isNonEmpty(const std::string &value) const;
    std::string readLine(const std::string &prompt);
    int readPositiveInt(const std::string &prompt);
    double readNonNegativeDouble(const std::string &prompt);
    int readChoice(const std::string &prompt, int minValue, int maxValue);
    Difficulty readDifficulty();
    PriceInfo readPriceInfo();
    void addPrintBook();
    void addAudioBook();
    void removeItemUI();

public:
    Manager() = default;

    bool isEmpty() const;

    Manager &operator+=(ReadingItem *item);
    Manager &operator-=(int index);
    ReadingItem *operator[](int index) const;

    void addItem(ReadingItem *item);
    bool removeItem(int index);

    int getItemCount() const;
    int getTotalPages() const;
    double getTotalHours() const;

    int sequentialSearchByTitle(const std::string &title) const;
    void sortByTitle();
    int binarySearchByTitle(const std::string &title) const;

    double getAvgSpeed() const;
    int countByDifficulty(Difficulty difficulty) const;

    std::string peekRecentAddition() const;
    bool hasPendingRemovals() const;

    void showBanner() const;
    void showMenu() const;
    void showReport();
    void saveToFile() const;
    void run();
};
