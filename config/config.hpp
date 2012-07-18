//
//  config.hpp
//  config
//
//  Created by Andrey Kubarkov on 7/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef config_config_hpp
#define config_config_hpp

#include <string>
#include <sstream>
#include <fstream>
#include <map>

class config_t
{
public:
    
    enum store_policy
    {
          store_on_destruction
        , store_each_change
    };
    
    inline explicit config_t(const char *file_name, store_policy policy = store_on_destruction);
    inline ~config_t();
    
    template <typename T>
    inline void set(const char *name, const T &value);
    
    template <typename T>
    inline T get(const char *name);
    
private:
    
    inline void store();
    inline void load();
    
    inline std::string escape(const std::string &str);
    inline std::string unescape(const std::string &str);
    
    std::string m_file_name;
    store_policy m_policy;
    std::map<std::string, std::string> m_properties;
};

// implementation

inline config_t::config_t(const char *file_name, store_policy policy)
    : m_file_name(file_name)
    , m_policy(policy)
{
    load();
}

inline config_t::~config_t()
{
    if (store_on_destruction == m_policy)
    {
        store();
    }
}

template <typename T>
inline void config_t::set(const char *name, const T &value)
{
    std::stringstream ss;
    ss << value;
    m_properties[name] = ss.str();
    
    if (store_each_change == m_policy)
    {
        store();
    }
}

template <typename T>
inline T config_t::get(const char *name)
{
    T ret;
    
    std::stringstream ss;
    ss << m_properties[name];
    ss >> ret;
    
    return ret;
}

template <>
inline std::string config_t::get<std::string>(const char *name)
{
    return m_properties[name];
}


inline void config_t::store()
{
    std::ofstream f(m_file_name.c_str(), std::ios_base::out | std::ios_base::trunc);
    
    for (auto &property : m_properties)
    {
        f << escape(property.first) << " " << escape(property.second) << std::endl;
    }
}

inline void config_t::load()
{
    std::ifstream f(m_file_name.c_str(), std::ios_base::in);
    
    if (!f.is_open())
    {
        return;
    }
    
    while (!f.eof()) 
    {
        std::string line;
        std::getline(f, line, '\n');
        
        size_t space = line.find(' ');
        if (std::string::npos == space)
        {
            continue;
        }
        
        std::string name = unescape(line.substr(0, space));
        std::string value = unescape(line.substr(space + 1, line.length() - space - 1));
        
        m_properties[name] = value;
    }
}

inline std::string config_t::escape(const std::string &str)
{
    std::string ret;
    ret.reserve(str.length() * 2);
    
    size_t i = 0;
    while (i < str.length())
    {
        switch (str[i])
        {
            case ' ' : {
                ret += "%0";
                break;
            }
            case '\n' : {
                ret += "%1";
                break;
            }
            case '%' : {
                ret += "%2";
                break;
            }
            default : {
                ret += str[i];
                break;
            }
        }
        ++i;
    }
    return ret;
}

inline std::string config_t::unescape(const std::string &str)
{
    std::string ret;
    ret.reserve(str.length());
    
    size_t i = 0;
    while (i < str.length())
    {
        switch (str[i])
        {
            case '%' : {
                ++i;
                switch (str[i])
                {
                    case '0' : {
                        ret += ' ';
                        break;
                    }
                    case '1' : {
                        ret += '\n';
                        break;
                    }
                    case '2' : {
                        ret += '%';
                        break;
                    }
                }                
                break;
            }
            default : {
                ret += str[i];
                break;
            }
        }
        ++i;
    }
    return ret;
}


#endif
