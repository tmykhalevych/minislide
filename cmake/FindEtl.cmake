if (NOT ETL_PATH)
  message(STATUS "ETL_PATH not specified, fetching ETL from github.com/ETLCPP")

  include(FetchContent)
  FetchContent_Declare(Etl
    GIT_REPOSITORY https://github.com/ETLCPP/etl.git
    GIT_TAG 20.38.13
    GIT_PROGRESS TRUE)

  # check if populated already, just to do it once
  FetchContent_GetProperties(Etl)
  if (NOT Etl_POPULATED)
    set(FETCHCONTENT_QUIET FALSE)
    FetchContent_Populate(Etl)
  endif()

  set(ETL_PATH ${etl_SOURCE_DIR})
  message(STATUS "ETL_PATH set to '${ETL_PATH}'")
endif()

add_library(etl INTERFACE)
target_include_directories(etl INTERFACE ${ETL_PATH}/include)
target_compile_definitions(etl INTERFACE ETL_NO_EXCEPTIONS ETL_VERBOSE_ERRORS ETL_CHECK_PUSH_POP)

if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
  target_compile_definitions(etl INTERFACE ETL_NO_SMALL_CHAR_SUPPORT=0)
endif()
