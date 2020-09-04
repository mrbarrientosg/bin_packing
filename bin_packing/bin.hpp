#ifndef bin_hpp
#define bin_hpp

#include <vector>
#include "item.hpp"

class Bin {
    std::vector<long> items;
    long capacity;
    long long free_space;
    long long fill_space;

public:
    Bin(long capacity);
    Bin(const Bin &other);
    ~Bin();

    bool can_add_item(long item) const;
    bool add_item(long item);
    void remove_item(long item);

    long get_item(long idx) const;
    long size() const;
    bool empty() const;
    long get_capacity() const;
    long long get_free_space() const;
    long long get_fill_space() const;
};

#endif /* bin_hpp */
