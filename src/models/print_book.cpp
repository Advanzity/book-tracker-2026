#include "models/print_book.h"

#include <iomanip>

PrintBook::PrintBook()
    : ReadingItem(), author("Unknown"), price() {}

PrintBook::PrintBook(const std::string &title, int pages, double hours, Difficulty difficulty,
                     const std::string &author, const PriceInfo &price)
    : ReadingItem(title, pages, hours, difficulty), author(author), price(price) {}

void PrintBook::setAuthor(const std::string &author)
{
    this->author = author;
}

std::string PrintBook::getAuthor() const
{
    return author;
}

void PrintBook::setPrice(const PriceInfo &price)
{
    this->price = price;
}

const PriceInfo &PrintBook::getPrice() const
{
    return price;
}

bool PrintBook::operator==(const PrintBook &other) const
{
    return title == other.title && author == other.author;
}

std::string PrintBook::displayName() const
{
    return title + " by " + author;
}

void PrintBook::toStream(std::ostream &os) const
{
    os << "PrintBook: " << displayName()
       << " | pages: " << pages
       << " | hours: " << std::fixed << std::setprecision(1) << hours
       << " | difficulty: " << difficultyToString(difficulty)
       << " | cost: " << price.formattedCost();
}

void PrintBook::print(std::ostream &os) const
{
    ReadingItem::print(os);
    os << "Author: " << author << "\n";
    os << "Cost: " << price.formattedCost() << "\n";
}
