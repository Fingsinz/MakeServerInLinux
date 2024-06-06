# 设置包含目录
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
include_directories(${SRC_INCLUDE_DIR})

# 设置变量TEST_SOURCES，将所有.cpp文件存储在测试目录中
file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

# 创建名为“build-tests”的自定义目标以仅显示测试
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
# 创建名为“check-tests”的自定义目标以在详细模式下运行测试
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

# 遍历TEST_SOURCES中的每个测试源文件
foreach (test_source ${TEST_SOURCES})
    # 组合为可读的名称，使得每个Cpp都可以make
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})

    # 为测试添加可执行目标，默认情况下将其从所有生成中排除
    add_executable(${test_name} EXCLUDE_FROM_ALL ${test_source})
    # 添加对生成测试和检查测试的依赖项
    add_dependencies(build-tests ${test_name})
    add_dependencies(check-tests ${test_name})

    # 将“network_shared”库链接到可执行测试文件
    target_link_libraries(${test_name} network_shared)

    # 设置测试目标的属性，指定输出目录和运行测试的命令
    set_target_properties(${test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        COMMAND ${test_name}
    )
endforeach(test_source ${TEST_SOURCES})