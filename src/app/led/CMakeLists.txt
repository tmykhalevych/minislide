add_library(led INTERFACE)
target_include_directories(led INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(led INTERFACE hal common logger service)
