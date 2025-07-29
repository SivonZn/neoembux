#include "../../include/neoembux.h"
#include "../../include/neoembux/ux_gpio.h"
#include "../../include/neoembux/ux_softirq.h"

static char embux_exe_name[PATH_MAX] = "NeoEmbUX";
struct gpiod_request_config *embux_reqcfg = NULL;

static int initialized = 0;

struct io_raw io_raw[EMBUX_GPIO_NUM];
struct io io[EMBUX_GPIO_NUM];

static int initPin(int acc_pin);
static int initPinMap();
static void releasePin_impl(int acc_pin);

static int ioSetup_impl() {
    // Init the consumer name
    char *exec_path = (char *)getauxval(AT_EXECFN);
    if (exec_path) {
        strncpy(embux_exe_name, exec_path, PATH_MAX - 1);
        embux_exe_name[PATH_MAX - 1] = '\0';
    } else {
        perror("NeoEmbUX Can't Get Exe Location! It May Cause Some Bugs...");
    }
    if(embux_reqcfg == NULL) {
        gpiod_request_config_set_consumer(embux_reqcfg, embux_exe_name);
    }
    // Init Pin map
    initPinMap();
    initialized = 1;
    return EMBUX_EXIT_SUCCESS;
}

static int readPin_impl(int pin) {
    // Transform Pin 1~40 to 0~39
    int acc_pin = pin - 1;

    // Check if IO is inited
    if(!initialized) {
        fprintf(stderr, "[%s] Using ioSetup First!\n", embux_exe_name);
        return EMBUX_EXIT_FAILURE;
    }
    if(!io[acc_pin].isInited) {
        if(initPin(acc_pin) != EMBUX_EXIT_SUCCESS) {
            return EMBUX_EXIT_FAILURE;
        }
    }

    if(!io[acc_pin].request) {
        fprintf(stderr, "[%s] Set pin direction first!\n", embux_exe_name);
        return EMBUX_EXIT_FAILURE;
    }

    return gpiod_line_request_get_value(io[acc_pin].request, io_raw[acc_pin].line);
}

