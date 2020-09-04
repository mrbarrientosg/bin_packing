#ifndef bin_packing_hpp
#define bin_packing_hpp

#include "state.hpp"
#include <list>
class BinPacking {
    std::vector<long> items;
    long capacity;
    State *best;

private:
    struct ItemSet { // Para MBS
        std::vector<long> items;
        long free_space;

        ItemSet(long free_space): free_space(free_space), items() {}
        ItemSet(const ItemSet &o): free_space(o.free_space), items(o.items) {}

        void add_item(long item);
        void remove_item(long item);
    };

    void mbs(long idx, long n, long min_size, std::vector<long> &items, ItemSet &current, ItemSet &best) const;
    State *initial_state() const;
    void shaking(State *current, long k) const;
    void local_search(State *current) const;
    void best_action(State *current, long &best, std::vector<Item*> &z, std::vector<long> &empty_bin) const;
    bool stop_condition(long k, long k_max, bool decrease) const;
    void move_or_not(State **current, State **best, State *last, long &k, long k_max, bool decrease) const;
public:
    BinPacking(std::vector<long> items, long capacity);

    void solve(long k_max, bool decrease);
    long long best_fitness() const;
    long best_size() const;
    void print_best() const;
};

#endif /* bin_packing_hpp */
