#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <list>
#include <vector>

#include "classes/String.hpp"

struct ObjectPool {
    std::vector<std::pair<int, String*>> strings;
    std::list<int> freeStringsList;
    ObjectPool() {
        strings.reserve(10000);
    }
};

#endif
