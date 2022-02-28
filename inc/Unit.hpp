#ifndef UNIT_HPP
#define UNIT_HPP

#include "types.hpp"

class Unit
{
public:
    std::vector<Rucksack> rucksack_list;
    Unit() = default;
    explicit Unit(std::vector<Rucksack> list):rucksack_list(std::move(list)){};
    void clear_rucksacks();
    int count_result(std::vector<Item> items);
    static void print_unit(Unit& uni, std::vector<Item> items);
    static void shuffle_units(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2);
    static void ox_operator(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2, int p); // losowanie miedzy 0 a size-2
    static void pmx_operator(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2, int p1, int p2); //losowanie miedzy 0 a size-2
    static std::pair<Unit,Unit> crossover_operation(Unit& uni1, Unit& uni2, int opt = 0);
    static std::vector<int> range(int i);
    static void mutation_operator_1(Unit* uni, std::vector<Item> items);
    static void mutation_operator_2(Unit* uni, std::vector<Item> items);
    static void mutation_operator_3(Unit* uni, std::vector<Item> items);
};

#endif //UNIT_HPP
