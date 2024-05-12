add_subdirectory(bsp)
add_subdirectory(freertos)
add_subdirectory(firmware)

set(application ${CMAKE_PROJECT_NAME})
set(artifacts_dir ${CMAKE_BINARY_DIR}/out)

add_executable(${application} main.cpp)
target_link_libraries(${application} PRIVATE freertos firmware)

add_bsp_post_build_steps(${application} ${artifacts_dir})
