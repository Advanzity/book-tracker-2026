#ifdef _DEBUG
#include "support/test_headers.h"

TEST_CASE("Manager operator[] valid index returns correct item")
{
    Manager manager;
    PriceInfo price(10.0, false);
    ReadingItem *first = new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price);
    manager += first;

    CHECK(manager[0] == first);
}

TEST_CASE("Manager operator[] invalid index throws")
{
    Manager manager;
    CHECK_THROWS_AS(manager[-1], ContainerException);
    CHECK_THROWS_AS(manager[0], ContainerException);
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

TEST_CASE("Manager operator-= invalid index throws")
{
    Manager manager;
    PriceInfo price(10.0, false);
    manager += new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price);

    CHECK_THROWS_AS((manager -= -1), ContainerException);
    CHECK_THROWS_AS((manager -= 1), ContainerException);
}

TEST_CASE("Manager initialization")
{
    Manager manager;
    CHECK(manager.getItemCount() == 0);
    CHECK(manager.isEmpty() == true);
}

TEST_CASE("Manager adds items and preserves linked-list order")
{
    Manager manager;
    PriceInfo price(10.0, false);

    manager.addItem(new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price));
    manager.addItem(new PrintBook("Book 2", 200, 10.0, MEDIUM, "Author 2", price));

    CHECK(manager.getItemCount() == 2);
    CHECK(manager[0]->getTitle() == "Book 1");
    CHECK(manager[1]->getTitle() == "Book 2");

    manager.addItem(new PrintBook("Book 3", 300, 15.0, HARD, "Author 3", price));

    CHECK(manager.getItemCount() == 3);
    CHECK(manager[2]->getTitle() == "Book 3");
}

TEST_CASE("Manager removes items")
{
    Manager manager;
    PriceInfo price(10.0, false);

    manager.addItem(new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price));
    manager.addItem(new PrintBook("Book 2", 200, 10.0, MEDIUM, "Author 2", price));
    manager.addItem(new PrintBook("Book 3", 300, 15.0, HARD, "Author 3", price));

    bool removed = manager.removeItem(1);

    CHECK(removed == true);
    CHECK(manager.getItemCount() == 2);
    CHECK(manager[0]->getTitle() == "Book 1");
    CHECK(manager[1]->getTitle() == "Book 3");
    CHECK(manager.removeItem(5) == false);
}

TEST_CASE("Manager countByDifficulty returns zero for empty manager")
{
    Manager manager;
    CHECK(manager.countByDifficulty(EASY) == 0);
}

TEST_CASE("Manager countByDifficulty counts matching difficulty in the linked list")
{
    Manager manager;
    PriceInfo paid(10.0, false);
    PriceInfo freeItem(0.0, true);

    manager.addItem(new PrintBook("Book 1", 100, 4.0, EASY, "Author 1", paid));
    manager.addItem(new AudioBook("Book 2", 120, 6.0, HARD, "Narrator 2", freeItem));
    manager.addItem(new PrintBook("Book 3", 180, 8.0, HARD, "Author 3", paid));
    manager.addItem(new AudioBook("Book 4", 90, 3.0, MEDIUM, "Narrator 4", freeItem));

    CHECK(manager.countByDifficulty(EASY) == 1);
    CHECK(manager.countByDifficulty(MEDIUM) == 1);
    CHECK(manager.countByDifficulty(HARD) == 2);
}

TEST_CASE("Manager sequentialSearchByTitle finds correct index")
{
    Manager manager;
    PriceInfo price(10.0, false);
    manager.addItem(new PrintBook("Dune", 500, 10.0, HARD, "Frank Herbert", price));
    manager.addItem(new PrintBook("Foundation", 300, 6.0, MEDIUM, "Isaac Asimov", price));

    CHECK(manager.sequentialSearchByTitle("Dune") == 0);
    CHECK(manager.sequentialSearchByTitle("Foundation") == 1);
    CHECK(manager.sequentialSearchByTitle("Hobbit") == -1);
}

TEST_CASE("Manager sortByTitle orders items correctly (Bubble Sort)")
{
    Manager manager;
    PriceInfo price(10.0, false);
    manager.addItem(new PrintBook("Z", 100, 2.0, EASY, "Author Z", price));
    manager.addItem(new PrintBook("B", 100, 2.0, EASY, "Author A", price));
    manager.addItem(new PrintBook("G", 100, 2.0, EASY, "Author M", price));

    manager.sortByTitle();

    CHECK(manager[0]->getTitle() == "B");
    CHECK(manager[1]->getTitle() == "G");
    CHECK(manager[2]->getTitle() == "Z");
}

TEST_CASE("Manager binarySearchByTitle finds correct index after sorting the linked list")
{
    Manager manager;
    PriceInfo price(10.0, false);
    manager.addItem(new PrintBook("Mike", 100, 2.0, EASY, "Author Z", price));
    manager.addItem(new PrintBook("Alpha", 100, 2.0, EASY, "Author A", price));
    manager.addItem(new PrintBook("Zulu", 100, 2.0, EASY, "Author M", price));
    manager.addItem(new PrintBook("Bravo", 100, 2.0, EASY, "Author B", price));

    manager.sortByTitle();

    CHECK(manager.binarySearchByTitle("Alpha") == 0);
    CHECK(manager.binarySearchByTitle("Bravo") == 1);
    CHECK(manager.binarySearchByTitle("Mike") == 2);
    CHECK(manager.binarySearchByTitle("Zulu") == 3);
    CHECK(manager.binarySearchByTitle("Nowhere") == -1);
}

TEST_CASE("Search algorithms return -1 on empty Manager")
{
    Manager manager;
    CHECK(manager.sequentialSearchByTitle("Apple") == -1);
    CHECK(manager.binarySearchByTitle("Apple") == -1);
}
#endif
