#include <callback.hpp>
#include <cassert>
#include <functional>

int var = 0;

void free_func1()
{
    var = 1;
}

void free_func2(int v)
{
    var = v;
}



using namespace std::placeholders;

int callback_test()
{
    callback_t<32> cbff1(std::bind(free_func1));
    cbff1();
    assert(1 == var);

    callback_t<32> cbff2(std::bind(free_func2, 2));
    cbff2();
    assert(2 == var);



    return 0;
}
