if (NOT RP_PICO_SDK_PATH)
  message(STATUS "RP_PICO_SDK_PATH not specified, fetching RP Pico SDK from github.com/raspberrypi/pico-sdk")

  include(FetchContent)
  FetchContent_Declare(RpPicoSDK
    GIT_REPOSITORY https://github.com/raspberrypi/pico-sdk
    GIT_TAG 1.5.1
    GIT_PROGRESS TRUE)

  # check if populated already, just to do it once
  FetchContent_GetProperties(RpPicoSDK)
  if (NOT RpPicoSDK_POPULATED)
    set(FETCHCONTENT_QUIET FALSE)
    FetchContent_Populate(RpPicoSDK)
  endif()

  set(RP_PICO_SDK_PATH ${rppicosdk_SOURCE_DIR})
  message(STATUS "RP_PICO_SDK_PATH set to '${RP_PICO_SDK_PATH}'")
endif()

include(${RP_PICO_SDK_PATH}/pico_sdk_init.cmake)
