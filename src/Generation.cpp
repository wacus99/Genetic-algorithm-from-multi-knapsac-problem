#include "Generation.hpp"

Generation::Generation(count_t size, Unit* ptr): units_count(size), units(ptr) {}

Generation::~Generation()
{
    delete units;
}

count_t Generation::get_units_count()
{
    return units_count;
}

const Unit Generation::get_unit(count_t index)
{
    return units[index];
}