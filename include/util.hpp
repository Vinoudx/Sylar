#ifndef __UTIL__
#define __UTIL__

#include <unistd.h>
#include <stdint.h>

namespace sylar{

class noncopyable{
public:
    noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable(noncopyable&&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};


size_t getFiberId();
pid_t getThreadId();

}

#endif