#ifndef EMBUX_IODEFINE
#define EMBUX_IODEFINE

#ifdef EMBUX_BOARD_FORLINX_ELF_2
#include "./board/forlinx_elf_2.h"
#endif

#ifdef EMBUX_BOARD_ORANGEPI_5_PLUS
#include "./board/orangepi_5_plus.h"
#endif


static const int gpio[EMBUX_GPIO_NUM] = {
    #define IO(pin, value)  value,
    EMBUX_GPIO_MAPPING
    #undef IO
};

#ifdef EMBUX_HAS_EXTERNED_PIN
static const int external_gpio[EMBUX_GPIO_NUM] = {
    #define IO(pin, value)  value,
    EXTERN_GPIO_MAPPING
    #undef IO
};
#endif

#ifdef EMBUX_ROCKCHIP

#endif

#endif