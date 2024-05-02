add_subdirectory(freertos)
add_subdirectory(hal)
add_subdirectory(system)

set(application ${CMAKE_PROJECT_NAME})
set(artifacts ${CMAKE_BINARY_DIR}/out)

add_executable(${application} main.cpp)
target_link_libraries(${application} PRIVATE hal freertos common service)

add_extra_outputs(${application})

add_custom_command(TARGET ${application}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                           ${CMAKE_CURRENT_BINARY_DIR}/${application}.*
                           ${artifacts})
