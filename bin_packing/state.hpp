#ifndef state_hpp
#define state_hpp

#include "bin.hpp"
#include "action.hpp"
#include <algorithm>
#include <set>
#include <numeric>
#include <chrono>
#include <random>

extern std::mt19937 rng;

class State {
    std::vector<Bin *> bins;
    long capacity;
    long long fitness;

private:
    const std::vector<Item*> get_z_list() const;
    long get_random_idx(long max) const;
    void transfer(Action &action);
    void swap_items(Action &action);

public:
    State(long capacity);
    State(const State &s);
    ~State();

    void add_item(long item);
    void update_fitness();
    Bin *get_bin(long idx) const;
    long long get_fitness() const;
    long size() const;
    void transition(Action &action);

    const std::vector<Item*> get_not_full_z_list(std::vector<long> &bin_idx) const;
    void shaking(long k_max);
};

#endif /* state_hpp */
