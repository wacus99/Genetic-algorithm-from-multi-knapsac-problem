// #include "types.hpp"
// #include "Rucksack.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <map>

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
static std::vector<Item> items = {{40,110},{60,150},
                                  {30,70},{40,80},
                                  {20,30},{5,5}};
//static std::vector<Item> items = {{40,80},{10,20},
//                                  {40,60},{30,40},
//                                  {50,60},{50,60},
//                                  {55,65},{25,25},
//                                  {40,30}};
//static std::vector<Item> items = {{18,78},{9,35},
//                                  {23,89},{20,36},
//                                  {59,94},{61,75},
//                                  {70,74},{79,75},
//                                  {76,80},{30,16}};

int find_by_value(Item value)
{
    for (int i = 0; i < items.size(); ++i)
    {
        if (items[i] == value)
        {
            return i;
        }
    }
    return -1;
}

typedef struct Rucksack
{
    static std::vector<int> capacities;
    std::vector<int> items_index;
    int rucksack_index;
    explicit Rucksack(int index):rucksack_index(index){};
    int capacity_left()
    {
        int sum = 0;
        for (auto &el: items_index)
        {
            sum += items[el].weight;
        }
        return (capacities[rucksack_index] > sum) ? capacities[rucksack_index] - sum : 0;
    }
} Rucksack;

std::vector<int> Rucksack::capacities = {65, 85};

typedef struct Unit
{
    std::vector<Rucksack> rucksack_list;
    Unit() = default;
    explicit Unit(std::vector<Rucksack> list):rucksack_list(std::move(list)){};
    void clear_rucksacks()
    {
        for(auto& ruck: rucksack_list)
        {
            ruck.items_index.clear();
        }
    }
    int count_result()
    {
        int sum = 0;
        for (auto& ruck: rucksack_list)
        {
            for (auto& index: ruck.items_index)
            {
                sum += items[index].profit;
            }
        }
        return sum;
    }
} Unit;

typedef struct Generation
{
    std::vector<Unit> unit_list;
    std::vector<int> result_list;
    void update_result()
    {
        result_list.clear();
        for(auto& uni: unit_list)
            result_list.push_back(uni.count_result());
    }
    int get_max_result()
    {
        return *std::max_element(result_list.begin(), result_list.end());
    }
} Generation;

void draw_unit(Unit* uni)
{
    std::vector<Item> items_copy(items);
    for(int j = 0; j < items.size(); j++)
    {
        unsigned int it = rand() % items_copy.size();
        for (auto& ruk: uni->rucksack_list)
        {
            if(ruk.capacity_left() >= items_copy[it].weight)
            {
                ruk.items_index.push_back(find_by_value(items_copy[it]));
                items_copy.erase(items_copy.begin()+it);
                break;
            }
        }
    }
}

// DEGUB
void print_unit(Unit& uni)
{
    for (auto& ruk: uni.rucksack_list)
    {
        for (auto& el: ruk.items_index) std::cout << el << ", ";
        std::cout << "Capacity left:" << ruk.capacity_left();
        std::cout << "\n";
    }
    std::cout << uni.count_result();
    std::cout << "\n\n";
}
// DEGUB

Generation create_generation(int size)
{
    Generation gen;
    std::vector<Rucksack> ruck_list;
    for (auto i = 0; i < Rucksack::capacities.size(); ++i)
    {
        ruck_list.emplace_back(Rucksack(i));
    }
    Unit uni(ruck_list);
    srand(time(nullptr));
    for (int i = 0; i < size; ++i)
    {
        uni.clear_rucksacks();
        draw_unit(&uni);
        gen.unit_list.push_back(uni);
    }
    return gen;
}

void delete_worst_units(Generation* gen, int count)
{
    std::vector<int> worst_results;
    worst_results = gen->result_list;
    std::sort(worst_results.begin(), worst_results.end());
    worst_results = std::vector<int>(worst_results.begin(), worst_results.begin()+count);
    for (auto& result: worst_results)
    {
        for (auto unit_it = gen->unit_list.begin(); unit_it != gen->unit_list.end(); ++unit_it)
        {
            if (result == unit_it->count_result())
            {
                gen->unit_list.erase(unit_it);
                break;
            }
        }
    }
}

std::vector<int> range(int i)
{
    std::vector<int> range_list;
    for(int it = 0; it < i; ++it)
        range_list.push_back(it);
    return range_list;
}

