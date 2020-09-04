#include "bin_packing.hpp"
#include <iostream>

BinPacking::BinPacking(std::vector<long> items, long capacity): items(items), capacity(capacity) {
    std::sort(this->items.begin(), this->items.end(), std::greater<long>());
    this->best = nullptr;
}

void BinPacking::ItemSet::add_item(long item) {
    this->items.push_back(item);
    this->free_space -= item;
}

void BinPacking::ItemSet::remove_item(long item) {
    items.erase(items.begin() + std::distance(items.begin(), std::find(items.begin(), items.end(), item)));
    this->free_space += item;
}

long long BinPacking::best_fitness() const {
    if (best == nullptr) {
        return 0;
    }

    return best->get_fitness();
}

long BinPacking::best_size() const {
    if (best == nullptr) {
        return 0;
    }

    return best->size();
}

void BinPacking::mbs(long idx, long n, long min_size, std::vector<long> &items, ItemSet &current, ItemSet &best) const {
    if (current.free_space >= min_size) {
        for (long r = idx; r < n; r++) {
            long item = items[r];

            if (item <= current.free_space) {
                current.add_item(item);
                mbs(r + 1, n, min_size, items, current, best);
                current.remove_item(item);
                if (best.free_space == 0)
                    return;
            }
        }
    }

    if (current.free_space < best.free_space) {
        best = current;
    }
}

State *BinPacking::initial_state() const {
    State *initial = new State(this->capacity);
    std::vector<long> items(this->items);
    ItemSet current(this->capacity);
    ItemSet best(this->capacity);
    long n = items.size();

    while (n > 0) {
        current.add_item(items.front());
        best.add_item(items.front());

        mbs(1, n, this->items.back(), items, current, best);

        for (const long &item: best.items) {
            initial->add_item(item);
            items.erase(items.begin() + std::distance(items.begin(), std::find(items.begin(), items.end(), item)));
        }

        n -= best.items.size();
        current = ItemSet(this->capacity);
        best = ItemSet(this->capacity);
    }

    initial->update_fitness();

    return initial;
}

void BinPacking::best_action(State *from, long &best, std::vector<Item*> &z, std::vector<long> &empty_bin) const {
    Action transfer;
    Action swap;
    long best_tranf = 0;
    long best_swap = 0;
    best = 0;

    for (const long &idx: empty_bin) {
        Bin *current = from->get_bin(idx);

        for (long i = z.size() - 1; i >= 0; i--) {
            if (z[i]->value > current->get_free_space()) {
                break;
            }

            if (z[i]->id != idx && z[i]->value <= current->get_free_space()) {
                long l_a = current->get_fill_space(); // - z[i].value
                long l_b = from->get_bin(z[i]->id)->get_fill_space();

                long f = pow((l_a + z[i]->value), 2) + pow((l_b - z[i]->value), 2) - pow(l_a, 2) - pow(l_b, 2);

                if (f > 0 && f > best_tranf) {
                    best_tranf = f;
                    transfer = Action(z[i], idx);
                }
            }
        }

    }

    for (long q = z.size() - 1; q >= 0; q--) {
        Item *current_q = z[q];
        Bin *current_bq = from->get_bin(current_q->id);
        long r = q;

        while (z[r]->value == z[q]->value && r > 0) {
            r--;
        }

        for (long i = r; i >= 0; i--) {
            Item *current_i = z[i];
            Bin *current_bi = from->get_bin(current_i->id);

            if (current_i->value > (current_q->value + current_bq->get_free_space()) || current_q->value > (current_i->value + current_bi->get_free_space())) {
               break;
            }

            if (current_q->id != current_i->id) {
                long l_a = current_bq->get_fill_space(); // - z[i].value
                long l_b = current_bi->get_fill_space();

                long curr_sum_i_update = l_b - current_i->value + current_q->value;
                long curr_sum_q_update = l_a - current_q->value + current_i->value;

                long f = pow(curr_sum_q_update, 2) + pow(curr_sum_i_update, 2) - pow(l_b, 2) - pow(l_a, 2);

                if (f > best_swap) {
                    best_swap = f;
                    swap = Action(current_q, current_i);
                }
            }
        }
    }

    if (best_swap > best_tranf) {
        best = best_swap;
        from->transition(swap);
    } else if (transfer.bin_idx_dest != -1) {
        best = best_tranf;
        from->transition(transfer);
    }

    for (Item *item: z) {
        delete item;
    }

    z.clear();
}

void BinPacking::local_search(State *current) const {
    std::vector<long> empty_bin_idx;
    std::vector<Item*> z;
    long best_action = 0;

    do {
        z = current->get_not_full_z_list(empty_bin_idx);
        this->best_action(current, best_action, z, empty_bin_idx);
        empty_bin_idx.clear();
    } while (best_action > 0);

    current->update_fitness();
}

bool BinPacking::stop_condition(long k, long k_max, bool reverse) const {
    return (!reverse) ? k < k_max : k > 0;
}

void BinPacking::print_best() const {
    if (best == nullptr) {
        std::cout << "No hay mejor\n";
        return;
    }

    std::cout << "Total Bins: " << best->size() << "\n";
    std::cout << "Fitness: " << best->get_fitness() << "\n";

    std::cout << "Solution:\n";
    for (long i = 0; i < best->size(); i++) {
        for (long j = 0; j < best->get_bin(i)->size(); j++) {
            std::cout << best->get_bin(i)->get_item(j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void BinPacking::move_or_not(State **current, State **best, State *last, long &k, long k_max, bool reverse) const {
    if ((*current)->size() < (*best)->size() || (*current)->get_fitness() > (*best)->get_fitness()) {
        k = (!reverse) ? 0 : k_max;
        delete *best;
        *best = new State(**current);
    } else {
        delete *current;
        *current = new State(*last);
        k += (!reverse) ? 1 : -1;
    }
}

void BinPacking::solve(long k_max, bool reverse) {
    best = nullptr;
    State *initial = initial_state();
    State *best = new State(*initial);
    State *current = new State(*initial);
    State *last;
    long k = (!reverse) ? 0 : k_max;

    do {
        last = new State(*current);

        // shaking
        current->shaking(k);

        // local search
        local_search(current);

        // move
        move_or_not(&current, &best, last, k, k_max, reverse);

        delete last;
    } while (stop_condition(k, k_max, reverse));

    this->best = best;
}

