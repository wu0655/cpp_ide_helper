cmake_minimum_required(VERSION 3.20)
project(cpp_ide_helper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_C_COMPILER gcc)
#set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS " -static ")
set(CMAKE_EXE_LINKER_FLAGS " -static ")



#set_target_properties(kernel_helper PROPERTIES LINK_FLAGS "-static")

# 添加子模块
add_subdirectory(common)
add_subdirectory(file_parser)
add_subdirectory(main)
