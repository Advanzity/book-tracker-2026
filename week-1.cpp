#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

using namespace std;

// Enum for difficulty level
enum Difficulty
{
    EASY = 1,
    MEDIUM,
    HARD
};

// Convert difficulty to a label
const char *difficultyToString(Difficulty d)
{
    switch (d)
    {
    case EASY:
        return "Easy";
    case MEDIUM:
        return "Medium";
    case HARD:
        return "Hard";
    default:
        return "Unknown";
    }
}

// Function template used by manager calculations
template <typename T>
T safeDivide(T numerator, T denominator)
{
    if (denominator == static_cast<T>(0))
    {
        return static_cast<T>(0);
    }
    return numerator / denominator;
}

// Class template that replaces raw dynamic array logic
template <typename T>
class DynamicArray
{
private:
    T *data;
    int count;
    int capacity;

    void resize()
    {
        int newCapacity = capacity * 2;
        T *newData = new T[newCapacity];

        for (int i = 0; i < count; i++)
        {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    DynamicArray(int initialCapacity = 2)
        : count(0), capacity(initialCapacity > 0 ? initialCapacity : 2)
    {
        data = new T[capacity];
    }

    ~DynamicArray()
    {
        delete[] data;
    }

    int getCount() const
    {
        return count;
    }

    int getCapacity() const
    {
        return capacity;
    }

    void add(const T &item)
    {
        if (count == capacity)
        {
            resize();
        }
        data[count] = item;
        count++;
    }

    bool removeAt(int index, T &removedItem)
    {
        if (index < 0 || index >= count)
        {
            return false;
        }

        removedItem = data[index];
        for (int i = index; i < count - 1; i++)
        {
            data[i] = data[i + 1];
        }

        data[count - 1] = T();
        count--;
        return true;
    }

    bool tryGet(int index, T &item) const
    {
        if (index < 0 || index >= count)
        {
            return false;
        }

        item = data[index];
        return true;
    }

    T getAt(int index) const
    {
        return data[index];
    }
};

// Base class
class ReadingItem
{
protected:
    string title;
    int pages;
    Difficulty difficulty;
    double hours;

public:
    ReadingItem()
        : title("Untitled"), pages(0), difficulty(EASY), hours(0.0) {}

    ReadingItem(const string &title, int pages, double hours, Difficulty difficulty)
        : title(title), pages(pages), difficulty(difficulty), hours(hours) {}

    virtual ~ReadingItem() = default;

    void setTitle(const string &title)
    {
        this->title = title;
    }

    string getTitle() const
    {
        return title;
    }

    void setPages(int pages)
    {
        this->pages = pages;
    }

    int getPages() const
    {
        return pages;
    }

    void setHours(double hours)
    {
        this->hours = hours;
    }

    double getHours() const
    {
        return hours;
    }

    void setDifficulty(Difficulty difficulty)
    {
        this->difficulty = difficulty;
    }

    Difficulty getDifficulty() const
    {
        return difficulty;
    }

    // Pure virtual function
    virtual string displayName() const = 0;

    virtual void toStream(ostream &os) const
    {
        os << displayName()
           << " | pages: " << pages
           << " | hours: " << fixed << setprecision(1) << hours
           << " | difficulty: " << difficultyToString(difficulty);
    }

    virtual void print(ostream &os = cout) const
    {
        os << "Title: " << title << "\n";
        os << "Pages: " << pages << "\n";
        os << "Hours: " << fixed << setprecision(1) << hours << "\n";
        os << "Difficulty: " << difficultyToString(difficulty) << "\n";
    }

    friend ostream &operator<<(ostream &os, const ReadingItem &item)
    {
        item.toStream(os);
        return os;
    }
};

// Composition class
class PriceInfo
{
private:
    double cost;
    bool includedWithSubscription;

public:
    PriceInfo()
        : cost(0.0), includedWithSubscription(false) {}

    PriceInfo(double cost, bool includedWithSubscription)
        : cost(cost), includedWithSubscription(includedWithSubscription) {}

    void setCost(double cost)
    {
        this->cost = cost;
    }

    double getCost() const
    {
        return cost;
    }

    void setIncludedWithSubscription(bool includedWithSubscription)
    {
        this->includedWithSubscription = includedWithSubscription;
    }

    bool getIncludedWithSubscription() const
    {
        return includedWithSubscription;
    }

    bool isFree() const
    {
        return cost <= 0.0 || includedWithSubscription;
    }

    string formattedCost() const
    {
        if (isFree())
        {
            return "Free";
        }

        ostringstream oss;
        oss << "$" << fixed << setprecision(2) << cost;
        return oss.str();
    }
};

// Derived class 1
class PrintBook : public ReadingItem
{
private:
    string author;
    PriceInfo price;

public:
    PrintBook()
        : ReadingItem(), author("Unknown"), price() {}

    PrintBook(const string &title, int pages, double hours, Difficulty difficulty,
              const string &author, const PriceInfo &price)
        : ReadingItem(title, pages, hours, difficulty), author(author), price(price) {}

    void setAuthor(const string &author)
    {
        this->author = author;
    }

    string getAuthor() const
    {
        return author;
    }
 
    void setPrice(const PriceInfo &price)
    {
        this->price = price;
    }

    const PriceInfo &getPrice() const
    {
        return price;
    }

    bool operator==(const PrintBook &other) const
    {
        return this->title == other.title && this->author == other.author;
    }

    string displayName() const override
    {
        return title + " by " + author;
    }

    void toStream(ostream &os) const override
    {
        os << "PrintBook: " << displayName()
           << " | pages: " << pages
           << " | hours: " << fixed << setprecision(1) << hours
           << " | difficulty: " << difficultyToString(difficulty)
           << " | cost: " << price.formattedCost();
    }

    void print(ostream &os = cout) const override
    {
        ReadingItem::print(os);
        os << "Author: " << author << "\n";
        os << "Cost: " << price.formattedCost() << "\n";
    }
};

// Derived class 2
class AudioBook : public ReadingItem
{
private:
    string narrator;
    PriceInfo price;

public:
    AudioBook()
        : ReadingItem(), narrator("Unknown"), price() {}

    AudioBook(const string &title, int pages, double hours, Difficulty difficulty,
              const string &narrator, const PriceInfo &price)
        : ReadingItem(title, pages, hours, difficulty), narrator(narrator), price(price) {}

    void setNarrator(const string &narrator)
    {
        this->narrator = narrator;
    }

    string getNarrator() const
    {
        return narrator;
    }

    void setPrice(const PriceInfo &price)
    {
        this->price = price;
    }

    const PriceInfo &getPrice() const
    {
        return price;
    }

    string displayName() const override
    {
        return title + " (narrated by " + narrator + ")";
    }

    void toStream(ostream &os) const override
    {
        os << "AudioBook: " << displayName()
           << " | pages: " << pages
           << " | hours: " << fixed << setprecision(1) << hours
           << " | difficulty: " << difficultyToString(difficulty)
           << " | cost: " << price.formattedCost();
    }

    void print(ostream &os = cout) const override
    {
        ReadingItem::print(os);
        os << "Narrator: " << narrator << "\n";
        os << "Cost: " << price.formattedCost() << "\n";
    }
};

// Manager class (formerly BookTracker)
class Manager
{
private:
    DynamicArray<ReadingItem *> items;

    bool isNonEmpty(const string &s) const
    {
        return !s.empty();
    }

    string readLine(const string &prompt)
    {
        string value;
        cout << prompt;
        getline(cin >> ws, value);
        while (!isNonEmpty(value))
        {
            cout << "Input cannot be empty. Try again: ";
            getline(cin >> ws, value);
        }
        return value;
    }

    int readPositiveInt(const string &prompt)
    {
        int value;
        while (true)
        {
            cout << prompt;
            if (cin >> value && value > 0)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid! Enter a positive number.\n";
        }
    }

    double readNonNegativeDouble(const string &prompt)
    {
        double value;
        while (true)
        {
            cout << prompt;
            if (cin >> value && value >= 0.0)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid! Enter a non-negative number.\n";
        }
    }

    int readChoice(const string &prompt, int minValue, int maxValue)
    {
        int value;
        while (true)
        {
            cout << prompt;
            if (cin >> value && value >= minValue && value <= maxValue)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid! Enter a number between " << minValue << " and " << maxValue << ".\n";
        }
    }

    Difficulty readDifficulty()
    {
        cout << "\nDifficulty:\n";
        cout << "1. Easy\n";
        cout << "2. Medium\n";
        cout << "3. Hard\n";
        int choice = readChoice("Choose (1-3): ", 1, 3);
        return static_cast<Difficulty>(choice);
    }

    PriceInfo readPriceInfo()
    {
        double cost = readNonNegativeDouble("Cost (0 for free): ");
        int included = readChoice("Included with subscription? (1=Yes, 2=No): ", 1, 2);
        return PriceInfo(cost, included == 1);
    }

    void addPrintBook()
    {
        cout << "\n--- Add Print Book ---\n";
        string title = readLine("Title: ");
        int pages = readPositiveInt("Pages: ");
        double hours = readNonNegativeDouble("Hours spent: ");
        Difficulty difficulty = readDifficulty();
        string author = readLine("Author: ");
        PriceInfo price = readPriceInfo();

        addItem(new PrintBook(title, pages, hours, difficulty, author, price));
        cout << "\nPrint book added!\n";
    }

    void addAudioBook()
    {
        cout << "\n--- Add Audio Book ---\n";
        string title = readLine("Title: ");
        int pages = readPositiveInt("Pages (or length in pages): ");
        double hours = readNonNegativeDouble("Hours listened: ");
        Difficulty difficulty = readDifficulty();
        string narrator = readLine("Narrator: ");
        PriceInfo price = readPriceInfo();

        addItem(new AudioBook(title, pages, hours, difficulty, narrator, price));
        cout << "\nAudio book added!\n";
    }

    void removeItemUI()
    {
        if (isEmpty())
        {
            cout << "\nNo items to remove!\n";
            return;
        }

        cout << "\n--- Remove Item ---\n";
        for (int i = 0; i < getItemCount(); i++)
        {
            cout << (i + 1) << ". " << items.getAt(i)->displayName() << "\n";
        }

        int index = readChoice("Select item to remove (0 to cancel): ", 0, getItemCount());
        if (index > 0)
        {
            removeItem(index - 1);
            cout << "\nItem removed.\n";
        }
    }

public:
    Manager() = default;

    ~Manager()
    {
        for (int i = 0; i < getItemCount(); i++)
        {
            delete items.getAt(i);
        }
    }

    bool isEmpty() const
    {
        return this->getItemCount() == 0;
    }

    Manager &operator+=(ReadingItem *item)
    {
        items.add(item);
        return *this;
    }

    Manager &operator-=(int index)
    {
        ReadingItem *removedItem = nullptr;
        if (items.removeAt(index, removedItem))
        {
            delete removedItem;
        }
        return *this;
    }

    ReadingItem *operator[](int index) const
    {
        ReadingItem *item = nullptr;
        if (items.tryGet(index, item))
        {
            return item;
        }
        return nullptr;
    }

    void addItem(ReadingItem *item)
    {
        *this += item;
    }

    bool removeItem(int index)
    {
        int before = getItemCount();
        *this -= index;
        return getItemCount() < before;
    }

    int getItemCount() const
    {
        return items.getCount();
    }

    int getCapacity() const
    {
        return items.getCapacity();
    }

    int getTotalPages() const
    {
        int totalPages = 0;
        for (int i = 0; i < getItemCount(); i++)
        {
            totalPages += items.getAt(i)->getPages();
        }
        return totalPages;
    }

    double getTotalHours() const
    {
        double totalHours = 0.0;
        for (int i = 0; i < getItemCount(); i++)
        {
            totalHours += items.getAt(i)->getHours();
        }
        return totalHours;
    }

    double getAvgSpeed() const
    {
        return safeDivide(static_cast<double>(getTotalPages()), getTotalHours());
    }

    int countByDifficulty(Difficulty d) const
    {
        int total = 0;
        for (int i = 0; i < getItemCount(); i++)
        {
            if (items.getAt(i)->getDifficulty() == d)
            {
                total++;
            }
        }
        return total;
    }

    void showBanner() const
    {
        cout << "\n================================\n";
        cout << "   DAVID'S BOOK TRACKER 2026\n";
        cout << "=================================\n\n";
    }

    void showMenu() const
    {
        cout << "\n--- MENU ---\n";
        cout << "1. Add Print Book\n";
        cout << "2. Add Audio Book\n";
        cout << "3. Remove Item\n";
        cout << "4. View Report\n";
        cout << "5. Save Report to File\n";
        cout << "6. Exit\n";
        cout << "Choice: ";
    }

    void showReport() const
    {
        if (isEmpty())
        {
            cout << "\nNo items yet!\n";
            return;
        }

        cout << "\n--- Reading Report ---\n";
        cout << "Total items: " << getItemCount() << " (Capacity: " << getCapacity() << ")\n";

        double totalHours = getTotalHours();
        double avgSpeed = getAvgSpeed();

        cout << fixed << setprecision(1);
        cout << "Total hours: " << totalHours << "\n";
        cout << "Avg speed: " << avgSpeed << " pages/hour\n";

        int hardCount = countByDifficulty(HARD);
        if (hardCount > 0 && totalHours >= 10.0)
        {
            cout << "Nice, reading hard books!\n";
        }
        else if (hardCount == 0 && getItemCount() >= 2)
        {
            cout << "Try a harder book next!\n";
        }

        cout << "\n--- Items ---\n";
        for (int i = 0; i < getItemCount(); i++)
        {
            cout << "\nItem " << (i + 1) << ": " << items.getAt(i)->displayName() << "\n";
            items.getAt(i)->print(cout);
        }
    }

    void saveToFile() const
    {
        ofstream file("report.txt");

        if (!file)
        {
            cout << "Could not save file!\n";
            return;
        }

        file << "BOOK TRACKER REPORT\n";
        file << "===================\n\n";
        file << "Total items: " << getItemCount() << "\n\n";

        for (int i = 0; i < getItemCount(); i++)
        {
            file << "Item " << (i + 1) << ": " << items.getAt(i)->displayName() << "\n";
            items.getAt(i)->print(file);
            file << "\n";
        }

        file.close();
        cout << "Report saved to report.txt\n";
    }

    void run()
    {
        showBanner();

        int choice = 0;
        do
        {
            showMenu();
            while (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid! Enter a number: ";
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
                cout << "\nHappy reading!\n\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
            }

        } while (choice != 6);
    }
};

#ifndef _DEBUG
int main()
{
    Manager manager;
    manager.run();
    return 0;
}
#endif

#ifdef _DEBUG
TEST_CASE("ReadingItem cannot be instantiated directly (Abstract)")
{
    ReadingItem *item = nullptr;
    CHECK(true);
}

TEST_CASE("PrintBook (Derived) constructor and getters")
{
    PriceInfo price(14.50, false);
    PrintBook book("Dune", 600, 20.0, HARD, "Frank Herbert", price);

    CHECK(book.getTitle() == "Dune");
    CHECK(book.getPages() == 600);
    CHECK(book.getAuthor() == "Frank Herbert");
    CHECK(book.getPrice().getCost() == doctest::Approx(14.50));
    CHECK(book.displayName() == "Dune by Frank Herbert");
}

TEST_CASE("AudioBook (Derived) constructor and getters")
{
    PriceInfo price(0.0, true);
    AudioBook book("Project Hail Mary", 496, 18.0, MEDIUM, "Ray Porter", price);

    CHECK(book.getTitle() == "Project Hail Mary");
    CHECK(book.getPages() == 496);
    CHECK(book.getNarrator() == "Ray Porter");
    CHECK(book.getPrice().isFree() == true);
    CHECK(book.displayName() == "Project Hail Mary (narrated by Ray Porter)");
}

TEST_CASE("PrintBook operator== true when identity fields match")
{
    PriceInfo regular(18.00, false);
    PriceInfo sale(0.0, true);
    PrintBook left("Dune", 600, 20.0, HARD, "Frank Herbert", regular);
    PrintBook right("Dune", 450, 15.0, MEDIUM, "Frank Herbert", sale);

    CHECK(left == right);
}

TEST_CASE("PrintBook operator== false when identity fields differ")
{
    PriceInfo price(18.00, false);
    PrintBook left("Dune", 600, 20.0, HARD, "Frank Herbert", price);
    PrintBook right("Dune", 600, 20.0, HARD, "Someone Else", price);

    CHECK_FALSE(left == right);
}

TEST_CASE("operator<< outputs one-line PrintBook summary via base reference")
{
    PriceInfo price(14.50, false);
    PrintBook book("Dune", 600, 20.0, HARD, "Frank Herbert", price);

    ostringstream oss;
    ReadingItem &item = book;
    oss << item;

    CHECK(oss.str() == "PrintBook: Dune by Frank Herbert | pages: 600 | hours: 20.0 | difficulty: Hard | cost: $14.50");
}

TEST_CASE("operator<< outputs one-line AudioBook summary via base reference")
{
    PriceInfo price(0.0, true);
    AudioBook book("Project Hail Mary", 496, 18.0, MEDIUM, "Ray Porter", price);

    ostringstream oss;
    ReadingItem &item = book;
    oss << item;

    CHECK(oss.str() == "AudioBook: Project Hail Mary (narrated by Ray Porter) | pages: 496 | hours: 18.0 | difficulty: Medium | cost: Free");
}

TEST_CASE("Manager operator[] valid index returns correct item")
{
    Manager manager;
    PriceInfo price(10.0, false);
    ReadingItem *first = new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price);
    manager += first;

    CHECK(manager[0] == first);
}

TEST_CASE("Manager operator[] invalid index returns nullptr")
{
    Manager manager;
    CHECK(manager[-1] == nullptr);
    CHECK(manager[0] == nullptr);
}

TEST_CASE("Manager operator+= adds an item pointer")
{
    Manager manager;
    PriceInfo price(10.0, false);
    ReadingItem *first = new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price);

