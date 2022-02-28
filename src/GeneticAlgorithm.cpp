#include "GeneticAlgorithm.hpp"

Generation GeneticAlgorithm::create_generation(int size)
{
    Generation gen;
    std::vector<Rucksack> ruck_list;
    for (auto& capacity: rucksacks_capacities)
    {
        ruck_list.emplace_back(capacity);
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

void GeneticAlgorithm::draw_unit(Unit* uni)
{
    std::vector<Item> items_copy(items_list);
    for(int j = 0; j < items_list.size(); j++)
    {
        unsigned int it = rand() % items_copy.size();
        for (auto& ruk: uni->rucksack_list)
        {
            if(ruk.capacity_left(items_list) >= items_copy[it].weight)
            {
                ruk.items_index.push_back(std::find(items_list.begin(), items_list.end(), items_copy[it]) - items_list.begin());
                items_copy.erase(items_copy.begin()+it);
                break;
            }
        }
    }
}


Results GeneticAlgorithm::run(int generation_size, int count_to_del, int count_to_mute, int sel_id, int cross_id, int mut_id)
{
    std::clock_t start = std::clock();
    std::vector<std::vector<int>> pBest;
    int gBest=0;
    Unit gBest_uni;
    this->gen = create_generation(generation_size);
    int flag = 0;
    int it = 0;
    while(flag < 100)
    {
        gen.update_result(items_list);
        std:: cout << gen.get_max_result(items_list) << "," <<
                      std::accumulate(gen.result_list.begin(), gen.result_list.end(),0)/double(gen.result_list.size()) << "," <<
                      *(std::min_element(gen.result_list.begin(), gen.result_list.end())) << "\n";
        if(gen.get_max_result(items_list) > gBest)
        {
            it = flag;
        }
        flag++;
        gBest = gen.get_max_result(items_list) > gBest ? gen.get_max_result(items_list) : gBest;
        gen.selection(count_to_del, sel_id, items_list);
        gen.crossover(items_list, count_to_del, cross_id);
        gen.mutation(count_to_mute, items_list, mut_id);
    }
    std::clock_t stop = std::clock();
    double time = 1000.0*(stop-start)/CLOCKS_PER_SEC;
    return Results(it, gBest, time);
}

