cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(NAME bsp)

set(DEFINE
)

set(PUBLIC_BSP_HEADER
    ${CMAKE_SOURCE_DIR}/BSP/Inc
)

set(PUBLIC_FREERTOS_HEADER
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

set(PUBLIC_FATFS_HEADER
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt
)

set(PUBLIC_FMC_HEADER
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD
)

set(PUBLIC_DUSB_HEADER
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Inc
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Inc
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Inc
)

set(PUBLIC_ETH_HEADER
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/adapt
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/system
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/netif/ppp
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/lwip
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/lwip/apps
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/lwip/priv
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/lwip/prot
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/netif
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/compat/posix
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/compat/posix/net
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/compat/posix/sys
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/include/compat/stdc
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/system/arch
)

set(PUBLIC_HEADER
    # BSP headers
    ${PUBLIC_BSP_HEADER}
    # FreeRTOS headers
    ${PUBLIC_FREERTOS_HEADER}
    # FatFS headers
    ${PUBLIC_FATFS_HEADER}
    # FMC headers
    ${PUBLIC_FMC_HEADER}
    # USB Device headers
    ${PUBLIC_DUSB_HEADER}
    # Ethernet headers
    ${PUBLIC_ETH_HEADER}
)

set(PRIVATE_HEADER
)

set(SOURCE_BSP
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
)

set(SOURCE_FREERTOS
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

set(SOURCE_FATFS
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/diskio.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/ff.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/option/syscall.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/src/option/unicode.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/bsp_driver_sd.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/fatfs.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FatFs/adapt/sd_diskio.c
)

set(SOURCE_FMC
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/fmc.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_dma.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_dma2d.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FMC/LCD/lcd_mdma.c
)

set(SOURCE_DUSB
    # CDC
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usb_device.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_cdc_if.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_cdc.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_conf.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/CDC/Src/usbd_desc.c
    # HID
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usb_device.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usbd_hid.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usbd_conf.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/HID/Src/usbd_desc.c
    # DFU
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usb_device.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_dfu_if.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_conf.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_desc.c
    # ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/DFU/Src/usbd_dfu.c
    # MSC
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usb_device.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usbd_conf.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usbd_desc.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usbd_msc_bot.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usbd_msc_data.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usbd_msc_scsi.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usbd_msc.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Class/MSC/Src/usbd_storage_if.c
    # Core
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Src/usbd_core.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Src/usbd_ctlreq.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/USB/Device/Core/Src/usbd_ioreq.c
)

set(SOURCE_ETH
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/auth.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/ccp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/chap_ms.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/chap-md5.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/chap-new.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/demand.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/eap.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/eui64.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/fsm.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/ipcp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/ipv6cp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/lcp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/magic.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/mppe.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/multilink.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/ppp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/pppapi.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/pppcrypt.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/pppoe.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/pppol2tp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/pppos.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/upap.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/utils.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/vj.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/bridgeif.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/bridgeif_fdb.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ethernet.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/lowpan6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/lowpan6_ble.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/lowpan6_common.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/slipif.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/zepif.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/netif/ppp/ecp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/api_lib.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/api_msg.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/err.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/if_api.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/netbuf.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/netdb.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/netifapi.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/sockets.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/api/tcpip.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/altcp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/altcp_alloc.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/altcp_tcp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/def.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/dns.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/inet_chksum.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/init.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ip.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/mem.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/memp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/netif.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/pbuf.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/raw.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/stats.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/sys.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/tcp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/tcp_in.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/tcp_out.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/timeouts.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/udp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/acd.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/system/OS/sys_arch.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/adapt/lwip.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/adapt/lan8742.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LwIP/adapt/ethernetif.c
)

set(SOURCE
    # BSP sources
    ${SOURCE_BSP}
    # FreeRTOS sources
    ${SOURCE_FREERTOS}
    # FatFS sources
    ${SOURCE_FATFS}
    # FMC sources
    ${SOURCE_FMC}
    # USB Device sources
    ${SOURCE_DUSB}
    # Ethernet sources
    ${SOURCE_ETH}
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
