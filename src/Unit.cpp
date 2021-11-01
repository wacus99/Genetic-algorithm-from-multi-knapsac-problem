#include <iostream>
#include "Unit.hpp"

Unit::Unit(const count_t size, Rucksack* ptr): rucksack_count(size), rucksacks(ptr) {}

Unit::~Unit()
{
    delete rucksacks;
}

count_t Unit::get_rucksack_count()
{
    return rucksack_count;
}

const Rucksack Unit::get_rucksack(const count_t index)
{
    return rucksacks[index];
}