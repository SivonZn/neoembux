#ifndef EMBUX_IODEFINE
#define EMBUX_IODEFINE

#ifdef EMBUX_BOARD_ELF2
#define EMBUX_ROCKCHIP
#include "./iodefine_elf2.h"
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