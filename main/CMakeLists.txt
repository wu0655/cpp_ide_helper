add_executable(merge merge.cpp)

add_executable(kernel_helper kernel.cpp)
target_link_libraries(kernel_helper PRIVATE common file_parser)

#add_executable(atf_helper atf.cpp)
#target_link_libraries(atf_helper PRIVATE common file_parser)

add_executable(kern_dep_analyzer kern_dep_analyzer.cpp)
target_link_libraries(kern_dep_analyzer PRIVATE common file_parser)

add_executable(check_dep check_dep.cpp)
target_link_libraries(check_dep PRIVATE common file_parser)