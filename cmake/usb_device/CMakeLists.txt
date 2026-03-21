cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(NAME usb_device)

set(DEFINE
)

set(PUBLIC_HEADER
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Inc
)

set(PRIVATE_HEADER
)

set(SOURCE
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
