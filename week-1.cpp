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
enum Difficulty {
    EASY = 1,
    MEDIUM,
    HARD
};

// Convert difficulty to a label
const char* difficultyToString(Difficulty d) {
    switch (d) {
    case EASY: return "Easy";
    case MEDIUM: return "Medium";
    case HARD: return "Hard";
    default: return "Unknown";
    }
}

// Base class
class ReadingItem {
protected:
    string title;
    int pages;
    Difficulty difficulty;
    double hours;

public:
    ReadingItem()
        : title("Untitled"), pages(0), difficulty(EASY), hours(0.0) {}

    ReadingItem(const string& title, int pages, double hours, Difficulty difficulty)
        : title(title), pages(pages), difficulty(difficulty), hours(hours) {}

    virtual ~ReadingItem() = default;

    void setTitle(const string& title) {
        this->title = title;
    }

    string getTitle() const {
        return title;
    }

    void setPages(int pages) {
        this->pages = pages;
    }

    int getPages() const {
        return pages;
    }

    void setHours(double hours) {
        this->hours = hours;
    }

    double getHours() const {
        return hours;
    }

    void setDifficulty(Difficulty difficulty) {
        this->difficulty = difficulty;
    }

    Difficulty getDifficulty() const {
        return difficulty;
    }

    virtual void print(ostream& os = cout) const {
        os << "Title: " << title << "\n";
        os << "Pages: " << pages << "\n";
        os << "Hours: " << fixed << setprecision(1) << hours << "\n";
        os << "Difficulty: " << difficultyToString(difficulty) << "\n";
    }
};

// Composition class
class PriceInfo {
private:
    double cost;
    bool includedWithSubscription;

public:
    PriceInfo()
        : cost(0.0), includedWithSubscription(false) {}

    PriceInfo(double cost, bool includedWithSubscription)
        : cost(cost), includedWithSubscription(includedWithSubscription) {}

    void setCost(double cost) {
        this->cost = cost;
    }

    double getCost() const {
        return cost;
    }

    void setIncludedWithSubscription(bool includedWithSubscription) {
        this->includedWithSubscription = includedWithSubscription;
    }

    bool getIncludedWithSubscription() const {
        return includedWithSubscription;
    }

    bool isFree() const {
        return cost <= 0.0 || includedWithSubscription;
    }

    string formattedCost() const {
        if (isFree()) {
            return "Free";
        }

        ostringstream oss;
        oss << "$" << fixed << setprecision(2) << cost;
        return oss.str();
    }
};

// Derived class 1
class PrintBook : public ReadingItem {
private:
    string author;
    PriceInfo price;

public:
    PrintBook()
        : ReadingItem(), author("Unknown"), price() {}

    PrintBook(const string& title, int pages, double hours, Difficulty difficulty,
        const string& author, const PriceInfo& price)
        : ReadingItem(title, pages, hours, difficulty), author(author), price(price) {}

    void setAuthor(const string& author) {
        this->author = author;
    }

    string getAuthor() const {
        return author;
    }

    void setPrice(const PriceInfo& price) {
        this->price = price;
    }

    const PriceInfo& getPrice() const {
        return price;
    }

    string displayName() const {
        return title + " by " + author;
    }

    void print(ostream& os = cout) const override {
        ReadingItem::print(os);
        os << "Author: " << author << "\n";
        os << "Cost: " << price.formattedCost() << "\n";
    }
};

// Derived class 2
class AudioBook : public ReadingItem {
private:
    string narrator;
    PriceInfo price;

public:
    AudioBook()
        : ReadingItem(), narrator("Unknown"), price() {}

    AudioBook(const string& title, int pages, double hours, Difficulty difficulty,
        const string& narrator, const PriceInfo& price)
        : ReadingItem(title, pages, hours, difficulty), narrator(narrator), price(price) {}

    void setNarrator(const string& narrator) {
        this->narrator = narrator;
    }

    string getNarrator() const {
        return narrator;
    }

    void setPrice(const PriceInfo& price) {
        this->price = price;
    }

    const PriceInfo& getPrice() const {
        return price;
    }

    string displayName() const {
        return title + " (narrated by " + narrator + ")";
    }

    void print(ostream& os = cout) const override {
        ReadingItem::print(os);
        os << "Narrator: " << narrator << "\n";
        os << "Cost: " << price.formattedCost() << "\n";
    }
};

// BookTracker class
class BookTracker {
private:
    static const int MAX_ITEMS = 5;
    ReadingItem* items[MAX_ITEMS];
    int count;

