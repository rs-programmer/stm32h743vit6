cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(NAME fmc)

set(DEFINE
)

set(PUBLIC_HEADER
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD
)

set(PRIVATE_HEADER
)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/fmc.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_dma.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_dma2d.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_mdma.c
)

set(PUBLIB_LIB_DIRS
)

set(PUBLIB_LIBS
)

set(PRIVATE_LIB_DIRS
    ${LIB_DIRS}/stm32cubemx
    ${LIB_DIRS}/bsp
    ${LIB_DIRS}/freertos
)

set(PRIVATE_LIBS
    "drivers"
    "bsp"
    "freertos"
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
