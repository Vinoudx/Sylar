## 日志
LogEvent: 日志事件类
LogFormatter: 日志格式化类
LogAppender: 日志输出器基类
Logger: 日志类，拥有上述类的实例
LoggerWapper: 帮助在宏中实现析构时输出
AsyncLogger: 双缓冲异步输出器

考虑到一个日志可能会有多个输出地址，所以Logger类中可以含有多个LogAppender

用所给的宏进行日志输出天然线程安全，因为每一个线程都有一个临时的Logger对象，AsyncLogger也是线程安全的

## 配置
配置：约定优于配置，在框架开发中作者约定了一系列配置，用户只需要在配置文件中约定自己需要改变的配置就行了
配置文件采用yaml格式
ConfigVarBase: 配置项基类
ConfigVar: 配置项类，与基类配合实现类型擦除
ConfigCast: 模板类及其偏特化，实现 基础类型, vector, list, set, unordered_set, map, unordered_map 的FromString和ToString功能
Config类: 配置类，提供lookup的两种重载和loadFromFile函数，其中lookup单参数版本在未查找到时返回nullptr，三参数版本在未查找到时根据defaultVal创建项，查找到则根据defaultVal进行覆盖，loadFromFile只会覆盖已存在值不会创建
ConfigVar::addListener: 给对应配置项的变更事件添加回调函数