add_subdirectory(logger)
add_subdirectory(common)
add_subdirectory(service)

add_library(app INTERFACE)
target_include_directories(app INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app INTERFACE freertos hal common logger service)
