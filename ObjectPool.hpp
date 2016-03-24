#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include "classes/String.hpp"
#include <list>
#include <vector>

struct ObjectPool {
    std::vector< std::pair< int, String*> > strings;
    std::list<int> freeStringsList;
    ObjectPool() {
    	strings.resize(10000);
    	for(int i=0;i<10000;i++) freeStringsList.push_back(i);
    }
};

#endif