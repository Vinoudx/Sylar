#ifndef __THREAD__
#define __THREAD__

#include <thread>
#include <functional>
#include <memory>
#include <string>
#include <shared_mutex>
#include <mutex>

#include "util.hpp"
#include "logger.hpp"

namespace sylar{

class Semaphore: public noncopyable{
public:
    Semaphore(uint64_t initial);
    ~Semaphore();
    void wait();
    void notify();
private:
    sem_t semaphore_;
};

class Thread: public noncopyable{
public:
    using ptr_t = std::shared_ptr<Thread>;
    using ThreadCallback = std::function<void()>; 

    Thread(ThreadCallback cb, const std::string& name);
    ~Thread();

    void join();
    const std::string& getName() const{return name_;}
    
    // 处理线程对象的属性
    void setName(const std::string& name){name_ = name;}
    pid_t getId() {return tid_;}

    void run();

    static Thread* getThis();

    // 处理调用这个函数的线程的thread_local
    static void SetName(const std::string& name);
    static const std::string& GetName();
    
private:
    std::string name_;
    pid_t tid_;
    std::unique_ptr<std::thread> thread_;
    ThreadCallback cb_;
    // 让线程类在线程初始化完成后再退出构造函数
    Semaphore sem_;
};

class RWLock : public noncopyable {
public:
    RWLock() = default;
    ~RWLock() = default;

    std::shared_mutex& get_mutex() { return smtx_; }

private:
    std::shared_mutex smtx_;
};

class ReadLockGuard {
public:
    ReadLockGuard(RWLock& lock) : lock_(lock), guard_(lock_.get_mutex()) {}
private:
    RWLock& lock_;
    std::shared_lock<std::shared_mutex> guard_;
};

class WriteLockGuard {
public:
    WriteLockGuard(RWLock& lock) : lock_(lock), guard_(lock_.get_mutex()) {}
private:
    RWLock& lock_;
    std::unique_lock<std::shared_mutex> guard_;
};

}


#endif