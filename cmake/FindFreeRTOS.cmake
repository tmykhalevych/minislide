include(FetchDependency)

FetchDependency(FreeRTOS https://github.com/FreeRTOS/FreeRTOS.git 202212.01)

function(freertos_init)
  add_subdirectory(${FREERTOS_PATH}/FreeRTOS/Source freertos)
endfunction()
