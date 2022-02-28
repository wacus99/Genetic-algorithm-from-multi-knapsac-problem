#include "Unit.hpp"

void Unit::clear_rucksacks()
{
    for(auto& ruck: rucksack_list)
    {
        ruck.items_index.clear();
    }
}

int Unit::count_result(std::vector<Item> items)
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

void Unit::print_unit(Unit& uni, std::vector<Item> items)
{
    for (auto& ruk: uni.rucksack_list)
    {
        for (auto& el: ruk.items_index)
            std::cout << el << ",";
        std::cout << "Capacity left:" << ruk.capacity_left(items);
        std::cout << "\n";
    }
    std::cout << uni.count_result(items);
    std::cout << "\n\n";
}

void Unit::shuffle_units(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2)
{
    for(int it = 0; it < uni1.rucksack_list.size(); ++it)
    {
        if(it % 2 == 0)
        {
            new_unit1->rucksack_list.push_back(uni2.rucksack_list[it]);
            new_unit2->rucksack_list.push_back(uni1.rucksack_list[it]);
        }
        else
        {
            new_unit2->rucksack_list.push_back(uni2.rucksack_list[it]);
            new_unit1->rucksack_list.push_back(uni1.rucksack_list[it]);
        }
    }
}

void Unit::ox_operator(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2, int p) // losowanie miedzy 0 a size-2
{
    for(int it = 0; it < uni1.rucksack_list.size(); ++it)
    {
        if(it > p)
        {
            new_unit1->rucksack_list.push_back(uni2.rucksack_list[it]);
            new_unit2->rucksack_list.push_back(uni1.rucksack_list[it]);
        }
        else
        {
            new_unit2->rucksack_list.push_back(uni2.rucksack_list[it]);
            new_unit1->rucksack_list.push_back(uni1.rucksack_list[it]);
        }
    }
}

void Unit::pmx_operator(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2, int p1, int p2) //losowanie miedzy 0 a size-2
{
    for(int it = 0; it < uni1.rucksack_list.size(); ++it)
    {
        if(it > p1 && it <= p2)
        {
            new_unit1->rucksack_list.push_back(uni2.rucksack_list[it]);
            new_unit2->rucksack_list.push_back(uni1.rucksack_list[it]);
        }
        else
        {
            new_unit2->rucksack_list.push_back(uni2.rucksack_list[it]);
            new_unit1->rucksack_list.push_back(uni1.rucksack_list[it]);
        }
    }
}

std::pair<Unit,Unit> Unit::crossover_operation(Unit& uni1, Unit& uni2, int opt)
{
    Unit cross_unit1;
    Unit cross_unit2;
    if(uni1.rucksack_list.size() != uni2.rucksack_list.size())
    {
        std::cout << "ERROR! Different sizes of rucksuck lists!\n";
    }
    else
    {
        if(opt == 0)
            shuffle_units(uni1, uni2, &cross_unit1, &cross_unit2);
        else if (opt == 1)
        {
            int p = rand() % uni1.rucksack_list.size() - 2;
            ox_operator(uni1, uni2, &cross_unit1, &cross_unit2, p);
        }
        else if (opt == 2)
        {
            int p1 = rand() % uni1.rucksack_list.size() - 2;
            int p2 = rand() % uni1.rucksack_list.size() - 2;
            if(p1 > p2)
                pmx_operator(uni1, uni2, &cross_unit1, &cross_unit2, p1, p2);
            else
                pmx_operator(uni1, uni2, &cross_unit1, &cross_unit2, p2, p1);
        }
    }
    return std::make_pair(cross_unit1, cross_unit2);
}

std::vector<int> Unit::range(int i)
{
    std::vector<int> range_list;
    for(int it = 0; it < i; ++it)
        range_list.push_back(it);
    return range_list;
}

void Unit::mutation_operator_1(Unit* uni, std::vector<Item> items)
{
    for (int i = 0; i < uni->rucksack_list.size(); ++i)
    {
        unsigned int rnd_it = rand() % uni->rucksack_list[i].items_index.size();
        uni->rucksack_list[i].items_index.erase(uni->rucksack_list[i].items_index.begin() + rnd_it);
    }

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
                  [items](const int a, const int b) -> bool
                  {
                      return items[a].weight > items[b].weight;
                  });
        for(auto& ruck: uni->rucksack_list)
        {
            std::vector<int> item_index_copy = items_index;
            for(auto& it: item_index_copy)
            {
                if (ruck.capacity_left(items) >= items[it].weight)
                {
                    ruck.items_index.push_back(it);
                    items_index.erase(std::find(items_index.begin(), items_index.end(), it));
                }
            }
        }
    }
}

void Unit::mutation_operator_2(Unit* uni, std::vector<Item> items)
{
    int count = (int(uni->rucksack_list.size()) / 3) + 1;
    for(int i = 0; i < count; ++i)
    {
        unsigned int rnd_it = rand()% uni->rucksack_list.size();
        uni->rucksack_list[rnd_it].items_index.clear();
    }

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
                  [items](const int a, const int b) -> bool
                  {
                      return (items[a].profit/items[a].weight) > (items[b].profit/items[b].weight);
                  });
        for(auto& ruck: uni->rucksack_list)
        {
            std::vector<int> item_index_copy = items_index;
            for(auto& it: item_index_copy)
            {
                if (ruck.capacity_left(items) >= items[it].weight)
                {
                    ruck.items_index.push_back(it);
                    items_index.erase(std::find(items_index.begin(), items_index.end(), it));
                }
            }
        }
    }
}

void Unit::mutation_operator_3(Unit* uni, std::vector<Item> items)
{
    int rnd_1 = rand() % uni->rucksack_list.size();
    int rnd_2 = rand() % uni->rucksack_list.size();

    std::vector<int> buff = uni->rucksack_list[rnd_1].items_index;
    uni->rucksack_list[rnd_1].items_index = uni->rucksack_list[rnd_2].items_index;
    uni->rucksack_list[rnd_2].items_index = buff;

    while(uni->rucksack_list[rnd_1].capacity_left(items) < 0)
    {
        int rnd = rand() % uni->rucksack_list[rnd_1].items_index.size();
        uni->rucksack_list[rnd_1].items_index.erase(uni->rucksack_list[rnd_1].items_index.begin()+rnd);
    }

    while(uni->rucksack_list[rnd_2].capacity_left(items) < 0)
    {
        int rnd = rand() % uni->rucksack_list[rnd_2].items_index.size();
        uni->rucksack_list[rnd_2].items_index.erase(uni->rucksack_list[rnd_2].items_index.begin()+rnd);
    }

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
                  [items](const int a, const int b) -> bool
                  {
                      return (items[a].profit/items[a].weight) > (items[b].profit/items[b].weight);
                  });
        for(auto& ruck: uni->rucksack_list)
        {
            std::vector<int> item_index_copy = items_index;
            for(auto& it: item_index_copy)
            {
                if (ruck.capacity_left(items) >= items[it].weight)
                {
                    ruck.items_index.push_back(it);
                    items_index.erase(std::find(items_index.begin(), items_index.end(), it));
                }
            }
        }
    }
}

