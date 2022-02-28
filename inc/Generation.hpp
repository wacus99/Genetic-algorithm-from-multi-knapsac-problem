#ifndef GENERATION_HPP
#define GENERATION_HPP

#include "types.hpp"
#include "Unit.hpp"

class Generation
{
private:
    void delete_worst_units(int count, std::vector<Item> items);
    void rulette_selection_1(int count, std::vector<Item> items);
    void rulette_selection_2(int count, std::vector<Item> items);
    std::vector<int> range(int i);
    void check_unit(Unit* uni, std::vector<Item> items);

public:
    std::vector<Unit> unit_list;
    std::vector<int> result_list;
    int get_max_result(std::vector<Item> items);
    Unit get_max_unit(std::vector<Item> items);
    void update_result(std::vector<Item> items);
    void selection(int count, int method_id, std::vector<Item> items);
    void crossover(std::vector<Item> items, int count, int method_id);
    void mutation(int count_to_mute, std::vector<Item> items, int opt);
};

#endif //GENERATION_HPP