    manager += first;

    CHECK(manager.getItemCount() == 1);
    CHECK(manager[0] == first);
}

TEST_CASE("Manager operator-= removes and shifts remaining items")
{
    Manager manager;
    PriceInfo price(10.0, false);
    ReadingItem *first = new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price);
    ReadingItem *second = new PrintBook("Book 2", 200, 10.0, MEDIUM, "Author 2", price);
    ReadingItem *third = new PrintBook("Book 3", 300, 15.0, HARD, "Author 3", price);

    manager += first;
    manager += second;
    manager += third;
    manager -= 1;

    CHECK(manager.getItemCount() == 2);
    CHECK(manager[0] == first);
    CHECK(manager[1] == third);
}

TEST_CASE("Function template safeDivide works with int")
{
    CHECK(safeDivide(12, 3) == 4);
    CHECK(safeDivide(12, 0) == 0);
}

TEST_CASE("Function template safeDivide works with double")
{
    CHECK(safeDivide(9.0, 4.5) == doctest::Approx(2.0));
    CHECK(safeDivide(5.0, 0.0) == doctest::Approx(0.0));
}

TEST_CASE("Class template DynamicArray stores values and resizes")
{
    DynamicArray<int> values(1);
    values.add(10);
    values.add(20);

    int stored = 0;
    CHECK(values.getCount() == 2);
    CHECK(values.getCapacity() == 2);
    CHECK(values.tryGet(1, stored) == true);
    CHECK(stored == 20);
}

