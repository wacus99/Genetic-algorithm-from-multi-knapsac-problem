#ifndef TYPES_HPP
#define TYPES_HPP
#include <iostream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <ctime>

typedef struct Item
{
    int weight;
    int profit;

    bool operator==(const Item& val) const
    {
        if ((val.weight == this->weight) &&
            (val.profit == this->profit))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
} Item;

typedef struct Rucksack
{
    std::vector<int> items_index;
    int capacity;
    explicit Rucksack(int _capacity):capacity(_capacity){};
    int capacity_left(std::vector<Item> items)
    {
        int sum = 0;
        for (auto &index: items_index)
        {
            sum += items[index].weight;
        }
        return capacity - sum;
    }
} Rucksack;

typedef struct Results
{
    int iteration_count;
    int result;
    double time;
    Results() = default;
    Results(int it, int r, double t): iteration_count(it), result(r), time(t){};
}Results;

typedef struct Stats
{
    double avg_time;
    double avg_res;
    double acc_res;
    double avg_it;
}Stats;



#endif //TYPES_HPP
