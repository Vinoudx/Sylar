#ifndef __UTIL__
#define __UTIL__

#include <unistd.h>
#include <stdint.h>

namespace sylar{

size_t getFiberId();
uint32_t getThreadId();

}

#endif