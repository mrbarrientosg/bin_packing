#include "action.hpp"

Action::Action() {
    this->item1 = nullptr;
    this->item2 = nullptr;
    this->bin_idx_dest = -1;
    this->is_swap = false;
}

Action::Action(Item *item, long bin_idx_dest):  bin_idx_dest(bin_idx_dest), is_swap(false) {
    this->item1 = item;
    this->item2 = nullptr;
}

Action::Action(Item *item1, Item *item2): is_swap(true) {
    this->item1 = item1;
    this->item2 = item2;
}

Action::Action(const Action &a) {
    item1 = a.item1;
    item2 = a.item2;
    bin_idx_dest = a.bin_idx_dest;
    is_swap = a.is_swap;
}


bool Action::operator==(const Action &lhs) const {
    if (is_swap && lhs.is_swap) {
        return *item1 == *lhs.item1 && *item2 == *lhs.item2;
    } else if (!is_swap && !lhs.is_swap) {
        return *item1 == *lhs.item1 && bin_idx_dest == lhs.bin_idx_dest;
    } else {
        return false;
    }
}
