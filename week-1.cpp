#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
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

// Struct to represent a book
struct Book {
    string title;
    int pages;
    double hours;
    Difficulty difficulty; // 1 2 3
};

// BookTracker class
class BookTracker {
private:
    static const int MAX_BOOKS = 5;
    Book books[MAX_BOOKS];
    int count;

    /*
    validation function (non-empty string) 
     @string Takes a string reference
    */
    bool isNonEmpty(const string& s) {
        return !s.empty();
    }

public:
    // Constructor
    BookTracker() {
        count = 0;
    }

    // Add a book without cin (test-friendly)
    bool addBook(const Book& b) {
        if (count >= MAX_BOOKS) {
            return false;
        }
        if (!isNonEmpty(b.title)) {
            return false;
        }
        if (b.pages <= 0) {
            return false;
        }
        if (b.hours < 0) {
            return false;
        }
        if (b.difficulty < EASY || b.difficulty > HARD) {
            return false;
        }
        books[count] = b;
        count++;
        return true;
    }

    // Getters for tests/reports
    int getBookCount() const {
        return count;
    }

    double getTotalPages() const {
        double totalPages = 0;
        for (int i = 0; i < count; i++) {
            totalPages += books[i].pages;
        }
        return totalPages;
    }

    double getTotalHours() const {
        double totalHours = 0;
        for (int i = 0; i < count; i++) {
            totalHours += books[i].hours;
        }
        return totalHours;
    }

    double getAvgSpeed() const {
        double totalHours = getTotalHours();
        if (totalHours <= 0) {
            return 0.0;
        }
        return getTotalPages() / totalHours;
    }

    int countByDifficulty(Difficulty d) const {
        int total = 0;
        for (int i = 0; i < count; i++) {
            if (books[i].difficulty == d) {
                total++;
            }
        }
        return total;
    }

    // Display banner
    void showBanner() {
        cout << "\n================================\n";
        cout << "   DAVID'S BOOK TRACKER 2026\n";
        cout << "=================================\n\n";
    }

    // Display menu
    void showMenu() {
        cout << "\n--- MENU ---\n";
        cout << "1. Add Book\n";
        cout << "2. View Report\n";
        cout << "3. Save Report to File\n";
        cout << "4. Exit\n";             
        cout << "Choice: ";
    }

    // Add a book
    void addBook() {
        if (count >= MAX_BOOKS) {
            cout << "\nCannot add more books!\n";
            return;
        }

        cout << "\n--- Add Book ---\n";

        cin.ignore();

        // Get title (string with spaces)
        cout << "Title: ";
        getline(cin, books[count].title);

        // todo: implement isNonEmpty(&str)
        while (!isNonEmpty(books[count].title)) {
            cout << "Title cannot be empty. Try again: ";
            getline(cin, books[count].title);
        }

        // not stored in struct (yet)

        // Get pages (int)
        cout << "Total pages: ";
        while (!(cin >> books[count].pages) || books[count].pages <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid! Enter a positive number: ";
        }

        // Get hours (double)
        cout << "Hours spent: ";
        while (!(cin >> books[count].hours) || books[count].hours < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid! Enter a non-negative number: ";
        }

        // Get difficulty for the book
        int choice;
        cout << "\nDifficulty:\n";
        cout << "1. Easy\n";
        cout << "2. Medium\n";
        cout << "3. Hard\n";
        cout << "Choose (1-3): ";

        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid! Enter 1-3: ";
        }
        books[count].difficulty = static_cast<Difficulty>(choice);

        count++;
        cout << "\nBook added!\n";
    }

    // Show report
    void showReport() const {
        if (count == 0) {
            cout << "\nNo books yet!\n";
            return;
        }

        cout << "\n--- Reading Report ---\n";
        cout << "Total books: " << count << "\n";

        // Calculate average pages per hour
        double totalPages = getTotalPages();
        double totalHours = getTotalHours();
        double avgSpeed = getAvgSpeed();

        cout << fixed << setprecision(1);
        cout << "Total hours: " << totalHours << "\n";
        cout << "Avg speed " << avgSpeed << " pages/hour\n\n";

        // Show books in table
        // 10 + 8 + 20 = 38
        // ---- ---- --- --- --- ---

        cout << left << setw(20) << "Title"
            << right << setw(8) << "Pages"
            << setw(10) << "Hours" << "\n";
        cout << string(38, '-') << "\n";

        for (int i = 0; i < count; i++) {
            cout << left << setw(20) << books[i].title
                << right << setw(8) << books[i].pages
                << setw(10) << fixed << setprecision(1)
                << books[i].hours << "\n";
        }

        // Give recommendation based on difficulty
        int hardCount = countByDifficulty(HARD);

        cout << "\n";
        if (hardCount > 0 && totalHours >= 10.0) {
            cout << "Nice, reading hard books!\n";
        }
        else if (hardCount == 0 && count >= 2) {
            cout << "Try a harder book next!\n";
        }

        if (avgSpeed >= 20.0 && totalHours >= 5.0) {
            cout << "Your speed is good stay consistent\n";
        }
        else if (avgSpeed < 10.0 && totalHours >= 5.0) {
            cout << "Read in smaller sessions.\n";
        }
        else {
            cout << "Keep tracking.\n";
        }
    }

    void saveToFile() {
        ofstream file("report.txt");

        if (!file) {
            cout << "Could not save file!\n";
            return;
        }

        file << "BOOK TRACKER REPORT\n";
        file << "===================\n\n";
        file << "Total books: " << count << "\n\n";

        file << left << setw(20) << "Title"
            << right << setw(8) << "Pages"
            << setw(10) << "Hours" << "\n";
        file << string(38, '-') << "\n";

        for (int i = 0; i < count; i++) {
            file << left << setw(20) << books[i].title.substr(0, 19)
                << right << setw(8) << books[i].pages
                << setw(10) << fixed << setprecision(1)
                << books[i].hours << "\n";
        }

        file.close();
        cout << "Report saved to report.txt\n";
    }

    // Main program loop
    void run() {
        showBanner();

        int choice = 0;

        // Do-while loop
        do {
            showMenu();

            // While loop for validation
            while (!(cin >> choice)) {
                cin.clear();
                //cin.ignore(1000);
                cin.ignore(1000, '\n');
                cout << "Invalid! Enter a number: ";
            }

            // Switch statement
            switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                showReport();
                break;
            case 3:            
                saveToFile();
                break;
            case 4:            
                cout << "\nHappy reading!\n\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
            }

        } while (choice != 4); 

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
static Book makeBook(const string& title, int pages, double hours, Difficulty diff) {
    Book b;
    b.title = title;
    b.pages = pages;
    b.hours = hours;
    b.difficulty = diff;
    return b;
}

