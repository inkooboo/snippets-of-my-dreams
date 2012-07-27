//
//  master.hpp
//  subsystems
//
//  Created by Andrey Kubarkov on 7/27/12.
//

#include "subsystem.hpp"
#include "master.hpp"

#include <iostream>

class single
{
public:
    static single & singleton()
    {
        static single single_instance;
        return single_instance;
    }

    void do_nothing()
    {
        std::cout << "single do_nothing" << std::endl;
    }

private:
    single() {};
};

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

        master().subsystem<single>().do_nothing();
    }
    
private:
    const char *m_message;
};

struct foo
{};

int main()
{
    master_t master;
    
    master.add_unmanaged_subsystem<capturer>(42, 0, 'a');
    master.add_unmanaged_subsystem<foo>();
    master.add_managed_subsystem<render>("Yababadaba-DOOOO:");
    master.add_external_subsystem<single>(&single::singleton());
    master.start();
    
    master.subsystem<render>().start_render();
    
    master.stop();
    
    return 0;
}

