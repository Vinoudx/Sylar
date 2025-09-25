#include "util.hpp"

namespace sylar{

size_t getFiberId(){
    return 0;
}

uint32_t getThreadId(){
    return gettid();
}

}