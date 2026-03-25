#include "models/audio_book.h"

#include <iomanip>

AudioBook::AudioBook()
    : ReadingItem(), narrator("Unknown"), price() {}

AudioBook::AudioBook(const std::string &title, int pages, double hours, Difficulty difficulty,
                     const std::string &narrator, const PriceInfo &price)
    : ReadingItem(title, pages, hours, difficulty), narrator(narrator), price(price) {}

void AudioBook::setNarrator(const std::string &narrator)
{
    this->narrator = narrator;
}

std::string AudioBook::getNarrator() const
{
    return narrator;
}

void AudioBook::setPrice(const PriceInfo &price)
{
    this->price = price;
}

const PriceInfo &AudioBook::getPrice() const
{
    return price;
}

std::string AudioBook::displayName() const
{
    return title + " (narrated by " + narrator + ")";
}

void AudioBook::toStream(std::ostream &os) const
{
    os << "AudioBook: " << displayName()
       << " | pages: " << pages
       << " | hours: " << std::fixed << std::setprecision(1) << hours
       << " | difficulty: " << difficultyToString(difficulty)
       << " | cost: " << price.formattedCost();
}

void AudioBook::print(std::ostream &os) const
{
    ReadingItem::print(os);
    os << "Narrator: " << narrator << "\n";
    os << "Cost: " << price.formattedCost() << "\n";
}
