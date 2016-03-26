#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include "classes/String.hpp"
#include <list>
#include <vector>

struct ObjectPool {
    std::vector< std::pair< int, String*> > strings;
    std::list<int> freeStringsList;
    ObjectPool() {
        strings.reserve(10000);
    }
};

#endif
