add_library(service INTERFACE)
target_include_directories(service INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(service INTERFACE freertos common logger)
