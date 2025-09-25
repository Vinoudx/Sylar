#ifndef __CONFIG__
#define __CONFIG__

#include <string>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>

#include "logger.hpp"

namespace sylar{

class ConfigVarBase{
public:
    using ptr_t = std::shared_ptr<ConfigVarBase>;
    ConfigVarBase(const std::string& key, const std::string& desc):key_(key),desc_(desc){}
    virtual ~ConfigVarBase() = default;

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& str) = 0;

    const std::string& getKey() const{return key_;}
    const std::string& getDesc() const{return desc_;}

    virtual const char* getTypeInfo() const = 0;

private:
    std::string key_;
    std::string desc_;
};

// 基础类型的主模板, 支持string
template<typename From, typename To>
struct ConfigCast{
    To operator()(const From& from){
        return boost::lexical_cast<To>(from);
    }
};

template<typename To>
struct ConfigCast<std::string, std::vector<To>>{
    std::vector<To> operator()(const std::string& from){
        YAML::Node node = YAML::Load(from);
        std::vector<To> res;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); it++){
            ss.str("");
            ss << *it;
            // 对vector<vector<T>>这种类型可以递归
            res.push_back(ConfigCast<std::string, To>()(ss.str()));
        }
        return res;
    }
};

template<typename To>
struct ConfigCast<std::vector<To>, std::string>{
    std::string operator()(const std::vector<To>& from){
        YAML::Node node;
        for(auto& item: from){
            node.push_back(YAML::Load(ConfigCast<To, std::string>()(item)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};


template<typename To>
struct ConfigCast<std::string, std::list<To>>{
    std::list<To> operator()(const std::string& from){
        YAML::Node node = YAML::Load(from);
        std::list<To> res;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); it++){
            ss.str("");
            ss << *it;
            // 对vector<vector<T>>这种类型可以递归
            res.push_back(ConfigCast<std::string, To>()(ss.str()));
        }
        return res;
    }
};

template<typename To>
struct ConfigCast<std::list<To>, std::string>{
    std::string operator()(const std::list<To>& from){
        YAML::Node node;
        for(auto& item: from){
            node.push_back(YAML::Load(ConfigCast<To, std::string>()(item)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<typename To>
struct ConfigCast<std::string, std::set<To>>{
    std::set<To> operator()(const std::string& from){
        YAML::Node node = YAML::Load(from);
        std::set<To> res;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); it++){
            ss.str("");
            ss << *it;
            // 对vector<vector<T>>这种类型可以递归
            res.insert(ConfigCast<std::string, To>()(ss.str()));
        }
        return res;
    }
};

template<typename To>
struct ConfigCast<std::set<To>, std::string>{
    std::string operator()(const std::set<To>& from){
        YAML::Node node;
        for(auto& item: from){
            node.push_back(YAML::Load(ConfigCast<To, std::string>()(item)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<typename To>
struct ConfigCast<std::string, std::unordered_set<To>>{
    std::unordered_set<To> operator()(const std::string& from){
        YAML::Node node = YAML::Load(from);
        std::unordered_set<To> res;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); it++){
            ss.str("");
            ss << *it;
            // 对vector<vector<T>>这种类型可以递归
            res.insert(ConfigCast<std::string, To>()(ss.str()));
        }
        return res;
    }
};

template<typename To>
struct ConfigCast<std::unordered_set<To>, std::string>{
    std::string operator()(const std::unordered_set<To>& from){
        YAML::Node node;
        for(auto& item: from){
            node.push_back(YAML::Load(ConfigCast<To, std::string>()(item)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<typename To>
struct ConfigCast<std::string, std::map<std::string, To>>{
    std::map<std::string, To> operator()(const std::string& from){
        YAML::Node node = YAML::Load(from);
        std::map<std::string, To> res;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); it++){
            ss.str("");
            ss << it->second;
            // 对vector<vector<T>>这种类型可以递归
            res.insert(std::make_pair(it->first.Scalar(), ConfigCast<std::string, To>()(ss.str())));
        }
        return res;
    }
};

template<typename To>
struct ConfigCast<std::map<std::string, To>, std::string>{
    std::string operator()(const std::map<std::string, To>& from){
        YAML::Node node;
        for(auto& item: from){
            node[item.first] = (YAML::Load(ConfigCast<To, std::string>()(item.second)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<typename To>
struct ConfigCast<std::string, std::unordered_map<std::string, To>>{
    std::unordered_map<std::string, To> operator()(const std::string& from){
        YAML::Node node = YAML::Load(from);
        std::unordered_map<std::string, To> res;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); it++){
            ss.str("");
            ss << it->second;
            // 对vector<vector<T>>这种类型可以递归
            res.insert(std::make_pair(it->first.Scalar(), ConfigCast<std::string, To>()(ss.str())));
        }
        return res;
    }
};

template<typename To>
struct ConfigCast<std::unordered_map<std::string, To>, std::string>{
    std::string operator()(const std::unordered_map<std::string, To>& from){
        YAML::Node node;
        for(auto& item: from){
            node[item.first] = (YAML::Load(ConfigCast<To, std::string>()(item.second)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

// 配置中的每一项
template<typename T, typename FromString = ConfigCast<std::string, T>, typename ToString = ConfigCast<T, std::string>>
class ConfigVar: public ConfigVarBase{
public:
    using ptr_t = std::shared_ptr<ConfigVar>;

    ConfigVar(const std::string& key, const T& value, const std::string& desc = ""):ConfigVarBase(key, desc),value_(value){}

    // todo 将tostring和fromstring适配 基本类型，string，vector, list，map，set，unordered_map，unordered_set，自定义类
    std::string toString() override {
        try{
            // return boost::lexical_cast<std::string>(value_);
            return ToString()(value_);
        }catch(std::exception& e){
            SYLAR_LOG_ERROR << "ConfigVar::toString, " << e.what() << " converting " << typeid(T).name() << " to std::string";
        }
        return "";
    }

    bool fromString(const std::string& str) override {
        try{    
            // value_ = boost::lexical_cast<T>(str);
            value_ = FromString()(str);
            return true;
        }catch(std::exception& e){
            SYLAR_LOG_ERROR << "ConfigVar::fromString, " << e.what() << " converting std::string to " << typeid(T).name();
        }
        return false;
    }

    const T& getValue() const{return value_;}
    void setValue(const T& value){value_ = value;}

    const char* getTypeInfo() const override{return typeid(T).name();}

private:
    T value_;
};


// 所有配置
class Config{
public:
    using ConfigMap = std::unordered_map<std::string, ConfigVarBase::ptr_t>;

    // 查找，若不存在则返回nullptr
    template<typename T>
    static ConfigVar<T>::ptr_t lookup(const std::string& key){
        auto it = map_.find(key);
        if(it == map_.end()){
            SYLAR_LOG_INFO << "Config::lookup, cannot find config item with key = " << key;
            return nullptr;
        }
        SYLAR_LOG_INFO << "Config::lookup, find config item with key = " << key << " and value = " << it->second->toString();

        if(strcmp(it->second->getTypeInfo(), typeid(T).name()) != 0){
            SYLAR_LOG_ERROR << "Config::lookup, find key " << key << " but have different type, nothing have done";
        }

        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    // 查找，若不存在则插入
    template<typename T>
    static ConfigVar<T>::ptr_t lookup(const std::string& key, const T& defaultVal, const std::string& desc = ""){
        auto ptr = lookup<T>(key);
        auto it = map_.find(key);
        // 找到且类型出错
        if(ptr == nullptr && it != map_.end()){
            return nullptr;
        }
        // 找到且类型正确
        if(ptr != nullptr){
            ptr->setValue(defaultVal);
            return ptr;
        }
        // 没找到
        if(key.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._1234567890") != std::string::npos){
            SYLAR_LOG_ERROR << "Config::lookup, invalid key"; 
            return nullptr;
        }
        auto item = ConfigVarBase::ptr_t(new ConfigVar<T>(key, defaultVal, desc));
        map_.insert_or_assign(key, item);
        SYLAR_LOG_INFO << "Config::lookup, inserting config item with key = " << key << " and value = " << item->toString();
        return std::dynamic_pointer_cast<ConfigVar<T>>(item);
    }

    static void listAllMember(const std::string& prefix, const YAML::Node& node, std::list<std::pair<std::string, YAML::Node>>& members);

    static void loadFromFile(const std::string& path);

    static ConfigVarBase::ptr_t lookUpBase(const std::string& key);

    static void showAllMembers(){
        for(auto it = map_.begin(); it != map_.end(); it++){
            SYLAR_LOG_INFO << it->first << ' ' << it->second->toString();
        }
    }

private:
    static ConfigMap map_;
};

}


#endif