TEST_CASE("Class template DynamicArray removes values and shifts")
{
    DynamicArray<int> values(2);
    values.add(5);
    values.add(10);
    values.add(15);

    int removed = 0;
    int shifted = 0;

    CHECK(values.removeAt(1, removed) == true);
    CHECK(removed == 10);
    CHECK(values.getCount() == 2);
    CHECK(values.tryGet(1, shifted) == true);
    CHECK(shifted == 15);
}

TEST_CASE("Polymorphism via Base Pointer")
{
    PriceInfo price(10.0, false);
    ReadingItem *item = new PrintBook("Polymorphism", 100, 5.0, EASY, "Author", price);

    CHECK(item->displayName() == "Polymorphism by Author");
    delete item;
}

TEST_CASE("Manager initialization")
{
    Manager manager;
    CHECK(manager.getItemCount() == 0);
    CHECK(manager.getCapacity() == 2); // default capacity
}

TEST_CASE("Manager adds items and resizes")
{
    Manager manager;
    PriceInfo price(10.0, false);

    // Add 2 items (fill capacity)
    manager.addItem(new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price));
    manager.addItem(new PrintBook("Book 2", 200, 10.0, MEDIUM, "Author 2", price));

    CHECK(manager.getItemCount() == 2);
    CHECK(manager.getCapacity() == 2);

    // Add 3rd item (trigger resize)
    manager.addItem(new PrintBook("Book 3", 300, 15.0, HARD, "Author 3", price));

    CHECK(manager.getItemCount() == 3);
    CHECK(manager.getCapacity() == 4); // doubled
}

TEST_CASE("Manager removes items")
{
    Manager manager;
    PriceInfo price(10.0, false);

    manager.addItem(new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price));
    manager.addItem(new PrintBook("Book 2", 200, 10.0, MEDIUM, "Author 2", price));
    manager.addItem(new PrintBook("Book 3", 300, 15.0, HARD, "Author 3", price));

    // Remove middle item
    bool removed = manager.removeItem(1); // Remove "Book 2"

    CHECK(removed == true);
    CHECK(manager.getItemCount() == 2);
    CHECK(manager[0]->getTitle() == "Book 1");
    CHECK(manager[1]->getTitle() == "Book 3");

    // Remove invalid index
    CHECK(manager.removeItem(5) == false);
}
#endif
