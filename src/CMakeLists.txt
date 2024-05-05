add_subdirectory(freertos)
add_subdirectory(platform)
add_subdirectory(app)

set(application ${CMAKE_PROJECT_NAME})
set(artifacts_dir ${CMAKE_BINARY_DIR}/out)

add_executable(${application} main.cpp)
target_link_libraries(${application} PRIVATE freertos platform app)

add_platform_post_build_steps(${application} ${artifacts_dir})
