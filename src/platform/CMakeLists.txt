add_library(platform STATIC)
set(PLATFOM_IMPL "")

if (CMAKE_BUILD_TYPE STREQUAL "Host")
    include(host/toolchain.cmake)
    set(PLATFOM_IMPL host)
else()
    set(PLATFOM_IMPL pico)
    set(PICO_BOARD pico_w)
    set(PICO_DEFAULT_FLOAT_IMPL pico_float_default)
    set(PICO_DEFAULT_DOUBLE_IMPL pico_double_default)

    pico_sdk_init()

    pico_find_compiler(PICO_SIZE ${PICO_GCC_TRIPLE}-size)
    set(CMAKE_SIZE_UTIL ${PICO_SIZE} CACHE FILEPATH "")

    target_link_libraries(platform PUBLIC pico_float PRIVATE pico_stdlib pico_cyw43_arch_none)
endif()

target_include_directories(platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PLATFORM_SRCS ${PLATFOM_IMPL}/*.cpp)
target_sources(platform PRIVATE ${PLATFORM_SRCS})

function(add_platform_post_build_steps target artifacts_dir)
    add_custom_command(
        TARGET ${application}
        POST_BUILD
        COMMAND mkdir -p ${artifacts_dir} && cp ${CMAKE_CURRENT_BINARY_DIR}/${application}.* ${artifacts_dir}
        COMMENT "Copying artifacts into '${artifacts_dir}'")

    if (CMAKE_BUILD_TYPE STREQUAL "Host")
        # TODO: add HOST post build steps
    else()
        pico_add_extra_outputs(${target})

        add_custom_command(
            TARGET ${application}
            POST_BUILD
            COMMAND ${CMAKE_SIZE_UTIL} --format=SysV ${artifacts_dir}/${application}.elf
            COMMENT "Dumping '${application}.elf' segments sizes")
    endif()
endfunction()
