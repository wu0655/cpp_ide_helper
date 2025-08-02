add_library(file_parser
        cmd_kernel.cpp
)

target_include_directories(file_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(file_parser PUBLIC common)