#ifndef action_hpp
#define action_hpp

#include "item.hpp"

struct Action {
    Item *item1;
    Item *item2;
    long bin_idx_dest;
    bool is_swap;

    Action();
    Action(Item *item, long bin_idx_dest);
    Action(Item *item1, Item *item2);
    Action(const Action &a);

    bool operator==(const Action &lhs) const;
};

#endif /* action_hpp */
