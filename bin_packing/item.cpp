#include "item.hpp"

Item::Item() {
    this->id = -1;
    this->value = -1;
}

Item::Item(long id, long value) {
    this->id = id;
    this->value = value;
}

Item::Item(const Item &other) {
    this->id = other.id;
    this->value = other.value;
}

bool Item::operator<(const Item &other) const {
    return !(value < other.value);
}

bool Item::operator==(const Item &lhs) const {
    return id == lhs.id && value == lhs.value;
}