    bool isNonEmpty(const string& s) const {
        return !s.empty();
    }

    string readLine(const string& prompt) {
        string value;
        cout << prompt;
        getline(cin >> ws, value);
        while (!isNonEmpty(value)) {
            cout << "Input cannot be empty. Try again: ";
            getline(cin >> ws, value);
        }
        return value;
    }

    int readPositiveInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value && value > 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid! Enter a positive number.\n";
        }
    }

    double readNonNegativeDouble(const string& prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= 0.0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid! Enter a non-negative number.\n";
        }
    }

    int readChoice(const string& prompt, int minValue, int maxValue) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= minValue && value <= maxValue) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid! Enter a number between " << minValue << " and " << maxValue << ".\n";
        }
    }

    Difficulty readDifficulty() {
        cout << "\nDifficulty:\n";
        cout << "1. Easy\n";
        cout << "2. Medium\n";
        cout << "3. Hard\n";
        int choice = readChoice("Choose (1-3): ", 1, 3);
        return static_cast<Difficulty>(choice);
    }

    PriceInfo readPriceInfo() {
        double cost = readNonNegativeDouble("Cost (0 for free): ");
        int included = readChoice("Included with subscription? (1=Yes, 2=No): ", 1, 2);
        return PriceInfo(cost, included == 1);
    }

    void addPrintBook() {
        if (count >= MAX_ITEMS) {
            cout << "\nCannot add more items!\n";
            return;
        }

        cout << "\n--- Add Print Book ---\n";
        string title = readLine("Title: ");
        int pages = readPositiveInt("Pages: ");
        double hours = readNonNegativeDouble("Hours spent: ");
        Difficulty difficulty = readDifficulty();
        string author = readLine("Author: ");
        PriceInfo price = readPriceInfo();

        if (addItem(new PrintBook(title, pages, hours, difficulty, author, price))) {
            cout << "\nPrint book added!\n";
        }
    }

    void addAudioBook() {
        if (count >= MAX_ITEMS) {
            cout << "\nCannot add more items!\n";
            return;
        }

        cout << "\n--- Add Audio Book ---\n";
        string title = readLine("Title: ");
        int pages = readPositiveInt("Pages (or length in pages): ");
        double hours = readNonNegativeDouble("Hours listened: ");
        Difficulty difficulty = readDifficulty();
        string narrator = readLine("Narrator: ");
        PriceInfo price = readPriceInfo();

        if (addItem(new AudioBook(title, pages, hours, difficulty, narrator, price))) {
            cout << "\nAudio book added!\n";
        }
    }

public:
    BookTracker()
        : count(0) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            items[i] = nullptr;
        }
    }

    ~BookTracker() {
        for (int i = 0; i < count; i++) {
            delete items[i];
            items[i] = nullptr;
        }
    }

    bool addItem(ReadingItem* item) {
        if (item == nullptr || count >= MAX_ITEMS) {
            delete item;
            return false;
        }
        items[count] = item;
        count++;
        return true;
    }

    int getItemCount() const {
        return count;
    }

    int getTotalPages() const {
        int totalPages = 0;
        for (int i = 0; i < count; i++) {
            totalPages += items[i]->getPages();
        }
        return totalPages;
    }

    double getTotalHours() const {
        double totalHours = 0.0;
        for (int i = 0; i < count; i++) {
            totalHours += items[i]->getHours();
        }
        return totalHours;
    }

    double getAvgSpeed() const {
        double totalHours = getTotalHours();
        if (totalHours <= 0.0) {
            return 0.0;
        }
        return static_cast<double>(getTotalPages()) / totalHours;
    }

    int countByDifficulty(Difficulty d) const {
        int total = 0;
        for (int i = 0; i < count; i++) {
            if (items[i]->getDifficulty() == d) {
                total++;
            }
        }
        return total;
    }

    void showBanner() const {
        cout << "\n================================\n";
        cout << "   DAVID'S BOOK TRACKER 2026\n";
        cout << "=================================\n\n";
    }

    void showMenu() const {
        cout << "\n--- MENU ---\n";
        cout << "1. Add Print Book\n";
        cout << "2. Add Audio Book\n";
        cout << "3. View Report\n";
        cout << "4. Save Report to File\n";
        cout << "5. Exit\n";
        cout << "Choice: ";
    }

    void showReport() const {
        if (count == 0) {
            cout << "\nNo items yet!\n";
            return;
        }

        cout << "\n--- Reading Report ---\n";
        cout << "Total items: " << count << "\n";

        double totalHours = getTotalHours();
        double avgSpeed = getAvgSpeed();

        cout << fixed << setprecision(1);
        cout << "Total hours: " << totalHours << "\n";
        cout << "Avg speed: " << avgSpeed << " pages/hour\n";

        int hardCount = countByDifficulty(HARD);
        if (hardCount > 0 && totalHours >= 10.0) {
            cout << "Nice, reading hard books!\n";
        }
        else if (hardCount == 0 && count >= 2) {
            cout << "Try a harder book next!\n";
        }

        cout << "\n--- Items ---\n";
        for (int i = 0; i < count; i++) {
            cout << "\nItem " << (i + 1) << ":\n";
            items[i]->print(cout);
        }
    }

    void saveToFile() const {
        ofstream file("report.txt");

        if (!file) {
            cout << "Could not save file!\n";
            return;
        }

        file << "BOOK TRACKER REPORT\n";
        file << "===================\n\n";
        file << "Total items: " << count << "\n\n";

        for (int i = 0; i < count; i++) {
            file << "Item " << (i + 1) << ":\n";
            items[i]->print(file);
            file << "\n";
        }

        file.close();
        cout << "Report saved to report.txt\n";
    }

    void run() {
        showBanner();

        int choice = 0;
        do {
            showMenu();
            while (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid! Enter a number: ";
            }

            switch (choice) {
            case 1:
                addPrintBook();
                break;
            case 2:
                addAudioBook();
                break;
            case 3:
                showReport();
                break;
            case 4:
                saveToFile();
                break;
            case 5:
                cout << "\nHappy reading!\n\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
            }

        } while (choice != 5);
    }
};

