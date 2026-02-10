cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(NAME bsp)

set(DEFINE
)

set(PUBLIC_HEADER
    ${CMAKE_SOURCE_DIR}/BSP/Inc
)

set(PRIVATE_HEADER
)

set(SOURCE
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
    # ${CMAKE_SOURCE_DIR}/BSP/sd.c
)

set(PUBLIB_LIB_DIRS
)

set(PUBLIB_LIBS
)

set(PRIVATE_LIB_DIRS
    ${LIB_DIRS}/stm32cubemx
)

set(PRIVATE_LIBS
    "drivers"
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
