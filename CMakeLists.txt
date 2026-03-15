cmake_minimum_required(VERSION 3.22)

project(stm32h743vit6)

enable_language(C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(NAME stm32h743vit6)

set(LIB_DIRS ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/cmake/)

set(DEFINE
)

set(PUBLIC_HEADER
    ${CMAKE_SOURCE_DIR}/Inc
)

set(PRIVATE_HEADER
)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/startup_stm32h743xx.s
    ${CMAKE_SOURCE_DIR}/Src/main.c
    ${CMAKE_SOURCE_DIR}/Src/stm32h7xx_hal_msp.c
    ${CMAKE_SOURCE_DIR}/Src/stm32h7xx_it.c
    ${CMAKE_SOURCE_DIR}/Src/syscalls.c
    ${CMAKE_SOURCE_DIR}/Src/sysmem.c
    ${CMAKE_SOURCE_DIR}/Src/system_stm32h7xx.c
)

set(PUBLIB_LIB_DIRS
    ${LIB_DIRS}/stm32cubemx
    ${LIB_DIRS}/bsp
    ${LIB_DIRS}/freertos
    ${LIB_DIRS}/fatfs
    ${LIB_DIRS}/fmc
)

set(PUBLIB_LIBS
    "drivers"
    "bsp"
    "freertos"
    "fatfs"
    "fmc"
    "usb_device"
)

set(PRIVATE_LIB_DIRS
)

set(PRIVATE_LIBS
)

add_subdirectory(cmake/stm32cubemx)
add_subdirectory(cmake/bsp)
add_subdirectory(cmake/freertos)
add_subdirectory(cmake/fatfs)
add_subdirectory(cmake/fmc)
add_subdirectory(cmake/usb_device)

add_executable(${NAME} ${SOURCE})

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

# Ensure the toolchain defines objdump
if(NOT CMAKE_OBJDUMP)
    find_program(CMAKE_OBJDUMP NAMES ${TOOLCHAIN_PREFIX}objdump REQUIRED)
endif()

# Ensure the toolchain defines a `size` utility used for the memory report
if(NOT CMAKE_SIZE_UTIL)
    find_program(CMAKE_SIZE_UTIL
        NAMES ${TOOLCHAIN_PREFIX}size arm-none-eabi-size size
        REQUIRED)
endif()

# Post-build commands to generate additional output formats
add_custom_command(
    TARGET ${NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${NAME}> ${CMAKE_BINARY_DIR}/${NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${NAME}> ${CMAKE_BINARY_DIR}/${NAME}.bin
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${NAME}> > ${CMAKE_BINARY_DIR}/${NAME}.lst
    COMMAND ${CMAKE_OBJDUMP} -S $<TARGET_FILE:${NAME}> > ${CMAKE_BINARY_DIR}/${NAME}.asm
    COMMENT "Generating HEX, BIN, LST and ASM files"
)

# Post-build command to generate memory usage report
add_custom_command(
    TARGET ${NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "=== memory usage report ===" > ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "memory usage:" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_SIZE_UTIL} $<TARGET_FILE:${NAME}> >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "=== memory segments info ===" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND arm-none-eabi-objdump -h $<TARGET_FILE:${NAME}> >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "=== linker script defined symbols ===" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND arm-none-eabi-nm -n $<TARGET_FILE:${NAME}> | findstr "_Min_Heap_Size" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND arm-none-eabi-nm -n $<TARGET_FILE:${NAME}> | findstr "_Min_Stack_Size" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND arm-none-eabi-nm -n $<TARGET_FILE:${NAME}> | findstr "_estack" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND arm-none-eabi-nm -n $<TARGET_FILE:${NAME}> | findstr "_sdata" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND arm-none-eabi-nm -n $<TARGET_FILE:${NAME}> | findstr "_edata" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND ${CMAKE_COMMAND} -E echo "=== all symbols info ===" >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMAND arm-none-eabi-nm -n $<TARGET_FILE:${NAME}> >> ${CMAKE_BINARY_DIR}/memory_report.txt
    COMMENT "Generating memory usage report to memory_report.txt"
)