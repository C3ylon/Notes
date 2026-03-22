# CMAKE

```cmake
# 1. 指定最低 CMake 版本要求
# 建议至少使用 3.15 以上，以支持较新的策略和特性
cmake_minimum_required(VERSION 3.15...3.28)

# 2. 定义项目名称、版本号和使用的语言
project(MyModernProject
    VERSION 1.0.0
    DESCRIPTION "A standard modern CMake template"
    LANGUAGES CXX C # 如果只用 C++，可以去掉 C
)

# 3. 全局 C++ 标准设置
# 现代 C++ 开发通常至少从 C++11 起步，这里以 C++20 为例
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制要求支持该标准，否则报错
set(CMAKE_CXX_EXTENSIONS OFF)       # 禁用编译器特定扩展 (如 gnu++20)，保证跨平台可移植性

# 4. 设置默认的构建类型 (Release / Debug)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# =========================================================================
# 编译器警告级别选项 (Modern Way: 封装为一个 INTERFACE 目标)
# =========================================================================
# 不要使用 add_compile_options()，那会影响全局。
# 我们创建一个虚拟的 INTERFACE 目标，谁链接它，谁就继承这些编译选项。
add_library(project_warnings INTERFACE)

# 针对不同平台的编译器（macOS Clang/GCC vs Windows MSVC）设置不同的警告参数
if(MSVC)
    target_compile_options(project_warnings INTERFACE
        /W4     # 开启高级别警告
        /WX     # 将警告视为错误 (视需求决定是否开启)
        /utf-8  # 指定源代码和执行字符集为 UTF-8
    )
else()
    # 适用于 GCC 和 Clang (macOS/Linux/MinGW)
    target_compile_options(project_warnings INTERFACE
        -Wall -Wextra -Wpedantic
        -Werror # 将警告视为错误 (视需求决定是否开启)
    )
endif()

# =========================================================================
# 构建一个库 (Library)
# =========================================================================
# 假设我们有一个核心功能库，位于 src/core_lib.cpp 和 include/core_lib.h
# STATIC 表示静态库，SHARED 表示动态库。如果不写，可以在命令行通过 BUILD_SHARED_LIBS 控制
add_library(CoreLib STATIC
    src/core_lib.cpp
)

# 为 Target 指定头文件搜索路径 (关键：作用域控制)
# PUBLIC: CoreLib 自己编译时需要这个路径，链接 CoreLib 的其他目标也需要。
# PRIVATE: 只有 CoreLib 自己编译时需要。
# INTERFACE: CoreLib 自己不需要，但链接它的目标需要。
target_include_directories(CoreLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include> # 安装后的路径，用于打包发布
)

# 让库继承之前定义的编译器警告选项
target_link_libraries(CoreLib PRIVATE project_warnings)

# =========================================================================
# 跨平台/系统特定设置示例
# =========================================================================
# 如果需要在 macOS 或 Windows 执行特定操作
if(APPLE)
    # macOS 特定设置，例如处理框架
    # target_link_libraries(CoreLib PRIVATE "-framework Foundation")
elseif(WIN32)
    # Windows 特定设置，例如定义宏
    target_compile_definitions(CoreLib PRIVATE WIN32_LEAN_AND_MEAN)
endif()

# =========================================================================
# 构建可执行文件 (Executable)
# =========================================================================
add_executable(MyApplication
    src/main.cpp
)

# 链接刚才构建的库到可执行文件
target_link_libraries(MyApplication PRIVATE CoreLib)

# 继承警告选项
target_link_libraries(MyApplication PRIVATE project_warnings)

# =========================================================================
# 安装规则 (Installation) - 可选，用于 make install
# =========================================================================
include(GNUInstallDirs) # 提供标准安装目录变量如 CMAKE_INSTALL_BINDIR

install(TARGETS MyApplication CoreLib
    EXPORT MyProjectTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # 可执行文件及 Windows DLL 安装位置
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # 动态库 (.so, .dylib) 安装位置
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # 静态库 (.a, .lib) 安装位置
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# =========================================================================
# 测试 (Testing) - 可选，集成 CTest
# =========================================================================
# 开启测试功能
enable_testing()

# 假设添加了一个简单的测试脚本或可执行程序
# add_executable(TestCoreLib tests/test_core.cpp)
# target_link_libraries(TestCoreLib PRIVATE CoreLib)
# add_test(NAME CoreLibBasicTest COMMAND TestCoreLib)
```
