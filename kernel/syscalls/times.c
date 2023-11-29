#include <stdint.h>

#include "syscalls/times.h"

#include "processes/process.h"

int32_t times(struct tms *buf) {

	buf->tms_utime=current_process->total_time;
	buf->tms_stime=0;
	buf->tms_cutime=0;
	buf->tms_cstime=0;

	return 0;
}

#include "time/time.h"
uint32_t sys_get_current_time(void) {
    return tick_counter;
}
