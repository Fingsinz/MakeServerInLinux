cmake_minimum_required(VERSION 3.10)    # CMake运行的最小版本
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # 启用编译命令的导出，常与代码分析工具配合使用
set(BUILD_SHARED_LIBS ON)               # 构建共享（动态）库
set(CMAKE_CXX_STANDARD 17)              # 设置C++标准为17
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # 要求编译器支持C++17

# 项目信息
project(Day12           # 项目名称
        LANGUAGES CXX # 项目语言
       )

# 为源代码和测试添加子目录
add_subdirectory(src)
add_subdirectory(test)

# 设置编译和链接选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -std=c++17 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fPIC")
set(GCC_COVERAGE_LINK_FLAGS "-fPIC")

# 显示编译器和链接器标志
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# 设置包含目录
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
include_directories(${SRC_INCLUDE_DIR} ${TEST_INCLUDE_DIR})

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)