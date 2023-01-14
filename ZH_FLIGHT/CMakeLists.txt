set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)

cmake_minimum_required(VERSION 3.1.0)

project(ZH_Flight_H7 C CXX ASM)


set(target "${PROJECT_NAME}")
set(COMPILE_TOOLS GCC)
# set(CMAKE_MAKE_PROGRAM D:/App/mingw/mingw64/bin/make.exe)
set(CROSS_COMPILE_PREFIX arm-none-eabi)
include(target-def.cmake)

set(CORE_SRC
Core/Src/main.c
Core/Src/sys_callback.c
Core/Src/gpio.c
Core/Src/flight_task.c
Core/Src/stm32h7xx_it.c
Core/Src/stm32h7xx_hal_msp.c
Core/Src/system_stm32h7xx.c
Core/Src/freertos.c
Core/Src/stm32h7xx_hal_timebase_tim.c
Core/Src/spi.c
Core/Src/i2c.c
Core/Src/dma.c
Core/Src/octospi.c
Core/Src/tim.c
Core/Src/usart.c
)

set(DRIVERS_SRC
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ospi.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
)

set(MIDDLEWARES_SRC
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
Middlewares/Third_Party/FreeRTOS/Source/croutine.c
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
Middlewares/Third_Party/FreeRTOS/Source/list.c
Middlewares/Third_Party/FreeRTOS/Source/queue.c
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
Middlewares/Third_Party/FreeRTOS/Source/tasks.c
Middlewares/Third_Party/FreeRTOS/Source/timers.c
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
)

set(USB_DEVICE_SRC
USB_DEVICE/App/usb_device.c
USB_DEVICE/App/usbd_desc.c
USB_DEVICE/App/usbd_cdc_if.c
USB_DEVICE/Target/usbd_conf.c
)

set(SYS_SRC
# Sys/Test/test.cpp
Sys/Aircraft/aircraft.cpp
Sys/AircraftState/aircraft_state.cpp
Sys/Control/AttitudeControl/attitude_controller.cpp
Sys/Control/AttitudeControl/attitude_controller_interface.cpp
Sys/Control/AttitudeControl/pid_attitude_controller.cpp
Sys/Control/pid_core.cpp
Sys/EventServer/event_server.cpp
Sys/Sensor/sensor.cpp
Sys/Sensor/Imu/icm20602.cpp
Sys/Sensor/Imu/icm20689.cpp
Sys/Sensor/Baro/ms5611.cpp
Sys/StateMachine/state_machine.cpp
Sys/Driver/z_spi.cpp
Sys/Driver/z_iic.cpp
Sys/Driver/z_uart.cpp
Sys/Protocol/dshot.cpp
Sys/Protocol/ibus.cpp
Sys/Driver/z_pwm.cpp
Sys/Motor/motor.cpp
Sys/MessageServer/message_server.cpp
Sys/AHRS/mahony.cpp
Sys/Task/aircraft_task.cpp
)

set(START_UP_ASM
startup_stm32h7b0xx.s
)


set(SRC
${CORE_SRC}
${DRIVERS_SRC}
${MIDDLEWARES_SRC}
${USB_DEVICE_SRC}
${SYS_SRC}
${START_UP_ASM}
)

include_directories(
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Aircraft>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/AircraftState>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/EventServer>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Control>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Control/AttitudeControl>
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Test>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Task>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/AHRS>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Sensor>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Sensor/Imu>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Sensor/Baro>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/OS>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Action>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Config>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Math>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/ExternalLib>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Driver>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Type>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Control>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Control/AttitudeControl>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Filter>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/AHRS>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/AircraftState>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Motor>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/MessageServer>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Remote>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/Protocol>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sys/StateMachine>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS>
)

add_executable(${target}.elf ${SRC})

set(ELF_FILE ${PROJECT_BINARY_DIR}/${target}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${target}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${target}.bin)

add_custom_command(TARGET "${target}.elf" POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${ELF_FILE} ${HEX_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin"
    COMMAND ${CMAKE_COMMAND} -E copy ${HEX_FILE} "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex"
    COMMAND ${CMAKE_SIZE} --format=berkeley ${target}.hex ${target}.elf
)
