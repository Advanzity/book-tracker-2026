#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// Enum for difficulty level
enum Difficulty {
    EASY = 1,
    MEDIUM,
    HARD
};

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
        double totalPages = 0;
        double totalHours = 0;

        for (int i = 0; i < count; i++) {
            totalPages += books[i].pages;
            totalHours += books[i].hours;
        }

        double avgSpeed = 0;
        if (totalHours > 0) {
            avgSpeed = totalPages / totalHours;
        }

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
        int hardCount = 0;
        for (int i = 0; i < count; i++) {
            if (books[i].difficulty == HARD) {
                hardCount++;
            }
        }

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

int main() {
    BookTracker tracker;
    tracker.run();
    return 0;
}