#include <fixed_function.hpp>
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

int free_func4(int x) {
    return x;
}

std::string free_func5(const std::string &x, const std::string &y) {
    return x + y;
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

int fixed_function_test()
{
    fixed_function_t<void> cbff1(std::bind(free_func1));
    cbff1();
    assert(1 == var);

    fixed_function_t<void> cbff2(std::bind(free_func2, 2));
    cbff2();
    assert(2 == var);

    fixed_function_t<int> cbff3(std::bind(free_func3));
    assert(3 == cbff3());

    fixed_function_t<int, int> cbff4(std::bind(free_func4, _1));
    assert(4 == cbff4(4));

    fixed_function_t<std::string, std::string, std::string> cbff5(std::bind(free_func5, _1, _2));
    assert("55" == cbff5("5", "5"));

    Foo foo;
    fixed_function_t<std::string *> cb_class_method(std::bind(&Foo::method, &foo));
    assert(&foo.method_var == cb_class_method());

    fixed_function_t<std::string> cbclass_static_method(std::bind(&Foo::static_method));
    assert(cbclass_static_method() == "static_method");

    return 0;
}
