#pragma once

#include <string>

class PriceInfo
{
private:
    double cost;
    bool includedWithSubscription;

public:
    PriceInfo();
    PriceInfo(double cost, bool includedWithSubscription);

    void setCost(double cost);
    double getCost() const;

    void setIncludedWithSubscription(bool includedWithSubscription);
    bool getIncludedWithSubscription() const;

    bool isFree() const;
    std::string formattedCost() const;
};
