include(FetchDependency)

FetchDependency(Etl https://github.com/ETLCPP/etl.git 20.38.13)

function(etl_init)
  add_library(etl INTERFACE)
  target_include_directories(etl INTERFACE ${ETL_PATH}/include)
  target_compile_definitions(etl INTERFACE ETL_NO_EXCEPTIONS ETL_VERBOSE_ERRORS ETL_CHECK_PUSH_POP)

  if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
    target_compile_definitions(etl INTERFACE ETL_NO_SMALL_CHAR_SUPPORT=0)
  endif()
endfunction()
