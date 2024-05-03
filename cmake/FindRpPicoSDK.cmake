include(FetchDependency)

FetchDependency(RpPicoSDK https://github.com/raspberrypi/pico-sdk 1.5.1)

include(${RPPICOSDK_PATH}/pico_sdk_init.cmake)
