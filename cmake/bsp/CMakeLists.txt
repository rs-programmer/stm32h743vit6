cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(NAME bsp)

set(DEFINE
)

set(PUBLIC_HEADER
    # BSP headers
    ${CMAKE_SOURCE_DIR}/BSP/Inc
    # FreeRTOS headers
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    # FatFS headers
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt
    # FMC headers
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD
    # USB Device headers
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Inc
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Inc
)

set(PRIVATE_HEADER
)

set(SOURCE
    # BSP sources
    ${CMAKE_SOURCE_DIR}/BSP/delay.c
    ${CMAKE_SOURCE_DIR}/BSP/exti.c
    ${CMAKE_SOURCE_DIR}/BSP/gpio.c
    ${CMAKE_SOURCE_DIR}/BSP/iic.c
    ${CMAKE_SOURCE_DIR}/BSP/rng.c
    ${CMAKE_SOURCE_DIR}/BSP/rtc.c
    ${CMAKE_SOURCE_DIR}/BSP/spi.c
    ${CMAKE_SOURCE_DIR}/BSP/systick.c
    ${CMAKE_SOURCE_DIR}/BSP/uart.c
    ${CMAKE_SOURCE_DIR}/BSP/watchdog.c
    ${CMAKE_SOURCE_DIR}/BSP/tim.c
    ${CMAKE_SOURCE_DIR}/BSP/adc.c
    ${CMAKE_SOURCE_DIR}/BSP/sd.c
    ${CMAKE_SOURCE_DIR}/BSP/flash.c
    # FreeRTOS sources
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_5_user.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    # FatFS sources
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/diskio.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/ff.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/option/syscall.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/option/unicode.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/bsp_driver_sd.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/fatfs.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/sd_diskio.c
    # FMC sources
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/fmc.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_dma.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_dma2d.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_mdma.c
    # USB Device sources
    # CDC
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usb_device.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_cdc_if.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_cdc.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_conf.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_desc.c
    # HID
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usb_device.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usbd_hid.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usbd_conf.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usbd_desc.c
    # DFU
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usb_device.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_dfu_if.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_conf.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_desc.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_dfu.c
    # Core
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Src/usbd_core.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Src/usbd_ctlreq.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Src/usbd_ioreq.c
)

set(PUBLIB_LIB_DIRS
)

set(PUBLIB_LIBS
)

set(PRIVATE_LIB_DIRS
    ${LIB_DIRS}/stm32cubemx
)

set(PRIVATE_LIBS
    "stm32cubemx"
)

add_library(${NAME} STATIC ${SOURCE})

target_include_directories(${NAME}
    PUBLIC ${PUBLIC_HEADER}
    PRIVATE ${PRIVATE_HEADER}
)

target_link_directories(${NAME}
    PUBLIC ${PUBLIB_LIB_DIRS}
    PRIVATE ${PRIVATE_LIB_DIRS}
)

target_link_libraries(${NAME}
    PUBLIC ${PUBLIB_LIBS}
    PRIVATE ${PRIVATE_LIBS}
)

target_compile_definitions(${NAME}
    PUBLIC ${DEFINE}
)
