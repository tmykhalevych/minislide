set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")

if(HOST)
    set(FREERTOS_HEAP "3" CACHE STRING "")
    set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "")
else()
    set(FREERTOS_HEAP "4" CACHE STRING "")
    set(FREERTOS_PORT "GCC_RP2040" CACHE STRING "")
endif()

freertos_init()

add_library(freertos INTERFACE)
target_link_libraries(freertos INTERFACE freertos_kernel common)
