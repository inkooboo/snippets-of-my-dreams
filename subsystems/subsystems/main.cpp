//
//  main.cpp
//  subsystems
//
//  Created by Andrey Kubarkov on 7/27/12.
//

#include "subsystem.hpp"
#include "master.hpp"

#include <iostream>
#include <string>

class processor
{
public:
    static processor & singleton()
    {
        static processor instance;
        return instance;
    }

    void * process(void *data)
    {
        std::cout << "processor: process" << std::endl;
        return data;
    }

private:
    processor() {};
    processor(const processor&) {};
};

class capturer
{
public:
    
    capturer(const std::string &file_name)
        : m_file_name(file_name)
    {}
    
    void * frame()
    {
        return nullptr;
    }
    
private:
    const std::string m_file_name;
};

class render : public subsystem_t
{
public:
    render(const std::string &name, int width, int height)
        : m_name(name)
        , m_width(width)
        , m_height(height)
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

    
    void render_frame(void *frame)
    {
        void *processed = master().subsystem<processor>().process(frame);
        
        std::cout << "Render[" << m_name << "]: render_frame " << processed << std::endl;
    }
    
private:
    const std::string m_name;
    int m_width;
    int m_height;
};


int main()
{
    master_t master;
    
    master.add_unmanaged_subsystem<capturer>("test1.avi");
    master.add_managed_subsystem<render>("wnd1:", 640, 480);
    master.add_external_subsystem<processor>(&processor::singleton());
    
    master.start();
    
    //while (true)
    void *frame = master.subsystem<capturer>().frame();
    master.subsystem<render>().render_frame(frame);
    // }
    
    master.stop();
    
    return 0;
}

