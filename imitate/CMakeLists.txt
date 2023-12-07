# 确定CMAKE最低版本需求
cmake_minimum_required(VERSION 2.8)

# 确定工程名(不是必须)
project(imitate)

# 确定编译语言
add_definitions(-std=c++11)

# 库文件 
find_package (Threads) 
target_link_libraries (sunnet ${cmake_THREAD_LIBS_INIT})

# 添加需要的头文件
include_directories(include)

# 添加源代码
aux_source_directory(./src SRC)

# 生成可执行文件
add_executable(imitate ${SRC})