//
//  main.cpp
//  subsystems
//
//  Created by Admin on 7/18/12.
//

#include "subsystem.hpp"
#include "master.hpp"

#include <iostream>

class capturer
{
public:
    
    capturer(int x, int y, char f)
        : m_x(x + y - f)
    {}
    
    int get_x()
    {
        return m_x;
    }
    
private:
    int m_x;
};

class render : public subsystem_t
{
public:
    render(const char *message)
        : m_message(message)
    {
    }
    
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
        int n = master().subsystem<capturer>().get_x();
        
        std::cout << m_message << n << std::endl;
    }
    
private:
    const char *m_message;
};

int main()
{
    master_t master;
    
    master.add_subsystem<capturer>(42, 0, 'a');
    master.add_subsystem<render>("Yababadaba-DOOOO:");

    master.start();
    
    master.subsystem<render>().start_render();
    
    master.stop();
    
    return 0;
}

