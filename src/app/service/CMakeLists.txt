add_library(service STATIC event_loop.cpp)
target_include_directories(service PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(service PRIVATE freertos common logger)
