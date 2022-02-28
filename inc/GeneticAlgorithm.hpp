#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include "Generation.hpp"

class GeneticAlgorithm
{
private:
    Generation gen;
    Generation create_generation(int size);
    void draw_unit(Unit* uni);

public:
    std::vector<Item> items_list;
    std::vector<int> rucksacks_capacities;
    Results run(int generation_size, int count_to_del, int count_to_mute, int sel_id, int cross_id, int mut_id);
};

#endif //GENETIC_ALGORITHM_HPP