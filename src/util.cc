#include "util.hpp"

namespace sylar{

static thread_local pid_t t_tid = -1;

size_t getFiberId(){
    return 0;
}

pid_t getThreadId(){
    if(t_tid == -1) [[unlikely]]{
        t_tid = gettid();
    }
    return t_tid;
}

}