set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(HOST_COMPILERS_PATH "/usr/bin" CACHE STRING "Compilers location")

set(CMAKE_C_COMPILER ${HOST_COMPILERS_PATH}/gcc CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER ${HOST_COMPILERS_PATH}/g++ CACHE FILEPATH "C++ compiler")
set(CMAKE_C_OUTPUT_EXTENSION .o)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security")
