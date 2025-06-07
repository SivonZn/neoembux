#ifndef EMBUX_IODEFINE_ELF2
#define EMBUX_IODEFINE_ELF2

#define EMBUX_ROCKCHIP

#define EMBUX_GND 10000
#define EMBUX_VCC_3V3 10001
#define EMBUX_VCC_5V 10002
//3.3V = 1, 5 = 2, EMBUX_GND = 0
#define EMBUX_UNKNOWN 10003

#define EMBUX_CHIP_NUM 5

#define EMBUX_GPIO_NUM 40
#define EMBUX_GPIO_MAPPING \
    IO(1,  EMBUX_VCC_3V3)  \
    IO(2,  EMBUX_VCC_5V)   \
    IO(3,  EMBUX_UNKNOWN)\
    IO(4,  EMBUX_VCC_5V)   \
    IO(5,  EMBUX_UNKNOWN)\
    IO(6,  EMBUX_GND)      \
    IO(7,  107)      \
    IO(8,  EMBUX_UNKNOWN)\
    IO(9,  EMBUX_GND)      \
    IO(10, EMBUX_UNKNOWN)\
    IO(11, 109)      \
    IO(12, 104)      \
    IO(13, 108)      \
    IO(14, EMBUX_GND)      \
    IO(15, 98)       \
    IO(16, 115)      \
    IO(17, EMBUX_VCC_3V3)  \
    IO(18, 123)      \
    IO(19, EMBUX_UNKNOWN)\
    IO(20, EMBUX_GND)      \
    IO(21, EMBUX_UNKNOWN)\
    IO(22, 114)      \
    IO(23, EMBUX_UNKNOWN)\
    IO(24, EMBUX_UNKNOWN)\
    IO(25, EMBUX_GND)      \
    IO(26, 122)      \
    IO(27, EMBUX_UNKNOWN)\
    IO(28, EMBUX_UNKNOWN)\
    IO(29, 110)      \
    IO(30, EMBUX_GND)      \
    IO(31, 102)      \
    IO(32, 97)       \
    IO(33, 100)      \
    IO(34, EMBUX_GND)      \
    IO(35, 96)       \
    IO(36, 101)      \
    IO(37, 99)       \
    IO(38, 105)      \
    IO(39, EMBUX_GND)      \
    IO(40, 103)

// Define if Board Has Extrenal GPIO
#define EMBUX_HAS_EXTERNED_PIN

#define EMBUX_EXTERN_GPIO_NUM 20
#define EXTERN_GPIO_MAPPING \
    IO(1,  EMBUX_VCC_3V3)  \
    IO(2,  EMBUX_VCC_5V)   \
    IO(3,  EMBUX_GND)      \
    IO(4,  EMBUX_GND)      \
    IO(5,  EMBUX_UNKNOWN)\
    IO(6,  EMBUX_UNKNOWN)\
    IO(7,  EMBUX_UNKNOWN)\
    IO(8,  EMBUX_UNKNOWN)\
    IO(9,  EMBUX_GND)      \
    IO(10, EMBUX_GND)      \
    IO(11, 139)      \
    IO(12, 138)      \
    IO(13, EMBUX_GND)      \
    IO(14, EMBUX_GND)      \
    IO(15, 116)      \
    IO(16, 117)      \
    IO(17, EMBUX_GND)      \
    IO(18, EMBUX_GND)      \
    IO(19, EMBUX_UNKNOWN)\
    IO(20, EMBUX_UNKNOWN)

#endif

