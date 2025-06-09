#ifndef UX_SOFTIRQ
#define UX_SOFTIRQ
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <poll.h>

typedef void (*IRQExecFunc)(void*);

typedef struct {
    gpiod_line *line;
    IRQExecFunc func;  // 目标函数指针
    void* arg;           // 目标函数的参数
} IRQThreadPayload;

void* irq_monitor_thread(void* arg);

#endif