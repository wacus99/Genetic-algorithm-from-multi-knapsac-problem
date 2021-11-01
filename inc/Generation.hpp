#ifndef _GENERATION_HPP_
#define _GENERATION_HPP_

#include "types.hpp"
#include "Unit.hpp"

class Generation
{

private:
    const count_t units_count;
    Unit* units;

public:
    Generation(const count_t size, Unit* ptr);
    ~Generation();

    count_t get_units_count();
    const Unit get_unit(const count_t index);

};

#endif /* _GENERATION_HPP_ */