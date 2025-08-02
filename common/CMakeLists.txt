add_library(common
        common.cpp
        common.h
)

# 让使用 common 的模块都能自动找到它的头文件
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
#set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/deps")


FetchContent_Declare(
        bs_thread_pool
        GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
        GIT_TAG        v4.1.0
        SOURCE_DIR     ${CMAKE_SOURCE_DIR}/_deps/bs_thread_pool  # 指定源码目录
        BINARY_DIR     ${CMAKE_BINARY_DIR}/_deps/bs_thread_pool-subbuild      # 使用默认的 build 目录结构
)

FetchContent_MakeAvailable(bs_thread_pool)
target_include_directories(common PRIVATE ${bs_thread_pool_SOURCE_DIR}/include)