#include "app/manager.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

bool Manager::isNonEmpty(const std::string &value) const
{
    return !value.empty();
}

std::string Manager::readLine(const std::string &prompt)
{
    std::string value;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, value);
    while (!isNonEmpty(value))
    {
        std::cout << "Input cannot be empty. Try again: ";
        std::getline(std::cin >> std::ws, value);
    }

    return value;
}

int Manager::readPositiveInt(const std::string &prompt)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && value > 0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid! Enter a positive number.\n";
    }
}

double Manager::readNonNegativeDouble(const std::string &prompt)
{
    double value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && value >= 0.0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid! Enter a non-negative number.\n";
    }
}

int Manager::readChoice(const std::string &prompt, int minValue, int maxValue)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue && value <= maxValue)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid! Enter a number between " << minValue << " and " << maxValue << ".\n";
    }
}

Difficulty Manager::readDifficulty()
{
    std::cout << "\nDifficulty:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Medium\n";
    std::cout << "3. Hard\n";
    int choice = readChoice("Choose (1-3): ", 1, 3);
    return static_cast<Difficulty>(choice);
}

PriceInfo Manager::readPriceInfo()
{
    double cost = readNonNegativeDouble("Cost (0 for free): ");
    int included = readChoice("Included with subscription? (1=Yes, 2=No): ", 1, 2);
    return PriceInfo(cost, included == 1);
}

void Manager::addPrintBook()
{
    std::cout << "\n--- Add Print Book ---\n";
    std::string title = readLine("Title: ");
    int pages = readPositiveInt("Pages: ");
    double hours = readNonNegativeDouble("Hours spent: ");
    Difficulty difficulty = readDifficulty();
    std::string author = readLine("Author: ");
    PriceInfo price = readPriceInfo();

    addItem(new PrintBook(title, pages, hours, difficulty, author, price));
    std::cout << "\nPrint book added!\n";
}

void Manager::addAudioBook()
{
    std::cout << "\n--- Add Audio Book ---\n";
    std::string title = readLine("Title: ");
    int pages = readPositiveInt("Pages (or length in pages): ");
    double hours = readNonNegativeDouble("Hours listened: ");
    Difficulty difficulty = readDifficulty();
    std::string narrator = readLine("Narrator: ");
    PriceInfo price = readPriceInfo();

    addItem(new AudioBook(title, pages, hours, difficulty, narrator, price));
    std::cout << "\nAudio book added!\n";
}

void Manager::removeItemUI()
{
    if (isEmpty())
    {
        std::cout << "\nNo items to remove!\n";
        return;
    }

    std::cout << "\n--- Remove Item ---\n";
    int itemNumber = 1;
    for (ReadingItemListIterator it = items.begin(); it.isValid(); it.next(), itemNumber++)
    {
        std::cout << itemNumber << ". " << it.getData()->displayName() << "\n";
    }

    int index = readChoice("Select item to remove (0 to cancel): ", 0, getItemCount());
    if (index > 0)
    {
        removeItem(index - 1);
        std::cout << "\nItem removed.\n";
    }
}

bool Manager::isEmpty() const
{
    return items.isEmpty();
}

Manager &Manager::operator+=(ReadingItem *item)
{
    items.insertBack(item);
    return *this;
}

Manager &Manager::operator-=(int index)
{
    if (!removeItem(index))
    {
        throw ContainerException("Index out of range.");
    }

    return *this;
}

ReadingItem *Manager::operator[](int index) const
{
    if (index < 0 || index >= items.size())
    {
        throw ContainerException("Index out of range.");
    }

    return items.at(index);
}

void Manager::addItem(ReadingItem *item)
{
    *this += item;
    if (!recentAdditions.isFull())
    {
        recentAdditions.push(item->getTitle());
    }
}

bool Manager::removeItem(int index)
{
    if (index < 0 || index >= getItemCount())
    {
        return false;
    }

    std::string title = (*this)[index]->getTitle();
    bool removed = items.removeAt(index);
    if (removed)
    {
        removedTitles.enqueue(title);
    }

    return removed;
}

int Manager::getItemCount() const
{
    return items.size();
}

int Manager::getTotalPages() const
{
    int totalPages = 0;
    for (ReadingItemListIterator it = items.begin(); it.isValid(); it.next())
    {
        totalPages += it.getData()->getPages();
    }

    return totalPages;
}

double Manager::getTotalHours() const
{
    double totalHours = 0.0;
    for (ReadingItemListIterator it = items.begin(); it.isValid(); it.next())
    {
        totalHours += it.getData()->getHours();
    }

    return totalHours;
}

int Manager::sequentialSearchByTitle(const std::string &title) const
{
    return items.findByTitle(title);
}

void Manager::sortByTitle()
{
    items.sortByTitle();
}

int Manager::binarySearchByTitle(const std::string &title) const
{
    return items.binarySearchByTitle(title);
}

