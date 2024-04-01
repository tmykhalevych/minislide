set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")
set(FREERTOS_HEAP "4" CACHE STRING "")
set(FREERTOS_PORT "GCC_RP2040" CACHE STRING "")

pico_sdk_init()
freertos_init()

set(application ${CMAKE_PROJECT_NAME})
set(artifacts ${CMAKE_BINARY_DIR}/out)

add_executable(${application} main.cpp)
target_link_libraries(${application} PRIVATE pico_stdlib freertos_kernel)

pico_add_extra_outputs(${application})

add_custom_command(TARGET ${application}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                           ${CMAKE_CURRENT_BINARY_DIR}/${application}.*
                           ${artifacts})
