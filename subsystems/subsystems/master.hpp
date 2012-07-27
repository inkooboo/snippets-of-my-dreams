//
//  master.hpp
//  subsystems
//
//  Created by Andrey Kubarkov on 7/27/12.
//

#ifndef __MASTER_HPP__
# define __MASTER_HPP__

# include "subsystem.hpp"
# include "../../noncopyable.hpp"

# include <vector>
# include <cassert>

class master_t : private noncopyable_t
{
public:
    template <typename SubsystemType, typename... Args>
    inline void add_managed_subsystem(Args ...args);

    template <typename SubsystemType, typename... Args>
    inline void add_unmanaged_subsystem(Args ...args);

    template <typename SubsystemType>
    inline void add_external_subsystem(SubsystemType *raw_pointer);
    
    template <typename SubsystemType>
    inline SubsystemType & subsystem();
    
    inline void start();
    inline void stop();
    
    inline ~master_t();
    
private:
    std::vector<subsystem_t *> m_subsystems;
};

// Implementation

inline void master_t::start()
{
    for (auto &subsystem : m_subsystems)
    {
        subsystem->start();
    }
}

inline void master_t::stop()
{
    for (auto &subsystem : m_subsystems)
    {
        subsystem->stop();
    }
}

inline master_t::~master_t()
{
    for (auto &subsystem : m_subsystems)
    {
        delete subsystem;
    }
}

namespace internal
{
	template <typename SubsystemType>
	inline SubsystemType ** subsystem_instance()
	{
	    static SubsystemType *instance = 0;
	    return &instance;
	}

	template <typename SubsystemType>
	struct unmanaged_holder_t : public subsystem_t
	{
		template <typename... Args>
		inline unmanaged_holder_t(Args ...args) : holder(args...) {}
		SubsystemType holder;
	};
}

template <typename SubsystemType, typename... Args>
inline void master_t::add_managed_subsystem(Args ...args)
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance == 0);

    *instance = new SubsystemType(args...);
    static_cast<subsystem_t *>(*instance)->m_master = this;
    m_subsystems.push_back(*instance);
}

template <typename SubsystemType, typename... Args>
inline void master_t::add_unmanaged_subsystem(Args ...args)
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance == 0);

    internal::unmanaged_holder_t<SubsystemType> *unmanaged_holder = new internal::unmanaged_holder_t<SubsystemType>(args...);
    *instance = &(unmanaged_holder->holder);
    m_subsystems.push_back(unmanaged_holder);
}

template <typename SubsystemType>
inline void master_t::add_external_subsystem(SubsystemType *raw_pointer)
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance == 0);

    *instance = raw_pointer;
}

template <typename SubsystemType>
inline SubsystemType & master_t::subsystem()
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance != 0);
    
    return **instance;
}

#endif //__MASTER_HPP__