double Manager::getAvgSpeed() const
{
    return safeDivide(static_cast<double>(getTotalPages()), getTotalHours());
}

int Manager::countByDifficulty(Difficulty difficulty) const
{
    int matches = 0;
    for (ReadingItemListIterator it = items.begin(); it.isValid(); it.next())
    {
        if (it.getData()->getDifficulty() == difficulty)
        {
            matches++;
        }
    }

    return matches;
}

std::string Manager::peekRecentAddition() const
{
    if (recentAdditions.isEmpty())
    {
        return std::string();
    }

    return recentAdditions.peek();
}

bool Manager::hasPendingRemovals() const
{
    return !removedTitles.isEmpty();
}

void Manager::showBanner() const
{
    std::cout << "\n================================\n";
    std::cout << "   DAVID'S BOOK TRACKER 2026\n";
    std::cout << "=================================\n\n";
}

void Manager::showMenu() const
{
    std::cout << "\n--- MENU ---\n";
    std::cout << "1. Add Print Book\n";
    std::cout << "2. Add Audio Book\n";
    std::cout << "3. Remove Item\n";
    std::cout << "4. View Report\n";
    std::cout << "5. Save Report to File\n";
    std::cout << "6. Sort by Title\n";
    std::cout << "7. Search by Title\n";
    std::cout << "8. Exit\n";
    std::cout << "Choice: ";
}

void Manager::showReport()
{
    if (isEmpty())
    {
        std::cout << "\nNo items yet!\n";
    }
    else
    {
        std::cout << "\n--- Reading Report ---\n";
        std::cout << "Total items: " << getItemCount() << "\n";
        std::cout << "Storage: unordered linked list\n";

        double totalHours = getTotalHours();
        double avgSpeed = getAvgSpeed();

        std::cout << std::fixed << std::setprecision(1);
        std::cout << "Total hours: " << totalHours << "\n";
        std::cout << "Avg speed: " << avgSpeed << " pages/hour\n";

        int hardCount = countByDifficulty(HARD);
        if (hardCount > 0 && totalHours >= 10.0)
        {
            std::cout << "Nice, reading hard books!\n";
        }
        else if (hardCount == 0 && getItemCount() >= 2)
        {
            std::cout << "Try a harder book next!\n";
        }

        std::cout << "\n--- Items ---\n";
        int itemNumber = 1;
        for (ReadingItemListIterator it = items.begin(); it.isValid(); it.next(), itemNumber++)
        {
            std::cout << "\nItem " << itemNumber << ": " << it.getData()->displayName() << "\n";
            it.getData()->print(std::cout);
        }
    }

    if (!recentAdditions.isEmpty())
    {
        std::cout << "\nMost recent addition: " << recentAdditions.peek() << "\n";
    }

    if (!removedTitles.isEmpty())
    {
        std::cout << "\n--- Removed since last report (oldest first) ---\n";
        while (!removedTitles.isEmpty())
        {
            std::cout << "  " << removedTitles.front() << "\n";
            removedTitles.dequeue();
        }
    }
}

void Manager::saveToFile() const
{
    std::ofstream file("report.txt");
    if (!file)
    {
        std::cout << "Could not save file!\n";
        return;
    }

    file << "BOOK TRACKER REPORT\n";
    file << "===================\n\n";
    file << "Total items: " << getItemCount() << "\n\n";

    int itemNumber = 1;
    for (ReadingItemListIterator it = items.begin(); it.isValid(); it.next(), itemNumber++)
    {
        file << "Item " << itemNumber << ": " << it.getData()->displayName() << "\n";
        it.getData()->print(file);
        file << "\n";
    }

    file.close();
    std::cout << "Report saved to report.txt\n";
}

void Manager::run()
{
    showBanner();

    int choice = 0;
    do
    {
        showMenu();
        while (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid! Enter a number: ";
        }

        switch (choice)
        {
        case 1:
            addPrintBook();
            break;
        case 2:
            addAudioBook();
            break;
        case 3:
            removeItemUI();
            break;
        case 4:
            showReport();
            break;
        case 5:
            saveToFile();
            break;
        case 6:
            sortByTitle();
            std::cout << "\nLibrary sorted by title.\n";
            break;
        case 7:
        {
            std::string title = readLine("Enter title to search: ");
            int seqIndex = sequentialSearchByTitle(title);
            if (seqIndex != -1)
            {
                std::cout << "Found via Sequential Search at index " << seqIndex << "\n";
            }
            else
            {
                std::cout << "Not found via Sequential Search.\n";
            }

            sortByTitle();
            int binIndex = binarySearchByTitle(title);
            if (binIndex != -1)
            {
                std::cout << "Found via Binary Search at index " << binIndex
                          << " after sorting by title.\n";
            }
            else
            {
                std::cout << "Not found via Binary Search.\n";
            }
            break;
        }
        case 8:
            std::cout << "\nHappy reading!\n\n";
            break;
        default:
            std::cout << "\nInvalid choice!\n";
        }
    } while (choice != 8);
}
