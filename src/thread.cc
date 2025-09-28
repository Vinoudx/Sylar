#include "thread.hpp"

#include "util.hpp"

namespace sylar{

Semaphore::Semaphore(uint64_t initial){
    if(sem_init(&semaphore_, 0, initial) == -1){
        throw std::logic_error("sem_init fail");
    }
}

Semaphore::~Semaphore(){
    sem_destroy(&semaphore_);
}

void Semaphore::wait(){
    if(sem_wait(&semaphore_) == -1){
        throw std::logic_error("sem_wait fail");
    }
}

void Semaphore::notify(){
    if(sem_post(&semaphore_)){
        throw std::logic_error("sem_post fail");
    }
}


static thread_local Thread* t_this = 0;
static thread_local std::string t_name = "UNKOWN";

Thread::Thread(ThreadCallback cb, const std::string& name):cb_(std::move(cb)),name_(name),tid_(getThreadId()),sem_(0){
    thread_ = std::make_unique<std::thread>(std::bind(&Thread::run, this));
    sem_.wait();
}

Thread::~Thread(){
    join();
}

void Thread::join(){
    if(thread_->joinable()){
        thread_->join();
    }
}

Thread* Thread::getThis(){
    return t_this;
}


void Thread::SetName(const std::string& name){
    t_this->setName(name);
    t_name = name;
}

const std::string& Thread::GetName(){
    return t_name;
}

void Thread::run(){
    t_name = name_;
    t_this = this;
    ThreadCallback cb;
    cb.swap(cb_);
    sem_.notify();
    cb();
}


}