cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(NAME fatfs)

set(DEFINE
)

set(PUBLIC_HEADER
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt
)

set(PRIVATE_HEADER
)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/diskio.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/ff.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/option/syscall.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/option/unicode.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/bsp_driver_sd.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/fatfs.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/sd_diskio.c
)

set(PUBLIB_LIB_DIRS
)

set(PUBLIB_LIBS
)

set(PRIVATE_LIB_DIRS
    ${LIB_DIRS}/stm32cubemx
    ${LIB_DIRS}/freertos
)

set(PRIVATE_LIBS
    "drivers"
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
