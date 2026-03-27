#ifdef _DEBUG
#include "support/test_headers.h"

TEST_CASE("ReadingItemList insertFront handles inserting into an empty list")
{
    ReadingItemList list;
    PriceInfo price(12.0, false);

    list.insertFront(new PrintBook("Solo", 150, 6.0, EASY, "Author Solo", price));

    CHECK(list.size() == 1);
    CHECK(list.at(0)->getTitle() == "Solo");
}

TEST_CASE("ReadingItemList supports unordered inserts at the front and back")
{
    ReadingItemList list;
    PriceInfo price(10.0, false);

    list.insertBack(new PrintBook("Middle", 200, 8.0, MEDIUM, "Author Middle", price));
    list.insertFront(new PrintBook("First", 120, 4.0, EASY, "Author First", price));
    list.insertBack(new AudioBook("Last", 180, 7.0, HARD, "Narrator Last", price));

    CHECK(list.size() == 3);
    CHECK(list.at(0)->getTitle() == "First");
    CHECK(list.at(1)->getTitle() == "Middle");
    CHECK(list.at(2)->getTitle() == "Last");
}

TEST_CASE("ReadingItemList searchByTitle locates a node and returns nullptr when missing")
{
    ReadingItemList list;
    PriceInfo price(10.0, false);

    list.insertBack(new PrintBook("Dune", 500, 10.0, HARD, "Frank Herbert", price));
    list.insertBack(new PrintBook("Foundation", 300, 6.0, MEDIUM, "Isaac Asimov", price));

    CHECK(list.searchByTitle("Dune") != nullptr);
    CHECK(list.searchByTitle("Dune")->getTitle() == "Dune");
    CHECK(list.searchByTitle("Missing") == nullptr);
}

TEST_CASE("ReadingItemList deleteByTitle removes a node and ignores missing titles")
{
    ReadingItemList list;
    PriceInfo price(10.0, false);

    list.insertBack(new PrintBook("Book 1", 100, 5.0, EASY, "Author 1", price));
    list.insertBack(new PrintBook("Book 2", 200, 10.0, MEDIUM, "Author 2", price));

    CHECK(list.deleteByTitle("Book 2") == true);
    CHECK(list.size() == 1);
    CHECK(list.at(0)->getTitle() == "Book 1");
    CHECK(list.deleteByTitle("Does Not Exist") == false);
}

TEST_CASE("ReadingItemList traverse reports an empty list")
{
    ReadingItemList list;
    std::ostringstream output;

    list.traverse(output);

    CHECK(output.str() == "List is empty.\n");
}

TEST_CASE("ReadingItemList iterator starts at the front and advances through the list")
{
    ReadingItemList list;
    PriceInfo price(10.0, false);

    list.insertBack(new PrintBook("First", 100, 5.0, EASY, "Author 1", price));
    list.insertBack(new PrintBook("Second", 200, 10.0, MEDIUM, "Author 2", price));

    ReadingItemListIterator iterator = list.begin();

    CHECK(iterator.isValid() == true);
    CHECK(iterator.getData()->getTitle() == "First");

    iterator.next();
    CHECK(iterator.isValid() == true);
    CHECK(iterator.getData()->getTitle() == "Second");

    iterator.next();
    CHECK(iterator.isValid() == false);
    CHECK_THROWS_AS(iterator.getData(), ContainerException);
}
#endif
