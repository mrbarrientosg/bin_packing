#ifndef item_hpp
#define item_hpp

struct Item {
    long id;
    long value;

public:
    Item();
    Item(long id, long value);
    Item(const Item &other);

    bool operator<(const Item &other) const;
    bool operator==(const Item &lhs) const;
};

#endif /* item_hpp */
