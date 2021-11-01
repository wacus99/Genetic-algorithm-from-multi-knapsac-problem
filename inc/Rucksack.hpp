#ifndef _RUCKSACK_HPP_
#define _RUCKSACK_HPP_

#include "types.hpp"
#include <iostream>

class Rucksack
{
private:
    const capacity_t capacity;
    std::vector<Item> items;

public:
    inline Rucksack(const capacity_t cap): capacity(cap) {}
    inline Rucksack(const capacity_t cap, std::vector<Item> items_vector): capacity(cap)
    {
        capacity_t weight_sum = 0;
        for (auto item = items_vector.cbegin(); item != items_vector.cend(); ++item)
        {
            weight_sum += item->weight;
        }
        if (weight_sum <= cap)
        {
            for (auto item = items_vector.begin(); item != items_vector.end(); ++item)
            {
                items.push_back(*item);
            }
        }
        else
        {
            std::cout << "Rucksack capacity is too small. Please limit range of items!";
        }
    }

    inline capacity_t get_capacity(){return capacity;}
    inline Item get_item(const count_t index){return items[index];}
};

#endif /* _RUCKSACK_HPP_ */