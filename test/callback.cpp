#include <callback.hpp>
#include <cassert>
#include <functional>
#include <string>

int var = 0;

void free_func1()
{
    var = 1;
}

void free_func2(int v)
{
    var = v;
}

int free_func3()
{
    return 3;
}


struct Foo {
    std::string method_var = "method";

    std::string * method() {
        return &method_var;
    }

    static std::string static_method() {
        return "static_method";
    }
};

using namespace std::placeholders;

int callback_test()
{
    callback_t<void> cbff1(std::bind(free_func1));
    cbff1();
    assert(1 == var);

    callback_t<void> cbff2(std::bind(free_func2, 2));
    cbff2();
    assert(2 == var);

    callback_t<int> cbff3(std::bind(free_func3));
    assert(3 == cbff3());

    Foo foo;
    callback_t<std::string *> cb_class_method(std::bind(&Foo::method, &foo));
    assert(&foo.method_var == cb_class_method());

    callback_t<std::string> cbclass_static_method(std::bind(&Foo::static_method));
    assert(cbclass_static_method() == "static_method");

    return 0;
}
