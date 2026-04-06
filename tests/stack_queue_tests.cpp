#ifdef _DEBUG
#include "support/test_headers.h"

#include "structures/queue.h"
#include "structures/stack.h"

TEST_CASE("Stack starts empty")
{
    Stack s;
    CHECK(s.isEmpty() == true);
    CHECK(s.isFull() == false);
}

TEST_CASE("Stack push peek pop LIFO order")
{
    Stack s;
    s.push("a");
    s.push("b");
    CHECK(s.isEmpty() == false);
    CHECK(s.peek() == "b");
    s.pop();
    CHECK(s.peek() == "a");
    s.pop();
    CHECK(s.isEmpty() == true);
}

TEST_CASE("Stack pop on empty throws")
{
    Stack s;
    CHECK_THROWS_AS(s.pop(), ContainerException);
}

TEST_CASE("Stack peek on empty throws")
{
    Stack s;
    CHECK_THROWS_AS(s.peek(), ContainerException);
}

TEST_CASE("Stack push when full throws")
{
    Stack s;
    for (int i = 0; i < 64; i++)
    {
        s.push("x");
    }
    CHECK(s.isFull() == true);
    CHECK_THROWS_AS(s.push("overflow"), ContainerException);
}

TEST_CASE("Queue starts empty")
{
    Queue q;
    CHECK(q.isEmpty() == true);
}

TEST_CASE("Queue enqueue front dequeue FIFO order")
{
    Queue q;
    q.enqueue("first");
    q.enqueue("second");
    CHECK(q.isEmpty() == false);
    CHECK(q.front() == "first");
    q.dequeue();
    CHECK(q.front() == "second");
    q.dequeue();
    CHECK(q.isEmpty() == true);
}

TEST_CASE("Queue dequeue on empty throws")
{
    Queue q;
    CHECK_THROWS_AS(q.dequeue(), ContainerException);
}

TEST_CASE("Queue front on empty throws")
{
    Queue q;
    CHECK_THROWS_AS(q.front(), ContainerException);
}

TEST_CASE("Manager peekRecentAddition reflects last addItem title")
{
    Manager manager;
    PriceInfo price(10.0, false);
    manager.addItem(new PrintBook("Alpha", 100, 5.0, EASY, "Author 1", price));
    manager.addItem(new PrintBook("Beta", 200, 10.0, MEDIUM, "Author 2", price));

    CHECK(manager.peekRecentAddition() == "Beta");
}

TEST_CASE("Manager hasPendingRemovals after removeItem")
{
    Manager manager;
    PriceInfo price(10.0, false);
    manager.addItem(new PrintBook("Gone", 50, 2.0, EASY, "Author", price));
    CHECK(manager.hasPendingRemovals() == false);

    manager.removeItem(0);

    CHECK(manager.hasPendingRemovals() == true);
}

TEST_CASE("Manager showReport drains removal queue")
{
    Manager manager;
    PriceInfo price(10.0, false);
    manager.addItem(new PrintBook("One", 50, 2.0, EASY, "A", price));
    manager.removeItem(0);
    CHECK(manager.hasPendingRemovals() == true);

    manager.showReport();

    CHECK(manager.hasPendingRemovals() == false);
}

#endif
