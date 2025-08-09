include(FetchContent)
include(ExternalProject)

# 本地 tar.gz 包路径（你要提前准备好这个文件）
set(LOCAL_BS_THREAD_POOL_TGZ "${CMAKE_SOURCE_DIR}/third_party/bs_thread_pool.tar.gz")
set(LOCAL_BS_THREAD_POOL_DIR "${CMAKE_SOURCE_DIR}/_deps/bs_thread_pool")

# 简单检测网络（ping github.com，Linux/macOS用 -c 1）
execute_process(
        COMMAND ping -c 1 github.com
        RESULT_VARIABLE PING_RESULT
        OUTPUT_QUIET ERROR_QUIET
)

if(PING_RESULT EQUAL 0)
    message(STATUS "Network OK, fetch bs_thread_pool from GitHub")
    FetchContent_Declare(
            bs_thread_pool
            GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
            GIT_TAG v4.1.0
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/_deps/bs_thread_pool
            BINARY_DIR ${CMAKE_BINARY_DIR}/_deps/bs_thread_pool-subbuild
    )
    FetchContent_MakeAvailable(bs_thread_pool)
    set(BS_THREAD_POOL_INCLUDE_DIR ${bs_thread_pool_SOURCE_DIR}/include)
else()
    message(WARNING "Network unreachable, use local bs_thread_pool")
    # 用 tar 解压本地 tar.gz 包
    if(NOT EXISTS ${LOCAL_BS_THREAD_POOL_DIR})
        file(MAKE_DIRECTORY ${LOCAL_BS_THREAD_POOL_DIR})
        execute_process(
                COMMAND tar -xvf ${LOCAL_BS_THREAD_POOL_TGZ} -C ${LOCAL_BS_THREAD_POOL_DIR}
                RESULT_VARIABLE TAR_RESULT
        )
        if(NOT TAR_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to untar local bs_thread_pool from ${LOCAL_BS_THREAD_POOL_TGZ}")
        endif()
    endif()

    # 本地解压后的 include 目录（根据你压缩包里结构调整）
    set(BS_THREAD_POOL_INCLUDE_DIR ${LOCAL_BS_THREAD_POOL_DIR}/include)
endif()

add_library(common
        common.cpp
        common.h
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(common PRIVATE ${BS_THREAD_POOL_INCLUDE_DIR})