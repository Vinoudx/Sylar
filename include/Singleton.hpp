#ifndef __SINGLETON__
#define __SINGLETON__

#include <memory>
#include <mutex>

template<typename T>
class Singleton{
public:
    template<typename ...Args>
    static std::shared_ptr<T> getSingleton(Args&&... args){
        std::call_once(flag_, [&args...]{
            if(ptr_ == nullptr){
                ptr_.reset(new T(std::forward<Args>(args)...));
            }
        });
        return ptr_;
    }
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
private:
    static std::shared_ptr<T> ptr_;
    static std::once_flag flag_;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::ptr_ = nullptr;

template<typename T>
std::once_flag Singleton<T>::flag_;

#endif

