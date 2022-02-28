#ifndef TYPES_HPP
#define TYPES_HPP

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
    std::vector<Item> items;
    int capacity;
    explicit Rucksack(int _capacity):capacity(_capacity){};
    int capacity_left()
    {
        int sum = 0;
        for (auto &item: items)
        {
            sum += item.weight;
        }
        return (capacity > sum) ? capacity - sum : 0;
    }
} Rucksack;


typedef struct Unit
{
    std::vector<Rucksack> rucksack_list;
    Unit() = default;
    explicit Unit(std::vector<Rucksack> list):rucksack_list(std::move(list)){};
    void clear_rucksacks()
    {
        for(auto& ruck: rucksack_list)
        {
            ruck.items.clear();
        }
    }
    int count_result()
    {
        int sum = 0;
        for (auto& ruck: rucksack_list)
        {
            for (auto& item: ruck.items)
            {
                sum += item.profit;
            }
        }
        return sum;
    }

    static void print_unit(Unit& uni)
    {
        for (auto& ruk: uni.rucksack_list)
        {
            for (auto& el: ruk.items)
                std::cout << "[" << el.profit << ", " << el.weight << "]";
            std::cout << "Capacity left:" << ruk.capacity_left();
            std::cout << "\n";
        }
        std::cout << uni.count_result();
        std::cout << "\n\n";
    }

    static void shuffle_units(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2)
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

    static void ox_operator(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2, int p) // losowanie miedzy 0 a size-2
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

    static void pmx_operator(Unit& uni1, Unit& uni2, Unit* new_unit1, Unit* new_unit2, int p1, int p2) //losowanie miedzy 0 a size-2
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

    static std::pair<Unit,Unit> crossover_operation(Unit& uni1, Unit& uni2, int opt = 0)
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

    static std::vector<int> range(int i)
    {
        std::vector<int> range_list;
        for(int it = 0; it < i; ++it)
            range_list.push_back(it);
        return range_list;
    }

    static void mutation_operator(Unit* uni, std::vector<Item> items)
    {
        std::cout << "\nErasing:";
        for (int i = 0; i < uni->rucksack_list.size(); ++i)
        {
            unsigned int rnd_it = rand() % uni->rucksack_list[i].items.size();
            uni->rucksack_list[i].items.erase(uni->rucksack_list[i].items.begin() + rnd_it);
        }
        std::cout << "\n";
        std::vector<int> items_index = range(items.size());
        for(auto& ruck: uni->rucksack_list)
        {
            std::vector<Item> items_copy = ruck.items;
            for(auto& it: items_copy)
            {
                if (!bool(std::find(items.begin(), items.end(), it) == items.end()))
                {
                    items.erase(std::find(items.begin(), items.end(), it));
                }
            }
        }

        if (items.size() > 0)
        {
            std::sort(items.begin(), items.end(),
                      [](const Item a, const Item b) -> bool
                      {
                          return a.weight > b.weight;
                      });
            for(auto& ruck: uni->rucksack_list)
            {
                std::vector<Item> items_copy = items;
                for(auto& item: items_copy)
                {
                    if (ruck.capacity_left() >= item.weight)
                    {
                        ruck.items.push_back(item);
                        items.erase(std::find(items.begin(), items.end(), item));
                    }
                }
            }
        }
    }

} Unit;

#endif //TYPES_HPP
