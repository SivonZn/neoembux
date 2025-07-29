#include "./common.h"
#ifndef EMBUX_ORANGEPI_5_PLUS
#define EMBUX_ORANGEPI_5_PLUS

#define EMBUX_ROCKCHIP

#define EMBUX_CHIP_NUM 5

#define EMBUX_GPIO_NUM 40
#define EMBUX_GPIO_MAPPING \
    IO(1,  EMBUX_VCC_3V3)  \
    IO(2,  EMBUX_VCC_5V)   \
    IO(3,  EMBUX_UNKNOWN)\
    IO(4,  EMBUX_VCC_5V)   \
    IO(5,  EMBUX_UNKNOWN)\
    IO(6,  EMBUX_GND)      \
    IO(7,  EMBUX_UNKNOWN)      \
    IO(8,  EMBUX_UNKNOWN)\
    IO(9,  EMBUX_GND)      \
    IO(10, EMBUX_UNKNOWN)\
    IO(11, 36)      \
    IO(12, EMBUX_UNKNOWN)      \
    IO(13, 39)      \
    IO(14, EMBUX_GND)      \
    IO(15, 40)       \
    IO(16, 45)      \
    IO(17, EMBUX_VCC_3V3)  \
    IO(18, 46)      \
    IO(19, EMBUX_UNKNOWN)\
    IO(20, EMBUX_GND)      \
    IO(21, EMBUX_UNKNOWN)\
    IO(22, 34)      \
    IO(23, EMBUX_UNKNOWN)\
    IO(24, EMBUX_UNKNOWN)\
    IO(25, EMBUX_GND)      \
    IO(26, EMBUX_UNKNOWN)      \
    IO(27, EMBUX_UNKNOWN)\
    IO(28, EMBUX_UNKNOWN)\
    IO(29, 63)      \
    IO(30, EMBUX_GND)      \
    IO(31, 96)      \
    IO(32, EMBUX_UNKNOWN)       \
    IO(33, EMBUX_UNKNOWN)      \
    IO(34, EMBUX_GND)      \
    IO(35, EMBUX_UNKNOWN)       \
    IO(36, 101)      \
    IO(37, 99)       \
    IO(38, EMBUX_UNKNOWN)      \
    IO(39, EMBUX_GND)      \
    IO(40, EMBUX_UNKNOWN)

#endif