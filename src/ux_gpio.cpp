#include "../include/neoembux.h"
#include "../include/neoembux/ux_gpio.h"

static char neoembux_exe_name[PATH_MAX] = "NeoEmbUX";

static int initialized = 0;

struct io_raw io_raw[EMBUX_GPIO_NUM];
struct io io[EMBUX_GPIO_NUM];

static int initPin(int acc_pin);
static int initPinMap();

static int ioSetup_impl() {
    initPinMap();
    char *exec_path = (char *)getauxval(AT_EXECFN);
    if (exec_path) {
        strncpy(neoembux_exe_name, exec_path, PATH_MAX - 1);
        neoembux_exe_name[PATH_MAX - 1] = '\0';
    } else {
        perror("NeoEmbUX Can't Get Exe Location! It May Cause Some Bugs...");
    }

    initialized = 1;
    return EMBUX_EXIT_SUCCESS;
}

static int readPin_impl(int pin) {
    int acc_pin = pin - 1;
    if(!io[acc_pin].isInited) {
        if(initPin(acc_pin) != EMBUX_EXIT_SUCCESS) {
            return EMBUX_EXIT_FAILURE;
        }
    }
    if(!initialized) {
        fprintf(stderr, "[%s] Using ioSetup First!\n", neoembux_exe_name);
        return EMBUX_EXIT_FAILURE;
    }
    return gpiod_line_get_value(io[acc_pin].line);
}

static int setPin_impl(int pin, int mode, ...) {
    int acc_pin = pin - 1;
    int result = EMBUX_EXIT_SUCCESS;
    if(!io[acc_pin].isInited) {
        if(initPin(acc_pin) != EMBUX_EXIT_SUCCESS) {
            result = EMBUX_EXIT_FAILURE;
        }
    }
    if(!initialized) {
        fprintf(stderr, "[%s] Using ioSetup First!\n", neoembux_exe_name);
        result = EMBUX_EXIT_FAILURE;
    }
    if(gpiod_line_is_used(io[acc_pin].line)) {
        if(!strcmp(neoembux_exe_name, gpiod_line_consumer(io[acc_pin].line))) {
            gpiod_line_release(io[acc_pin].line);
        } else {
            perror("Device is in Use!");
            result = EMBUX_EXIT_FAILURE;
        }
    }
    va_list args;
    va_start(args, mode);
    if(mode == NEOEMBUX_IN) {
        if (gpiod_line_request_input(io[acc_pin].line, neoembux_exe_name) < 0) {
            perror("Set input failed");
            result = EMBUX_EXIT_FAILURE;
        }
    } else {
        int pinValue = va_arg(args, int);
        if (gpiod_line_request_output(io[acc_pin].line, neoembux_exe_name, pinValue) < 0) {
            perror("Set output failed");
            result = EMBUX_EXIT_FAILURE;
        }
    }
    va_end(args); 
    return EMBUX_EXIT_SUCCESS;
}

static bool checkPin_impl(int pin) {
    int acc_pin = pin -1;
    return io_raw[acc_pin].isGPIO;
}

static void ioRelease_impl() {
    if(!initialized) {
        fprintf(stderr, "[%s] Using ioSetup First!\n", neoembux_exe_name);
        return;
    }
    for(int i = 0; i < EMBUX_GPIO_NUM; i++) {
        if(io[i].isInited) {
            if(io[i].line) {
                gpiod_line_release(io[i].line);
                io[i].line = NULL;
            }
        }
    }

    for (int i = 0; i < EMBUX_GPIO_NUM; i++) {
        if(io[i].isInited) {
            if(io[i].chip) {
                gpiod_chip_close(io[i].chip);
                io[i].chip = NULL;
                io[i].isInited = false;
            }
        }
    }
    
    initialized = 0;
    return;
}