static int setPin_impl(int acc_pin, int mode, ...) {
    // Default result
    int result = EMBUX_EXIT_SUCCESS;

    // Check if IO is inited
    if(!initialized) {
        fprintf(stderr, "[%s] Using ioSetup First!\n", embux_exe_name);
        result = EMBUX_EXIT_FAILURE;
    }
    if(!io[acc_pin].isInited) {
        if(io[acc_pin].irq_isSet) {
            result = EMBUX_EXIT_FAILURE;
        }
        else if(initPin(acc_pin) != EMBUX_EXIT_SUCCESS) {
            result = EMBUX_EXIT_FAILURE;
        }
    }

    // Check if IO is in used
    struct gpiod_line_info *current_line_info;
    current_line_info = gpiod_chip_get_line_info(io[acc_pin].chip, io_raw[acc_pin].line);
    if(gpiod_line_info_is_used(current_line_info)) {
        if(!strcmp(embux_exe_name, gpiod_line_info_get_consumer(current_line_info))) {
            releasePin_impl(acc_pin);
            initPin(acc_pin);
        } else {
            perror("Device is in Use!");
            result = EMBUX_EXIT_FAILURE;
        }
    }
    gpiod_line_info_free(current_line_info);

    // Parse dynamic args 
    va_list args;
    va_start(args, mode);
    if(mode == NEOEMBUX_IN) {
        // Input mode
        gpiod_line_settings_set_direction(io[acc_pin].settings, GPIOD_LINE_DIRECTION_INPUT);
        int ret = gpiod_line_config_add_line_settings(io[acc_pin].line_cfg, &io_raw[acc_pin].line, 1, io[acc_pin].settings);
        if (ret < 0) {
            perror("Set input failed");
            result = EMBUX_EXIT_FAILURE;
        } else {
            io[acc_pin].direction = NEOEMBUX_IN;
        }
    } else if(mode == NEOEMBUX_OUT) {
        // Output mode
        int pinValue = va_arg(args, int);
        gpiod_line_settings_set_direction(io[acc_pin].settings, GPIOD_LINE_DIRECTION_OUTPUT);
        gpiod_line_settings_set_output_value(io[acc_pin].settings, pinValue ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
        int ret = gpiod_line_config_add_line_settings(io[acc_pin].line_cfg, &io_raw[acc_pin].line, 1, io[acc_pin].settings);
        if (ret < 0) {
            perror("Set output failed");
            result = EMBUX_EXIT_FAILURE;
        } else {
            io[acc_pin].direction = NEOEMBUX_OUT;
        }
    }
    va_end(args); 
    io[acc_pin].request = gpiod_chip_request_lines(io[acc_pin].chip, embux_reqcfg, io[acc_pin].line_cfg);
    if (!io[acc_pin].request) {
		fprintf(stderr, "failed to request line: %s\n",
			strerror(errno));
		return EXIT_FAILURE;
	}

    return EMBUX_EXIT_SUCCESS;
}

static bool checkPin_impl(int acc_pin) {
    return io_raw[acc_pin].isGPIO;
}

int setPinSoftIRQ_impl(int pin, int edge, void* fun) {
    // Transform Pin 1~40 to 0~39
    int acc_pin = pin - 1;

    // Check if IO is inited
    if(!initialized) {
        fprintf(stderr, "[%s] Using ioSetup First!\n", embux_exe_name);
        return EMBUX_EXIT_FAILURE;
    }
    if(!io[acc_pin].isInited) {
        if(initPin(acc_pin) != EMBUX_EXIT_SUCCESS) {
            return EMBUX_EXIT_FAILURE;
        }
    }

    setPin_impl(acc_pin, NEOEMBUX_IN);

    // Check if IO is in used
    struct gpiod_line_info *current_line_info;
    current_line_info = gpiod_chip_get_line_info(io[acc_pin].chip, io_raw[acc_pin].line);
    if(gpiod_line_info_is_used(current_line_info)) {
        if(!strcmp(embux_exe_name, gpiod_line_info_get_consumer(current_line_info))) {
            releasePin_impl(acc_pin);
            initPin(acc_pin);
        } else {
            perror("Device is in Use!");
            return EMBUX_EXIT_FAILURE;
        }
    }
    gpiod_line_info_free(current_line_info);

    // Set edge event
    gpiod_line_settings_set_direction(io[acc_pin].settings, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_edge_detection(io[acc_pin].settings, (edge == NEOEMBUX_RISING) ? GPIOD_LINE_EDGE_RISING : GPIOD_LINE_EDGE_FALLING);
    int ret = gpiod_line_config_add_line_settings(io[acc_pin].line_cfg, &io_raw[acc_pin].line, 1, io[acc_pin].settings);
    if (ret < 0) {
        perror("Set Input failed");
        return EMBUX_EXIT_FAILURE;
    } else {
        io[acc_pin].direction = NEOEMBUX_IN;
    }
    io[acc_pin].request = gpiod_chip_request_lines(io[acc_pin].chip, embux_reqcfg, io[acc_pin].line_cfg);
    if (!io[acc_pin].request) {
		fprintf(stderr, "failed to request line: %s\n",
			strerror(errno));
		return EXIT_FAILURE;
	}

    // Payload init
    IRQThreadPayload* payload = static_cast<IRQThreadPayload*>(malloc(sizeof(IRQThreadPayload)));
    payload->func = (IRQExecFunc)fun;
    payload->request = io[acc_pin].request;

    // Create new thread to process edge event
    if (pthread_create(&io[acc_pin].irq_thread, NULL, irq_monitor_thread, payload)) {
        perror("Failed to create IRQ thread");
        free(payload);
        return EMBUX_EXIT_FAILURE;
    }
    if (pthread_detach(io[acc_pin].irq_thread)) {
        perror("Failed to create IRQ thread");
        return EMBUX_EXIT_FAILURE;
    }

    return EMBUX_EXIT_SUCCESS;
}

static void releasePin_impl(int acc_pin) {
    gpiod_line_config_free(io[acc_pin].line_cfg);
    gpiod_line_settings_free(io[acc_pin].settings);
    gpiod_line_request_release(io[acc_pin].request);
    io[acc_pin].request = NULL;
    io[acc_pin].settings = NULL;
    io[acc_pin].line_cfg = NULL;
}

static void ioRelease_impl() {
    if(!initialized) {
        fprintf(stderr, "[%s] Using ioSetup First!\n", embux_exe_name);
        return;
    }
    for(int i = 0; i < EMBUX_GPIO_NUM; i++) {
        if(io[i].isInited) {
            releasePin_impl(i);
        }
    }

    for (int i = 0; i < EMBUX_GPIO_NUM; i++) {
        if(io[i].isInited) {
            if(io[i].chip) {
                gpiod_chip_close(io[i].chip);
                io[i].chip = NULL;
            }
            io[i].isInited = false;
        }
    }
    gpiod_request_config_free(embux_reqcfg);
    initialized = 0;
    return;
}

static int initPin(int acc_pin) {
    // Is this Pin is GPIO?
    if(!io_raw[acc_pin].isGPIO) {
        fprintf(stderr, "[%s] Pin %i is not a GPIO\n", 
            embux_exe_name, acc_pin + 1);
        return EMBUX_EXIT_FAILURE;
    }

    // Open Pin's GPIO chip
    char chippath[32];
    snprintf(chippath, sizeof(chippath), "/dev/gpiochip%d", io_raw[acc_pin].chip);

    io[acc_pin].chip = gpiod_chip_open(chippath);
    if (!io[acc_pin].chip){
        fprintf(stderr, "[%s] GPIO chip open failed: %s\n", 
            embux_exe_name, strerror(errno));
        return EMBUX_EXIT_FAILURE;
    }

    // Init GPIO setting
    io[acc_pin].settings = gpiod_line_settings_new();
    if (!io[acc_pin].settings){
        fprintf(stderr, "[%s] GPIO setting init failed: %s\n", 
            embux_exe_name, strerror(errno));
        return EMBUX_EXIT_FAILURE;
    }

    // Init default line config
    io[acc_pin].line_cfg = gpiod_line_config_new();
    if (!io[acc_pin].line_cfg){
        fprintf(stderr, "[%s] GPIO config init failed: %s\n", 
            embux_exe_name, strerror(errno));
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
            // Map all raw IO to NeoEmbUX IO
            io_raw[i].chip = gpio[i] / 32;
            io_raw[i].line = gpio[i] % 32;
            io_raw[i].isGPIO = true;
        }
    }
    return EMBUX_EXIT_SUCCESS;
};

#ifdef __cplusplus
int neoEmbUx::ioSetup() {
    return ioSetup_impl();
}
int neoEmbUx::readPin(int pin) {
    return readPin_impl(pin);
}
int neoEmbUx::setPin(int pin, int mode, ...) {
    // Transform Pin 1~40 to 0~39
    int acc_pin = pin - 1;
    va_list args;
    va_start(args, mode);
    if(mode == neoEmbUx::IN) {
        return setPin_impl(acc_pin, mode);
    } else if(mode == neoEmbUx::OUT){
        int pinValue = va_arg(args, int);
        if(pinValue != neoEmbUx::HIGH && pinValue != neoEmbUx::LOW) {
            perror("Error Args: pinValue!");
            return EMBUX_EXIT_FAILURE;
        }
        return setPin_impl(acc_pin, mode, pinValue);
    } else {
        perror("Error Args: pinMode!");
        return EMBUX_EXIT_FAILURE;
    }
    va_end(args); 
}
bool neoEmbUx::checkPin(int pin) {
    // Transform Pin 1~40 to 0~39
    int acc_pin = pin - 1;
    return checkPin_impl(acc_pin);
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
    // Transform Pin 1~40 to 0~39
    int acc_pin = pin - 1;
    va_list args;
    va_start(args, mode);
    if(mode == NEOEMBUX_IN) {
        return setPin_impl(acc_pin, mode);
    } else if(mode == NEOEMBUX_OUT){
        int pinValue = va_arg(args, int);
        if(pinValue != NEOEMBUX_HIGH && pinValue != NEOEMBUX_LOW) {
            perror("Error Args: pinValue!");
            return EMBUX_EXIT_FAILURE;
        }
        return setPin_impl(acc_pin, mode, pinValue);
    } else {
        perror("Error Args: pinMode!");
        return EMBUX_EXIT_FAILURE;
    }
    va_end(args); 
    return EMBUX_EXIT_SUCCESS;
}
bool neoEmbUx_checkPin(int pin) {
    // Transform Pin 1~40 to 0~39
    int acc_pin = pin - 1;
    return checkPin_impl(acc_pin);
}
void neoEmbUx_ioRelease(void) {
    ioRelease_impl();
}
#ifdef __cplusplus
}                           // 结束 extern "C" 块
#endif