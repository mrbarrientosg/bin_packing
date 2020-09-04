#include "bin.hpp"
#include <iostream>

Bin::Bin(long capacity) {
    this->capacity = capacity;
    this->fill_space = 0;
    this->free_space = capacity;
}

Bin::Bin(const Bin &other) {
    this->items = std::vector<long>(other.items);
    this->capacity = other.capacity;
    this->fill_space = other.fill_space;
    this->free_space = other.free_space;
}

Bin::~Bin() {
    this->capacity = -1;
    this->fill_space = -1;
    this->free_space = -1;
    items.clear();
}

bool Bin::can_add_item(long item) const {
    return item <= free_space;
}

bool Bin::add_item(long item) {
    if (can_add_item(item)) {
        this->fill_space += item;
        this->free_space -= item;
        this->items.push_back(item);
        return true;
    }

    return false;
}

void Bin::remove_item(long item) {
    std::vector<long>::iterator it = std::find_if(items.begin(), items.end(), [&](const long &a) {
        return a == item;
    });

    if (it != items.end()) {
        items.erase(it);
        this->fill_space -= item;
        this->free_space += item;
    } 
}

long Bin::get_item(long idx) const {
    return this->items[idx];
}

long Bin::size() const {
    return this->items.size();
}

bool Bin::empty() const {
    return this->items.empty();
}

long long Bin::get_free_space() const {
    return this->free_space;
}

long long Bin::get_fill_space() const {
    return this->fill_space;
}

long Bin::get_capacity() const {
    return this->capacity;
}

