#include "Generation.hpp"

void Generation::update_result(std::vector<Item> items)
{
    result_list.clear();
    for(auto& uni: unit_list)
        result_list.push_back(uni.count_result(items));
}

void Generation::delete_worst_units(int count, std::vector<Item> items)
{
    std::vector<int> worst_results;
    worst_results = result_list;
    std::sort(worst_results.begin(), worst_results.end());
    worst_results = std::vector<int>(worst_results.begin(), worst_results.begin()+count);
    for (auto& result: worst_results)
    {
        for (auto unit_it = unit_list.begin(); unit_it != unit_list.end(); ++unit_it)
        {
            if (result == unit_it->count_result(items))
            {
                unit_list.erase(unit_it);
                break;
            }
        }
    }
}

void Generation::rulette_selection_1(int count, std::vector<Item> items)
{
    int results_sum = 0;
    std::vector<float> segments;
    results_sum = std::accumulate(result_list.begin(), result_list.end(), 0);
    for (auto i = 0; i < result_list.size(); ++i)
    {
        float segment;
        if (i == 0)
            segment = (float(result_list[i])/float(results_sum))*1000.0F;
        else
            segment = (float(result_list[i])/float(results_sum))*1000.0F + segments[i-1];
        segments.push_back(segment);
    }
    std::vector<int> draw_numbers;
    std::vector<Unit> new_unit_list;
    for(auto i = 0; i < unit_list.size()-count; ++i)
    {
        draw_numbers.push_back(rand()%1000);
        for(auto seg_it = 0; seg_it < segments.size(); ++seg_it)
        {
            int min,max;
            max = segments[seg_it];
            min = seg_it == 0 ? 0 : segments[seg_it-1];
            if (float(draw_numbers[i]) >= min && float(draw_numbers[i]) <= max)
            {
                new_unit_list.push_back(unit_list[seg_it]);
                break;
            }
        }
    }
    unit_list = new_unit_list;
    update_result(items);
}

void Generation::rulette_selection_2(int count, std::vector<Item> items)
{
    int max_result = *std::max_element(result_list.begin(), result_list.end());
    int diff_sum = 0;
    for (auto &result: result_list)
        diff_sum = diff_sum + (max_result-result);
    if(diff_sum != 0)
    {
        std::vector<float> segments;
        for (auto i = 0; i < result_list.size(); ++i)
        {
            float segment;
            if (i == 0)
                segment = (float(max_result-result_list[i])/float(diff_sum))*1000.0F;
            else
                segment = (float(max_result-result_list[i])/float(diff_sum))*1000.0F + segments[i-1];
            segments.push_back(segment);
        }
        std::vector<int> index_to_del;
        for(auto i = 0; i < count; ++i)
        {
            int draw_number = rand()%1000;
            for(auto seg_it = 0; seg_it < segments.size(); ++seg_it)
            {
                int min,max;
                max = segments[seg_it];
                min = seg_it == 0 ? 0 : segments[seg_it-1];
                if (float(draw_number) >= min && float(draw_number) <= max)
                {
                    index_to_del.push_back(seg_it);
                    break;
                }
            }
        }
        std::sort(index_to_del.begin(), index_to_del.end(), [](int a, int b){return a > b;});
        for(auto &index: index_to_del)
            unit_list.erase(unit_list.begin()+index);
    }
    else
    {
        unit_list.erase(unit_list.begin(), unit_list.begin()+count);
    }
    update_result(items);
}

std::vector<int> Generation::range(int i)
{
    std::vector<int> range_list;
    for(int it = 0; it < i; ++it)
        range_list.push_back(it);
    return range_list;
}

void Generation::check_unit(Unit* uni, std::vector<Item> items)
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
              [items](const int a, const int b) -> bool
              {
                  return items[a].profit > items[b].profit;
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

int Generation::get_max_result(std::vector<Item> items)
{
    this->update_result(items);
    return *std::max_element(result_list.begin(), result_list.end());
}

Unit Generation::get_max_unit(std::vector<Item> items)
{
    Unit unit;
    this->update_result(items);
    for(auto i = 0; i < result_list.size(); i++)
    {
        if (result_list[i] == get_max_result(items))
            unit = unit_list[i];
    }
    return unit;
}

void Generation::selection(int count, int method_id, std::vector<Item> items)
{
    if(method_id == 0)
        delete_worst_units(count, items);
    else if (method_id == 1)
        rulette_selection_1(count, items);
    else if (method_id == 2)
        rulette_selection_2(count, items);
}

void Generation::crossover(std::vector<Item> items, int count, int method_id)
{
    for (int i = 0; i < count; i+=2)
    {
        std::pair<Unit, Unit> cross_uni = Unit::crossover_operation(unit_list[i],unit_list[i+1], method_id);
        check_unit(&cross_uni.first, items);
        check_unit(&cross_uni.second, items);
        unit_list.push_back(cross_uni.first);
        unit_list.push_back(cross_uni.second);
    }
}

void Generation::mutation(int count_to_mute, std::vector<Item> items, int opt)
{
    for (int i = 0; i < count_to_mute; ++i)
    {
        unsigned int it = rand() % unit_list.size();
        if (opt == 0)
            Unit::mutation_operator_1(&unit_list[it], items);
        else if (opt == 1)
            Unit::mutation_operator_2(&unit_list[it], items);
        else if (opt == 2)
            Unit::mutation_operator_3(&unit_list[it], items);
    }
}