TEST_CASE("BookTracker starts empty") {
    BookTracker tracker;
    CHECK(tracker.getBookCount() == 0);
    CHECK(tracker.getTotalPages() == 0);
    CHECK(tracker.getTotalHours() == 0);
    CHECK(tracker.getAvgSpeed() == doctest::Approx(0.0));
}

TEST_CASE("addBook accepts valid input") {
    BookTracker tracker;
    Book b = makeBook("C++ Primer", 500, 25.0, MEDIUM);
    CHECK(tracker.addBook(b) == true);
    CHECK(tracker.getBookCount() == 1);
}

TEST_CASE("Single book totals and average") {
    BookTracker tracker;
    tracker.addBook(makeBook("Solo Book", 150, 5.0, MEDIUM));
    CHECK(tracker.getTotalPages() == 150);
    CHECK(tracker.getTotalHours() == doctest::Approx(5.0));
    CHECK(tracker.getAvgSpeed() == doctest::Approx(30.0));
}

TEST_CASE("addBook rejects invalid input") {
    BookTracker tracker;
    Book badPages = makeBook("Bad Pages", -10, 2.0, EASY);
    Book badHours = makeBook("Bad Hours", 100, -1.0, EASY);
    Book badTitle = makeBook("", 100, 2.0, EASY);
    CHECK(tracker.addBook(badPages) == false);
    CHECK(tracker.addBook(badHours) == false);
    CHECK(tracker.addBook(badTitle) == false);
    CHECK(tracker.getBookCount() == 0);
}

TEST_CASE("Total pages sums across books") {
    BookTracker tracker;
    tracker.addBook(makeBook("Book A", 120, 3.0, EASY));
    tracker.addBook(makeBook("Book B", 180, 6.0, HARD));
    CHECK(tracker.getTotalPages() == 300);
}

TEST_CASE("Total hours sums across books") {
    BookTracker tracker;
    tracker.addBook(makeBook("Book A", 120, 3.5, EASY));
    tracker.addBook(makeBook("Book B", 180, 6.5, HARD));
    CHECK(tracker.getTotalHours() == doctest::Approx(10.0));
}

TEST_CASE("Average speed uses total pages and hours") {
    BookTracker tracker;
    tracker.addBook(makeBook("Book A", 200, 10.0, EASY));
    tracker.addBook(makeBook("Book B", 100, 5.0, MEDIUM));
    CHECK(tracker.getAvgSpeed() == doctest::Approx(20.0));
}

TEST_CASE("Average speed guards divide by zero") {
    BookTracker tracker;
    tracker.addBook(makeBook("Book A", 100, 0.0, EASY));
    CHECK(tracker.getAvgSpeed() == doctest::Approx(0.0));
}

TEST_CASE("Count by difficulty finds hard books") {
    BookTracker tracker;
    tracker.addBook(makeBook("Book A", 100, 2.0, HARD));
    tracker.addBook(makeBook("Book B", 100, 2.0, EASY));
    tracker.addBook(makeBook("Book C", 100, 2.0, HARD));
    CHECK(tracker.countByDifficulty(HARD) == 2);
}

TEST_CASE("Difficulty label for EASY") {
    CHECK(string(difficultyToString(EASY)) == "Easy");
}

TEST_CASE("Difficulty label for MEDIUM") {
    CHECK(string(difficultyToString(MEDIUM)) == "Medium");
}

TEST_CASE("Difficulty label for HARD") {
    CHECK(string(difficultyToString(HARD)) == "Hard");
}
#endif
