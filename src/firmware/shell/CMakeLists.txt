add_library(shell STATIC shell.cpp)
target_include_directories(shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shell PRIVATE bsp common logger service)
