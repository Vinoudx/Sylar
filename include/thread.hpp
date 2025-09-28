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

static thread_local std::unique_ptr<std::shared_lock<std::shared_mutex>> rdl = nullptr;
static thread_local std::unique_ptr<std::unique_lock<std::shared_mutex>> wrl = nullptr;

// 读写锁
class RWLock: public noncopyable{
public:
    RWLock() = default;
    ~RWLock() = default;

    void rdlock(){
        if(rdl != nullptr){
            SYLAR_LOG_WARNING << "RWLock, cannot relock read lock";
        }else if(rdl == nullptr && wrl == nullptr){
            rdl = std::make_unique<std::shared_lock<std::shared_mutex>>(smtx_);
            // 持有写锁，将锁降级为读锁
            if(wrl != nullptr){
                wrl.reset(nullptr);
            }          
        }
    }

    void wrlock(){
        if(wrl != nullptr){
            SYLAR_LOG_WARNING << "RWLock, cannot relock write lock";
        }
        if(rdl != nullptr){
            SYLAR_LOG_WARNING << "RWLock, cannot aquire write lock while holding read lock";
        }
        if(rdl == nullptr && wrl == nullptr){
            wrl = std::make_unique<std::unique_lock<std::shared_mutex>>(smtx_);
        }
    }

    void unlock(){
        if(rdl != nullptr){
            rdl.reset(nullptr);
        }
        if(wrl != nullptr){
            wrl.reset(nullptr);
        }
    }

private:
    std::shared_mutex smtx_;
};

// 读写锁guard
class RWLockGuard: public noncopyable{
public:
    RWLockGuard(RWLock& lock):lock_(lock){}
    ~RWLockGuard(){lock_.unlock();}
    void rdlock(){lock_.rdlock();}
    void wrlock(){lock_.wrlock();}
    void unlock(){lock_.unlock();}
private:
    RWLock& lock_;
};

// 读锁guard
class ReadLockGuard: public RWLockGuard{
public:
    ReadLockGuard(RWLock& lock):RWLockGuard(lock){RWLockGuard::rdlock();}
    void lock(){RWLockGuard::rdlock();}
    void unlock(){RWLockGuard::unlock();}
};

// 写锁guard
class WriteLockGuard: public RWLockGuard{
public:
    WriteLockGuard(RWLock& lock):RWLockGuard(lock){RWLockGuard::wrlock();}
    void lock(){RWLockGuard::wrlock();}
    void unlock(){RWLockGuard::unlock();}  
};

}


#endif