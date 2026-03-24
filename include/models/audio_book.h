#pragma once

#include <string>

#include "models/price_info.h"
#include "models/reading_item.h"

class AudioBook : public ReadingItem
{
private:
    std::string narrator;
    PriceInfo price;

public:
    AudioBook();
    AudioBook(const std::string &title, int pages, double hours, Difficulty difficulty,
              const std::string &narrator, const PriceInfo &price);

    void setNarrator(const std::string &narrator);
    std::string getNarrator() const;

    void setPrice(const PriceInfo &price);
    const PriceInfo &getPrice() const;

    std::string displayName() const override;
    void toStream(std::ostream &os) const override;
    void print(std::ostream &os = std::cout) const override;
};
