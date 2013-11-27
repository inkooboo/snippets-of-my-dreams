TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += main.cpp \
    test/config.cpp \
    test/subsystem.cpp \
    test/fixed_function.cpp

HEADERS += \
    noncopyable.hpp \
    config.hpp \
    subsystem.hpp \
    fixed_function.hpp

