#ifndef UX_GPIO
#define UX_GPIO

#include <gpiod.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/auxv.h>
#include <string.h>
#include <linux/limits.h>
#include <cerrno>
#include "./iodefine.h"
#include "./embux_global_define.h"

struct io {
    gpiod_chip *chip;
    gpiod_line *line;
    bool isInited = false;
};

struct io_raw {
    int chip;
    int line;
    bool isGPIO = false;
};

#ifdef __cplusplus
namespace neoEmbUx {
    int setup(); // C++命名空间版本
    int readPin(int pin);
    int setPin(int pin, int mode, ...);
    bool checkPin(int pin);
    void release();
}
extern "C" { 
#endif
int neoEmbUx_ioSetup(void); // C语言版本
int neoEmbUx_readPin(int pin);
int neoEmbUx_setPin(int pin, int mode, ...);
void neoEmbUx_ioRelease(void);
#ifdef __cplusplus
}                           // 结束 extern "C" 块
#endif
#endif