void check_units (Unit* uni)
{
    std::vector<int> items_index = range(items.size());
    for(auto& ruck: uni->rucksack_list)
    {
        std::vector<int> items_index_copy = ruck.items_index;
        for(auto& it: items_index_copy)
        {
            if (std::find(items_index.begin(), items_index.end(), it) == items_index.end())
            {
                ruck.items_index.erase(std::find(ruck.items_index.begin(), ruck.items_index.end(), it));
            }
            else
            {
                items_index.erase(std::find(items_index.begin(), items_index.end(), it));
            }
        }
    }

    std::sort(items_index.begin(), items_index.end(),
              [](const int a, const int b) -> bool
              {
                  return items[a].profit > items[b].profit;
              });

    for(auto& ruck: uni->rucksack_list)
    {
        std::vector<int> item_index_copy = items_index;
        for(auto& it: item_index_copy)
        {
            if (ruck.capacity_left() >= items[it].weight)
            {
                ruck.items_index.push_back(it);
                items_index.erase(std::find(items_index.begin(), items_index.end(), it));
            }
        }
    }

}

std::pair<Unit,Unit> crossover_operation(Unit& uni1, Unit& uni2)
{
    Unit cross_unit1;
    Unit cross_unit2;
    if(uni1.rucksack_list.size() != uni2.rucksack_list.size())
    {
        std::cout << "ERROR! Different sizes of rucksuck lists!\n";
    }
    else
    {
        for(int it = 0; it < uni1.rucksack_list.size(); ++it)
        {
            if(it % 2 == 0)
            {
                cross_unit1.rucksack_list.push_back(uni2.rucksack_list[it]);
                cross_unit2.rucksack_list.push_back(uni1.rucksack_list[it]);
            }
            else
            {
                cross_unit2.rucksack_list.push_back(uni2.rucksack_list[it]);
                cross_unit1.rucksack_list.push_back(uni1.rucksack_list[it]);
            }
        }
    }
    check_units(&cross_unit1);
    check_units(&cross_unit2);
    return std::make_pair(cross_unit1, cross_unit2);

}

void mutation_operator(Unit* uni)
{
    std::cout << "\nErasing:";
    for (int i = 0; i < uni->rucksack_list.size(); ++i)
    {
        unsigned int rnd_it = rand() % uni->rucksack_list[i].items_index.size();
        uni->rucksack_list[i].items_index.erase(uni->rucksack_list[i].items_index.begin() + rnd_it);
        std::cout <<uni->rucksack_list[i].items_index[rnd_it];
    }
    std::cout << "\n";
    std::vector<int> items_index = range(items.size());
    for(auto& ruck: uni->rucksack_list)
    {
        std::vector<int> items_index_copy = ruck.items_index;
        for(auto& it: items_index_copy)
        {
            if (!bool(std::find(items_index.begin(), items_index.end(), it) == items_index.end()))
            {
                items_index.erase(std::find(items_index.begin(), items_index.end(), it));
            }
        }
    }

    if (items_index.size() > 0)
    {
        std::sort(items_index.begin(), items_index.end(),
                  [](const int a, const int b) -> bool
                  {
                      return items[a].profit > items[b].profit;
                  });
        for(auto& ruck: uni->rucksack_list)
        {
            std::vector<int> item_index_copy = items_index;
            for(auto& it: item_index_copy)
            {
                if (ruck.capacity_left() >= items[it].weight)
                {
                    ruck.items_index.push_back(it);
                    items_index.erase(std::find(items_index.begin(), items_index.end(), it));
                }
            }
        }
    }

}


int main()
{
    int generation_size = 5;
    std::vector<int> pBest;
    Generation gen = create_generation(generation_size);
    for(int i=0; i < 20; ++i)
    {
        gen.update_result();
        pBest.push_back(gen.get_max_result());
        delete_worst_units(&gen, 2);
        for (int i = 0; i < 2*2; i+=2)
        {
            std::pair<Unit, Unit> cross_uni = crossover_operation(gen.unit_list[i],gen.unit_list[i+1]);
            gen.unit_list.push_back(cross_uni.first);
            gen.unit_list.push_back(cross_uni.second);
        }

        for (int i = 0; i < 2; ++i)
        {
            unsigned int it = rand() % generation_size;
            print_unit(gen.unit_list[it]);
            mutation_operator(&gen.unit_list[it]);
            print_unit(gen.unit_list[it]);
            std::cout << "___________NEXT_____ITERATION___\n";
        }

    }
    for (auto& it: pBest)
    {
        std::cout << it << " ,";
    }






        







}