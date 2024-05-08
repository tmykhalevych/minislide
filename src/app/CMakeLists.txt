add_subdirectory(logger)
add_subdirectory(common)
add_subdirectory(service)
add_subdirectory(led)

add_library(app STATIC app.cpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app
    PRIVATE
        freertos
        platform
        common
        logger
        service
        led
)
