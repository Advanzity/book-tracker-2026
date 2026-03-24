#pragma once

#include <string>

#include "models/price_info.h"
#include "models/reading_item.h"

class PrintBook : public ReadingItem
{
private:
    std::string author;
    PriceInfo price;

public:
    PrintBook();
    PrintBook(const std::string &title, int pages, double hours, Difficulty difficulty,
              const std::string &author, const PriceInfo &price);

    void setAuthor(const std::string &author);
    std::string getAuthor() const;

    void setPrice(const PriceInfo &price);
    const PriceInfo &getPrice() const;

    bool operator==(const PrintBook &other) const;

    std::string displayName() const override;
    void toStream(std::ostream &os) const override;
    void print(std::ostream &os = std::cout) const override;
};
