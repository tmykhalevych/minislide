set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

add_library(freertos INTERFACE)
target_include_directories(freertos INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(freertos INTERFACE common)

if (CMAKE_BUILD_TYPE STREQUAL "Host")
    set(FREERTOS_HEAP "3" CACHE STRING "")
    set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "")

    add_subdirectory($ENV{FREERTOS_PATH} freertos)
    target_link_libraries(freertos INTERFACE freertos_kernel)
else()
    add_subdirectory($ENV{FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040 freertos)
    target_link_libraries(freertos INTERFACE FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)
endif()
