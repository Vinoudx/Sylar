#include "config.hpp"

namespace sylar{

Config::ConfigMap Config::map_;

ConfigVarBase::ptr_t Config::lookUpBase(const std::string& key){
    auto it = map_.find(key);
    return it == map_.end() ? nullptr : it->second;
}

// 把 A:
//      B:value
// 变成
// A.B: value 
// 只对map类型生效，其他的都是直接<<
// 对上述例子，有
// A.B: value (string, value键值对)和
// A:
//    B: value  (string map/unordered_map键值对)
// 两种存在形式
void Config::listAllMember(const std::string& prefix, const YAML::Node& node, std::list<std::pair<std::string, YAML::Node>>& members){
    members.emplace_back(std::make_pair(prefix, node));
    if(node.IsMap()){
        for(auto it = node.begin(); it != node.end(); it++){
            listAllMember(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(), it->second, members);
        }
    }
}

void Config::loadFromFile(const std::string& path){
    std::list<std::pair<std::string, YAML::Node>> members;    
    try{
        YAML::Node node = YAML::LoadFile(path);
        listAllMember("", node, members);
    }catch(std::exception& e){
        SYLAR_LOG_ERROR << "Config::loadFromFile, " << e.what();
    }

    for(auto& item: members){
        if(item.first.empty())continue;
        ConfigVarBase::ptr_t ptr = Config::lookUpBase(item.first);
        // 只对存在的配置项进行修改
        if(ptr){
            SYLAR_LOG_INFO << "Config::loadFromFile, find key " << item.first; 
            if(item.second.IsScalar()){
                ptr->fromString(item.second.Scalar());
            }else{
                std::stringstream ss;
                ss << item.second;
                SYLAR_LOG_INFO<< "Config::loadFromFile, "<<ss.str();
                ptr->fromString(ss.str());
            }
        }
    }
}


}