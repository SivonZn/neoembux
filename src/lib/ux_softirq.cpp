#include <errno.h>
#include <string.h>
#include "../../include/neoembux/ux_softirq.h"

static const char *edge_event_type_str(struct gpiod_edge_event *event) {
	switch (gpiod_edge_event_get_event_type(event)) {
	case GPIOD_EDGE_EVENT_RISING_EDGE:
		return "Rising";
	case GPIOD_EDGE_EVENT_FALLING_EDGE:
		return "Falling";
	default:
		return "Unknown";
	}
}

void* irq_monitor_thread(void* arg) {
    struct gpiod_edge_event *event;
    struct gpiod_edge_event_buffer *event_buffer;
    
    int i, ret, event_buf_size;
    event_buf_size = 1;
	event_buffer = gpiod_edge_event_buffer_new(event_buf_size);
	if (!event_buffer) {
		fprintf(stderr, "failed to create event buffer: %s\n", strerror(errno));
        return NULL;
	}

    IRQThreadPayload* payload = (IRQThreadPayload*)arg;
    struct gpiod_line_request *request = payload->request;

    // printf("IRQ monitor thread started. TID: %ld\n", pthread_self());

    while(1) {
        /* Blocks until at least one event is available. */
        ret = gpiod_line_request_read_edge_events(request, event_buffer,
                                event_buf_size);
        if (ret == -1) {
            fprintf(stderr, "error reading edge events: %s\n", strerror(errno));
            return NULL;
        }
        for (i = 0; i < ret; i++) {
            event = gpiod_edge_event_buffer_get_event(event_buffer, i);
            printf("offset: %d  type: %-7s  event #%ld\n",
                    gpiod_edge_event_get_line_offset(event),
                    edge_event_type_str(event),
                    gpiod_edge_event_get_line_seqno(event));
        }
        usleep(50000);
    }
    return NULL;
}