// #include "types.hpp"
// #include "Rucksack.hpp"

#include "inc/GeneticAlgorithm.hpp"
#include "inc/data.hpp"
#include <iomanip>
#include <chrono>

extern std::vector<int> optimum_test;
extern std::vector<std::vector<int>> capacities_test;
extern std::vector<std::vector<Item>> items_test;

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        std::cout << "To less arguments!";
    }
    else
    {
        int test_case = std::atoi(argv[1]);
        int sel_id = std::atoi(argv[2]);
        int cross_id = std::atoi(argv[3]);
        int mut_id = std::atoi(argv[4]);
        GeneticAlgorithm g;
        srand(time(NULL));
        g.items_list = items_test[test_case];
        g.rucksacks_capacities = capacities_test[test_case];
        Results res;
        auto start = std::chrono::high_resolution_clock::now();
        res = g.run(10, 6, 5, sel_id, cross_id, mut_id);
        auto stop = std::chrono::high_resolution_clock::now();
        auto diff = stop - start;
        std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(diff);
        std::cout << res.result << "," << ns.count() << "," << res.iteration_count << '\n';
    }
}