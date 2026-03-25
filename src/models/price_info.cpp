#include "models/price_info.h"

#include <iomanip>
#include <sstream>

PriceInfo::PriceInfo()
    : cost(0.0), includedWithSubscription(false) {}

PriceInfo::PriceInfo(double cost, bool includedWithSubscription)
    : cost(cost), includedWithSubscription(includedWithSubscription) {}

void PriceInfo::setCost(double cost)
{
    this->cost = cost;
}

double PriceInfo::getCost() const
{
    return cost;
}

void PriceInfo::setIncludedWithSubscription(bool includedWithSubscription)
{
    this->includedWithSubscription = includedWithSubscription;
}

bool PriceInfo::getIncludedWithSubscription() const
{
    return includedWithSubscription;
}

bool PriceInfo::isFree() const
{
    return cost <= 0.0 || includedWithSubscription;
}

std::string PriceInfo::formattedCost() const
{
    if (isFree())
    {
        return "Free";
    }

    std::ostringstream output;
    output << "$" << std::fixed << std::setprecision(2) << cost;
    return output.str();
}
