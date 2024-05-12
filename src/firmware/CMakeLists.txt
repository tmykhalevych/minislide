add_subdirectory(logger)
add_subdirectory(common)
add_subdirectory(service)
add_subdirectory(led)

add_library(firmware STATIC firmware.cpp)
target_include_directories(firmware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(firmware
    PRIVATE
        freertos
        bsp
        common
        logger
        service
        led
)
