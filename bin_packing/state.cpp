#include "state.hpp"
#include <random>

State::State(long capacity) {
    this->capacity = capacity;
    this->fitness = 0;
}

State::State(const State &s) {
    this->bins.reserve(s.size());
    for (const Bin *bin: s.bins) {
        this->bins.push_back(new Bin(*bin));
    }
    this->capacity = s.capacity;
    this->fitness = s.fitness;
}

State::~State() {
    for (Bin *bin: bins) {
        delete bin;
    }
    bins.clear();
}

void State::add_item(long item) {
    std::vector<Bin *>::iterator it = std::find_if(this->bins.begin(), this->bins.end(), [&item](const Bin *b) {
        return b->can_add_item(item);
    });

    if (it != this->bins.end()) {
        (*it)->add_item(item);
    } else {
        Bin *b = new Bin(this->capacity);
        b->add_item(item);
        this->bins.push_back(b);
    }
}

void State::update_fitness() {
    this->fitness = std::accumulate(this->bins.begin(), this->bins.end(), 0LL, [](long a, const Bin *b) {
        return a + pow(b->get_fill_space(), 2);
    });
}

Bin *State::get_bin(long idx) const {
    return this->bins[idx];
}

long long State::get_fitness() const {
    return this->fitness;
}

const std::vector<Item*> State::get_not_full_z_list(std::vector<long> &bin_idx) const {
    std::vector<Item*> z;

    for (long i = 0; i < bins.size(); i++) {
        if (bins[i]->get_free_space() != 0) {
            bin_idx.push_back(i);
            for (long j = 0; j < bins[i]->size(); j++) {
                z.push_back(new Item (i, bins[i]->get_item(j)));
            }
        }
    }

    std::sort(z.begin(), z.end(), [](const Item *a, const Item *b) {
        return a->value > b->value;
    });

    return z;
}

void State::transition(Action &action) {
    if (action.is_swap) {
         this->swap_items(action);
     } else {
         this->transfer(action);
     }
}

void State::transfer(Action &action) {
    this->bins[action.item1->id]->remove_item(action.item1->value);
    this->bins[action.bin_idx_dest]->add_item(action.item1->value);

    if (this->bins[action.item1->id]->empty()) {
        delete this->bins[action.item1->id];
        this->bins.erase(bins.begin() + action.item1->id);
    }

    action.item1->id = action.bin_idx_dest;
}

void State::swap_items(Action &action) {
    long idx1 = action.item1->id;
    long idx2 = action.item2->id;

    this->bins[idx1]->remove_item(action.item1->value);
    this->bins[idx2]->remove_item(action.item2->value);

    this->bins[idx1]->add_item(action.item2->value);
    this->bins[idx2]->add_item(action.item1->value);

    action.item1->id = idx2;
    action.item2->id = idx1;
}

long State::size() const {
    return this->bins.size();
}

long State::get_random_idx(long max) const {
    return std::uniform_int_distribution<long>(0, max - 1)(rng);
}

const std::vector<Item*> State::get_z_list() const {
    std::vector<Item*> z;

    for (long i = 0; i < bins.size(); i++) {
        for (long j = 0; j < bins[i]->size(); j++) {
            z.push_back(new Item(i, bins[i]->get_item(j)));
        }
    }

    std::sort(z.begin(), z.end(), [](const Item *a, const Item *b) {
        return a->value > b->value;
    });

    return z;
}

void State::shaking(long k_max) {
    std::vector<Action> actions;
    std::set<long> selected;

    std::vector<Item*> z = get_z_list();
    std::vector<Item*> selected_item;
    long k = 0;

    long i = get_random_idx(z.size());
    Item *current = z[i];

    while (k < k_max) {
        // Transfer
        for (long j = 0; j < current->id; j++) {
            if ((bins[j]->get_free_space() - current->value) >= 0) {
                actions.push_back(Action(current, j));
            }
        }

        for (long j = current->id; j < bins.size(); j++) {
            if ((bins[j]->get_free_space() - current->value) >= 0) {
                actions.push_back(Action(current, j));
            }
        }

        // Swap
        for (long j = z.size() - 1; j > i; j--) {
            long pre_condition1 = bins[current->id]->get_free_space() + current->value - z[j]->value;
            long pre_condition2 = bins[z[j]->id]->get_free_space() + z[j]->value - current->value;

            if (current->id != z[j]->id &&
                current->value != z[j]->value &&
                !selected.count(j) &&
                pre_condition1 >= 0 &&
                pre_condition2 >= 0) {
                actions.push_back(Action(current, z[j]));
            }
        }

        // Change
        if (!actions.empty()) {
            long random_move = get_random_idx(actions.size());

            transition(actions[random_move]);

            selected.insert(i);
            selected_item.push_back(z[i]);
            z.erase(z.begin() + i);
        }

        if (z.empty()) {
            break;
        }

        i = get_random_idx(z.size());
        current = z[i];
        actions.clear();
        k++;
    }

    update_fitness();

    for (Item *item: z) {
        delete item;
    }

    for (Item *item: selected_item) {
        delete item;
    }


    z.clear();
}

