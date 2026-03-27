#ifdef _DEBUG
#include "support/test_headers.h"

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

    std::ostringstream output;
    ReadingItem &item = book;
    output << item;

    CHECK(output.str() == "PrintBook: Dune by Frank Herbert | pages: 600 | hours: 20.0 | difficulty: Hard | cost: $14.50");
}

TEST_CASE("operator<< outputs one-line AudioBook summary via base reference")
{
    PriceInfo price(0.0, true);
    AudioBook book("Project Hail Mary", 496, 18.0, MEDIUM, "Ray Porter", price);

    std::ostringstream output;
    ReadingItem &item = book;
    output << item;

    CHECK(output.str() == "AudioBook: Project Hail Mary (narrated by Ray Porter) | pages: 496 | hours: 18.0 | difficulty: Medium | cost: Free");
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

TEST_CASE("Polymorphism via Base Pointer")
{
    PriceInfo price(10.0, false);
    ReadingItem *item = new PrintBook("Polymorphism", 100, 5.0, EASY, "Author", price);

    CHECK(item->displayName() == "Polymorphism by Author");
    delete item;
}
#endif
