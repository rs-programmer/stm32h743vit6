cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(NAME freertos)

set(DEFINE
)

set(PUBLIC_HEADER
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

set(PRIVATE_HEADER
)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
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
