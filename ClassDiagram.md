# Class Diagram

```mermaid
classDiagram
    class ReadingItem {
        <<abstract>>
        #string title
        #int pages
        #Difficulty difficulty
        #double hours
        +displayName() string
        +toStream(ostream&) void
        +print(ostream&) void
    }

    class PriceInfo {
        -double cost
        -bool includedWithSubscription
        +isFree() bool
        +formattedCost() string
    }

    class PrintBook {
        -string author
        -PriceInfo price
        +displayName() string
        +print(ostream&) void
    }

    class AudioBook {
        -string narrator
        -PriceInfo price
        +displayName() string
        +print(ostream&) void
    }

    class ReadingItemNode {
        +ReadingItem* data
        +ReadingItemNode* next
    }

    class ReadingItemListIterator {
        -ReadingItemNode* current
        +isValid() bool
        +next() void
        +getData() ReadingItem*
    }

    class ReadingItemList {
        -ReadingItemNode* head
        -ReadingItemNode* tail
        -int count
        +insertFront(ReadingItem*) void
        +insertBack(ReadingItem*) void
        +removeAt(int) bool
        +deleteByTitle(string) bool
        +searchByTitle(string) ReadingItem*
        +traverse(ostream&) void
        +begin() ReadingItemListIterator
    }

    class Manager {
        -ReadingItemList items
        +addItem(ReadingItem*) void
        +removeItem(int) bool
        +sequentialSearchByTitle(string) int
        +sortByTitle() void
        +binarySearchByTitle(string) int
        +showReport() void
    }

    ReadingItem <|-- PrintBook
    ReadingItem <|-- AudioBook
    PrintBook *-- PriceInfo
    AudioBook *-- PriceInfo
    Manager *-- ReadingItemList
    ReadingItemList *-- ReadingItemNode
    ReadingItemNode --> ReadingItem
    ReadingItemListIterator --> ReadingItemNode
    ReadingItemList ..> ReadingItemListIterator
```
