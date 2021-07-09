cmake_minimum_required(VERSION 3.16.3)

# We always want to cross-compile for ARM
set(CMAKE_TOOLCHAIN_FILE
  ${CMAKE_SOURCE_DIR}/cmake/toolchain.arm-none-eabi-gcc.cmake)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

project(stm32f7-template)

set(PROJECT_FILES
  src/main.c
  src/logging/logging.c)

set(STM32CUBEMX_GENERATED_FILES
  src/stm/startup_stm32f746xx.s

  src/stm/Src/stm32f7xx_hal_msp.c
  src/stm/Src/stm32f7xx_it.c
  src/stm/Src/system_stm32f7xx.c

  src/stm/Inc/main.h
  src/stm/Inc/stm32f7xx_hal_conf.h
  src/stm/Inc/stm32f7xx_it.h)

set(STM32CUBEF7_FILES
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c)

set_source_files_properties(${STM32CUBEF7_FILES}
  PROPERTIES
  COMPILE_FLAGS -Wno-unused-parameter)

set(EXECUTABLE ${PROJECT_NAME}.elf)
add_executable(${EXECUTABLE} ${PROJECT_FILES}
  ${STM32CUBEMX_GENERATED_FILES}
  ${STM32CUBEF7_FILES})


include(${CMAKE_SOURCE_DIR}/cmake/logging.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/logging_config.cmake OPTIONAL)
logging_set_filename(${EXECUTABLE})


target_compile_definitions(${EXECUTABLE} PRIVATE
  -DUSE_HAL_DRIVER
  -DSTM32F746xx)

target_include_directories(${EXECUTABLE} PRIVATE
  src
  src/stm/Inc

  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Inc
  STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
  STM32CubeF7/Drivers/CMSIS/Device/ST/STM32F7xx/Include
  STM32CubeF7/Drivers/CMSIS/Include)

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-sp-d16
  -mfloat-abi=hard
  -fdata-sections
  -ffunction-sections

  -Wall
  -Werror

  $<$<CONFIG:Debug>:-Og>)

target_link_options(${EXECUTABLE} PRIVATE
  -T${CMAKE_SOURCE_DIR}/src/stm/STM32F746NGHx_FLASH.ld

  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-sp-d16
  -mfloat-abi=hard

  -specs=nano.specs
  -specs=nosys.specs
  -lc
  -lm
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections
  -Wl,--print-memory-usage)


add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-size ${EXECUTABLE})

add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
  COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)


set_target_properties(${EXECUTABLE} PROPERTIES ADDITIONAL_CLEAN_FILES
  "${PROJECT_NAME}.bin;${PROJECT_NAME}.hex;${PROJECT_NAME}.map")
