#ifndef _GENETIC_ALGORITHM_HPP
#define _GENETIC_ALGORITHM_HPP

#include "Generation.hpp"

class GeneticAlgorithm
{
private:
    Generation generation;
    
    const count_t generation_size;
    const count_t generation_size_limit;

public:
    GeneticAlgorithm(const count_t size, const count_t size_limit);
    
};

#endif /* _GENETIC_ALGORITHM_HPP_ */