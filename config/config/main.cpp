//
//  main.cpp
//  config
//
//  Created by Andrey Kubarkov on 7/18/12.
//

#include "config.hpp"

#include <cassert>

int main()
{
    {
        config_t config("test1.txt", config_t::store_on_destruction);
        
        config.set("string", "Hello World");
        config.set("int", 42);
        config.set("double", 42.42);
        config.set("bool", true);
    }
    
    {
        config_t config("test1.txt");
        
        assert(config.get<std::string>("string") == "Hello World");
        assert(config.get<int>("int") == 42);
        assert(config.get<double>("double") == 42.42);
        assert(config.get<bool>("bool") == true);
    }
    
    return 0;
}

