#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <ctime>
#include "bin_packing.hpp"

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
//std::chrono::high_resolution_clock::now().time_since_epoch().count()
int main(int argc, const char * argv[]) {

    if (argc == 1) {
        std::cout << "Debe ingresar una instancia\n";
        return 0;
    }

    std::ifstream source;
    std::string line;

    source.open(argv[1]);
    std::getline(source, line);
    int n = std::stoi(line);

    std::getline(source, line);
    int capacity = std::stoi(line);

    std::vector<long> items;
    items.reserve(n);

    while (std::getline(source, line)) {
        items.push_back(std::stoi(line));
    }

    source.close();

    BinPacking problem(items, capacity);

    std::cout << "Incremental:\n";
    problem.solve(20, false);
    problem.print_best();

    std::cout << "Decrease:\n";
    problem.solve(20, true);
    problem.print_best();
    

//    double time = 0.0;
//    std::ofstream log;
//    log.open("./output.csv");
//
//    // incremental
//    do {
//        std::clock_t s = std::clock();
//        problem.solve(20, false);
//        log << problem.best_fitness() << "," << problem.best_size() << "\n";
//        double c_time = (double) (std::clock() - s) / CLOCKS_PER_SEC;
//
//        time += c_time;
//    } while (time < 10);
//    log.close();
//
//
//    time = 0.0;
//    // reverse
//    log.open("./output-reverse.csv");
//    do {
//        std::clock_t s = std::clock();
//        problem.solve(20, true);
//        log << problem.best_fitness() << "," << problem.best_size() << "\n";
//        double c_time = (double) (std::clock() - s) / CLOCKS_PER_SEC;
//
//        time += c_time;
//    } while (time < 10);
//    log.close();


    return 0;
}
