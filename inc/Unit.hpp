#ifndef _UNIT_HPP_
#define _UNIT_HPP_

#include "types.hpp"
#include "Rucksack.hpp"

class Unit
{

private:
    const count_t rucksack_count;
    Rucksack* rucksacks;

public:
    Unit(const count_t size, Rucksack* ptr);
    ~Unit();

    count_t get_rucksack_count();
    const Rucksack get_rucksack(const count_t index);
};

#endif /* _UNIT_HPP_ */