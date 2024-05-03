cmake_minimum_required(VERSION 3.18)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(FreeRTOS  REQUIRED)
find_package(RpPicoSDK REQUIRED)
find_package(Etl       REQUIRED)

project(rp2040-study LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(src)
