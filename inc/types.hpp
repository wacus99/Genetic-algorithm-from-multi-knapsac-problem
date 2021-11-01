#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <vector>

typedef unsigned int weight_t;

typedef unsigned int profit_t;

typedef unsigned int capacity_t;

typedef unsigned int count_t;

typedef struct Item
{
    const weight_t weight;
    const profit_t profit;

    inline Item& operator=(const Item& other) = default;
    inline Item (const Item& other) = default;
} Item;

#endif /* _TYPES_HPP_ */