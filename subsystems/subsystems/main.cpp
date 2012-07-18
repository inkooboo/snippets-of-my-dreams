//
//  main.cpp
//  subsystems
//
//  Created by Admin on 7/18/12.
//

#include "subsystem.hpp"
#include "master.hpp"

#include <iostream>

class capturer : public subsystem_t
{
public:
    
    int get_int()
    {
        return 42;
    }

private:
    friend class master_t;
    explicit capturer(master_t *master)
        : subsystem_t(master)
    {
    }
};

class render : public subsystem_t
{
public:
    
    virtual void start() 
    {
        std::cout << "render start" << std::endl;
    }
    
    virtual void stop() 
    {
        std::cout << "render stop" << std::endl;
    }

    
    void start_render()
    {
        int n = master().subsystem<capturer>().get_int();
        
        std::cout << "render " << n << std::endl;
    }
    
private:
    friend class master_t;
    explicit render(master_t *master)
        : subsystem_t(master)
    {
    }
};


int main()
{
    master_t master;
    
    master.add_subsystem<capturer>();
    master.add_subsystem<render>();

    master.start();
    
    master.subsystem<render>().start_render();
    
    return 0;
}

