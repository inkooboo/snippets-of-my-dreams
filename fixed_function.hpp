#ifndef FIXED_FUNCTION_HPP
#define FIXED_FUNCTION_HPP

#include <noncopyable.hpp>
#include <type_traits>
#include <cstring>
#include <utility>

template <typename R, typename... ARGS>
class fixed_function_t : private noncopyable_t {
    enum {STORAGE_SIZE = 32};
    enum {ALIGNMENT = 8};
public:
    fixed_function_t()
        : m_object_ptr(&m_storage)
        , m_method_ptr(nullptr)
        , m_delete_ptr(nullptr)
    {
    }

    template <typename U>
    fixed_function_t(U &&object)
    {
        typedef typename std::remove_reference<U>::type unref_type;

        static_assert(sizeof(unref_type) < STORAGE_SIZE,
                      "functional object don't fit into internal storage");

        m_object_ptr = new (&m_storage) unref_type(std::forward<U>(object));
        m_method_ptr = &method_stub<unref_type>;
        m_delete_ptr = &delete_stub<unref_type>;
    }

    fixed_function_t(fixed_function_t &&o)
    {
        move_from_other(o);
    }

    fixed_function_t & operator=(fixed_function_t &&o)
    {
        move_from_other(o);
        return *this;
    }

    ~fixed_function_t()
    {
        if (m_delete_ptr) {
            (*m_delete_ptr)(m_object_ptr);
        }
    }

    R operator()(ARGS... args) const
    {
        if (m_method_ptr) {
            return (*m_method_ptr)(m_object_ptr, args...);
        }
        return R();
    }

private:
    typename std::aligned_storage<STORAGE_SIZE, ALIGNMENT>::type m_storage;

    void *m_object_ptr;

    typedef R (*method_type)(void *, ARGS...);
    method_type m_method_ptr;

    typedef void (*delete_type)(void *);
    delete_type m_delete_ptr;

    void move_from_other(fixed_function_t &o)
    {
        m_method_ptr = o.m_method_ptr;
        m_delete_ptr = o.m_delete_ptr;

        memcpy(&m_storage, &o.m_storage, STORAGE_SIZE);

        o.m_method_ptr = nullptr;
        o.m_delete_ptr = nullptr;
    }

    template <class T>
    static R method_stub(void *object_ptr, ARGS... args)
    {
        return static_cast<T *>(object_ptr)->operator()(args...);
    }

    template <class T>
    static void delete_stub(void *object_ptr)
    {
        static_cast<T *>(object_ptr)->~T();
    }
};


#endif