#ifndef _DEBUG
int main() {
    BookTracker tracker;
    tracker.run();
    return 0;
}
#endif

#ifdef _DEBUG
TEST_CASE("ReadingItem constructor initializes fields") {
    ReadingItem item("C++ Primer", 500, 25.0, MEDIUM);
    CHECK(item.getTitle() == "C++ Primer");
    CHECK(item.getPages() == 500);
    CHECK(item.getHours() == doctest::Approx(25.0));
    CHECK(item.getDifficulty() == MEDIUM);
}

TEST_CASE("ReadingItem setters update fields") {
    ReadingItem item;
    item.setTitle("Refactoring");
    item.setPages(350);
    item.setHours(12.5);
    item.setDifficulty(HARD);

    CHECK(item.getTitle() == "Refactoring");
    CHECK(item.getPages() == 350);
    CHECK(item.getHours() == doctest::Approx(12.5));
    CHECK(item.getDifficulty() == HARD);
}

TEST_CASE("PriceInfo helper behavior") {
    PriceInfo freeItem(0.0, false);
    PriceInfo paidItem(9.99, false);
    PriceInfo subscriptionItem(12.00, true);

    CHECK(freeItem.isFree() == true);
    CHECK(paidItem.isFree() == false);
    CHECK(subscriptionItem.isFree() == true);
    CHECK(paidItem.formattedCost() == "$9.99");
}

TEST_CASE("PrintBook constructor and getters") {
    PriceInfo price(14.50, false);
    PrintBook book("Dune", 600, 20.0, HARD, "Frank Herbert", price);

    CHECK(book.getTitle() == "Dune");
    CHECK(book.getPages() == 600);
    CHECK(book.getAuthor() == "Frank Herbert");
    CHECK(book.getPrice().getCost() == doctest::Approx(14.50));
    CHECK(book.displayName() == "Dune by Frank Herbert");
}

TEST_CASE("AudioBook constructor and getters") {
    PriceInfo price(0.0, true);
    AudioBook book("Project Hail Mary", 496, 18.0, MEDIUM, "Ray Porter", price);

    CHECK(book.getTitle() == "Project Hail Mary");
    CHECK(book.getPages() == 496);
    CHECK(book.getNarrator() == "Ray Porter");
    CHECK(book.getPrice().isFree() == true);
    CHECK(book.displayName() == "Project Hail Mary (narrated by Ray Porter)");
}

TEST_CASE("Derived print includes base fields") {
    PriceInfo price(12.00, false);
    PrintBook book("Dune", 500, 15.0, HARD, "Frank Herbert", price);

    ostringstream oss;
    book.print(oss);

    string output = oss.str();
    CHECK(output.find("Title: Dune") != string::npos);
    CHECK(output.find("Pages: 500") != string::npos);
    CHECK(output.find("Difficulty: Hard") != string::npos);
    CHECK(output.find("Author: Frank Herbert") != string::npos);
    CHECK(output.find("Cost: $12.00") != string::npos);
}
#endif