static int initPin(int acc_pin) {
    if(!io_raw[acc_pin].isGPIO) {
        fprintf(stderr, "[%s] Pin %i is not a GPIO\n", 
            neoembux_exe_name, acc_pin + 1);
        return EMBUX_EXIT_FAILURE;
    }

    char chippath[32];
    snprintf(chippath, sizeof(chippath), "/dev/gpiochip%d", io_raw[acc_pin].chip);

    io[acc_pin].chip = gpiod_chip_open(chippath);
    if (!io[acc_pin].chip){
        fprintf(stderr, "[%s] GPIO chip open failed: %s\n", 
            neoembux_exe_name, strerror(errno));
        return EMBUX_EXIT_FAILURE;
    }

    io[acc_pin].line = gpiod_chip_get_line(io[acc_pin].chip, io_raw[0].line);
    if (!io[acc_pin].line) {
        gpiod_chip_close(io[acc_pin].chip);
        fprintf(stderr, "[%s] GPIO line open failed: %s\n", 
            neoembux_exe_name, strerror(errno));
        return EMBUX_EXIT_FAILURE;
    }
    io[acc_pin].isInited = true;
    return EMBUX_EXIT_SUCCESS;
}

static int initPinMap(){
    for(int i = 0; i < EMBUX_GPIO_NUM; i++) {
        if(gpio[i] == EMBUX_GND || gpio[i] == EMBUX_VCC_5V || gpio[i] == EMBUX_VCC_3V3 || gpio[i] == EMBUX_UNKNOWN) {
            continue;
        } else {
            io_raw[i].chip = gpio[i] / 32;
            io_raw[i].line = gpio[i] % 32;
            io_raw[i].isGPIO = true;
        }
    }
    return EMBUX_EXIT_SUCCESS;
};

// C++命名空间版本实现
#ifdef __cplusplus
int neoEmbUx::ioSetup() {
    return ioSetup_impl(); // 复用核心实现
}
int neoEmbUx::readPin(int pin) {
    return readPin_impl(pin);
}
int neoEmbUx::setPin(int pin, int mode, ...) {
    va_list args;
    va_start(args, mode);
    if(mode == neoEmbUx::IN) {
        return setPin_impl(pin, mode);
    } else if(mode == neoEmbUx::OUT){
        int pinValue = va_arg(args, int);
        if(pinValue != neoEmbUx::HIGH && pinValue != neoEmbUx::LOW) {
            perror("Error Args: pinValue!");
            return EMBUX_EXIT_FAILURE;
        }
        return setPin_impl(pin, mode, pinValue);
    } else {
        perror("Error Args: pinMode!");
        return EMBUX_EXIT_FAILURE;
    }
    va_end(args); 
}
bool neoEmbUx::checkPin(int pin) {
    return checkPin_impl(pin);
}
void neoEmbUx::ioRelease() {
    return ioRelease_impl();
}

extern "C" { 
#endif
int neoEmbUx_ioSetup(void) {
    return ioSetup_impl();
}
int neoEmbUx_readPin(int pin) {
    return readPin_impl(pin);
}
int neoEmbUx_setPin(int pin, int mode, ...) {
    va_list args;
    va_start(args, mode);
    if(mode == NEOEMBUX_IN) {
        return setPin_impl(pin, mode);
    } else if(mode == NEOEMBUX_OUT){
        int pinValue = va_arg(args, int);
        if(pinValue != NEOEMBUX_HIGH && pinValue != NEOEMBUX_LOW) {
            perror("Error Args: pinValue!");
            return EMBUX_EXIT_FAILURE;
        }
        return setPin_impl(pin, mode, pinValue);
    } else {
        perror("Error Args: pinMode!");
        return EMBUX_EXIT_FAILURE;
    }
    va_end(args); 
    return EMBUX_EXIT_SUCCESS;
}
bool neoEmbUx_checkPin(int pin) {
    return checkPin_impl(pin);
}
void neoEmbUx_ioRelease(void) {
    ioRelease_impl();
}
#ifdef __cplusplus
}                           // 结束 extern "C" 块
#endif