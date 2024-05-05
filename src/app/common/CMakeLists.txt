add_library(etl INTERFACE)
target_include_directories(etl INTERFACE $ENV{ETL_PATH}/include)
target_compile_definitions(etl INTERFACE ETL_NO_EXCEPTIONS ETL_VERBOSE_ERRORS ETL_CHECK_PUSH_POP)

if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
    target_compile_definitions(etl INTERFACE ETL_NO_SMALL_CHAR_SUPPORT=0)
endif()

add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common INTERFACE freertos etl)
