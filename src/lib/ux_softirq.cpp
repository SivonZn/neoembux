#include "../../include/neoembux/ux_softirq.h"

void* irq_monitor_thread(void* arg) {
    struct gpiod_line_bulk gpiod_line_bulk, event_bulk;
    struct gpiod_line_event event;
    int ret;

    IRQThreadPayload* payload = (IRQThreadPayload*)arg;
    struct gpiod_line* line = payload->line;

    gpiod_line_bulk_init(&gpiod_line_bulk);
    gpiod_line_bulk_add(&gpiod_line_bulk, line);
    
    // printf("IRQ monitor thread started. TID: %ld\n", pthread_self());

    while(1) {
        // 等待中断发生（阻塞）
        ret = gpiod_line_event_wait_bulk(&gpiod_line_bulk, NULL, &event_bulk);
        if (ret < 0) {
            printf("gpiod line event wait error\n");
        }
        else if(ret == 0) {
            printf("gpiod line event wait timeout\n");
        }
        else {
            /* 解析检测到的事件 */
            for(int i = 0; i < event_bulk.num_lines; i++) {
                if(gpiod_line_event_read(event_bulk.lines[i], &event) == 0) {
                    printf("line %d event happen\n", gpiod_line_offset(event_bulk.lines[i]));
                    payload->func(payload->arg);
                    break;
                    /* 还可以根据event中的内容判断当前检测到的事件是上升沿还是下降沿*/
                }                  
            }
            usleep(50000);
        }
    }
    return NULL;
}