#pragma once

#include <iostream>
#include <string>

#include "common/difficulty.h"

class ReadingItem
{
protected:
    std::string title;
    int pages;
    Difficulty difficulty;
    double hours;

public:
    ReadingItem();
    ReadingItem(const std::string &title, int pages, double hours, Difficulty difficulty);
    virtual ~ReadingItem();

    void setTitle(const std::string &title);
    std::string getTitle() const;

    void setPages(int pages);
    int getPages() const;

    void setHours(double hours);
    double getHours() const;

    void setDifficulty(Difficulty difficulty);
    Difficulty getDifficulty() const;

    virtual std::string displayName() const = 0;
    virtual void toStream(std::ostream &os) const;
    virtual void print(std::ostream &os = std::cout) const;

    friend std::ostream &operator<<(std::ostream &os, const ReadingItem &item);
};

std::ostream &operator<<(std::ostream &os, const ReadingItem &item);
