# 递归搜索/src目录中的所有.cpp文件
file(GLOB_RECURSE day12_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fPIC -pthread")

# 使用找到的源文件创建一个名为day12_shared的共享库
add_library(day12_shared SHARED ${day12_sources})
