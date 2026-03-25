#include "models/reading_item.h"

#include <iomanip>

ReadingItem::ReadingItem()
    : title("Untitled"), pages(0), difficulty(EASY), hours(0.0) {}

ReadingItem::ReadingItem(const std::string &title, int pages, double hours, Difficulty difficulty)
    : title(title), pages(pages), difficulty(difficulty), hours(hours) {}

ReadingItem::~ReadingItem() = default;

void ReadingItem::setTitle(const std::string &title)
{
    this->title = title;
}

std::string ReadingItem::getTitle() const
{
    return title;
}

void ReadingItem::setPages(int pages)
{
    this->pages = pages;
}

int ReadingItem::getPages() const
{
    return pages;
}

void ReadingItem::setHours(double hours)
{
    this->hours = hours;
}

double ReadingItem::getHours() const
{
    return hours;
}

void ReadingItem::setDifficulty(Difficulty difficulty)
{
    this->difficulty = difficulty;
}

Difficulty ReadingItem::getDifficulty() const
{
    return difficulty;
}

void ReadingItem::toStream(std::ostream &os) const
{
    os << displayName()
       << " | pages: " << pages
       << " | hours: " << std::fixed << std::setprecision(1) << hours
       << " | difficulty: " << difficultyToString(difficulty);
}

void ReadingItem::print(std::ostream &os) const
{
    os << "Title: " << title << "\n";
    os << "Pages: " << pages << "\n";
    os << "Hours: " << std::fixed << std::setprecision(1) << hours << "\n";
    os << "Difficulty: " << difficultyToString(difficulty) << "\n";
}

std::ostream &operator<<(std::ostream &os, const ReadingItem &item)
{
    item.toStream(os);
    return os;
}
