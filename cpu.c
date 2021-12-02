#include "oslab.h"
#define NULLPCB [PID:0, AT:0, TBT:0. EST:0, EET:0, RBT:0, Priority:0]

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    new_process.arrival_timestamp = timestamp;
    if (current_process == NULLPCB) {
	new_process.execution_starttime = timestamp;
	new_process.execution_endtime = timestamp + new_process.total_bursttime;
	new_process.remaining_bursttime = new_process.total_bursttime;
	return new_process;
    } else {
	if (new_process.process_priority >= current_process.process_priority) {
	    ready_queue[queue_cnt] = new_process;
	    new_process.execution_starttime = 0;
	    new_process.execution_endtime = 0;
	    new_process.remaining_bursttime = new_process.total_bursttime;
	    return current_process;
	} else {
	    int bursted = timestamp - current_process.execution_starttime;
	    current_process.remaining_bursttime = current_process.remaining_bursttime - bursted;
	    current_process.execution_starttime = 0;
	    current_process.execution_endtime = 0;
	    ready_queue[queue_cnt] = current_process;
	    new_process.execution_starttime = timestamp;
	    new_process.execution_endtime = timestamp + new_process.total_bursttime;
	    new_process.remaining_bursttime = new_process.total_bursttime;
	    return new_process;
	}
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (queue_cnt == 0) {
	return NULLPCB;
    } else {
	struct PCB first = ready_queue[0];
	int priority = ready_queue[0].process_priority;
	int count = 0;
	for (int i = 0; i < queue_cnt; i++) { 
	    if (ready_queue[i].process_priority < priority) {
		first = ready_queue[i];
		priority = ready_queue[i].process_priority;
		count = i;
	    }
	}
	first.execution_starttime = timestamp;
	first.execution_endtime = timestamp + first.total_bursttime;
	first.remaining_bursttime = first.total_bursttime;
	for (int j = count + 1; j < queue_cnt; j++) {
	    ready_queue[j - 1] = ready_queue[j];
	}
	ready_queue[queue_cnt - 1] = NULLPCB;
	return first;
    }
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    new_process.arrival_timestamp = timestamp;
    if (current_process == NULLPCB) {
	new_process.execution_starttime = timestamp;
	new_process.execution_endtime = timestamp + new_process.total_bursttime;
	new_process.remaining bursttime = new_process.total_bursttime;
	return new_process;
    } else {
	if (new_process.total_bursttime >= current_process.remaining_bursttime) {
	    new_process.execution_starttime = 0;
	    new_process.execution_endtime = 0;
	    new_process.remaining_bursttime = new_process.total_bursttime;
	    ready_queue[queue_cnt] = new_process;
	    return current_process;
	} else {
	    int bursted = timestamp - current_process.execution_starttime;
	    current_process.remaining_bursttime = current_process.remaining_bursttime - bursted;
	    current_process.execution_starttime = 0;
	    current_process.execution_endtime = 0;
	    ready_queue[queue_cnt] = current_process;
	    new_process.execution_starttime = timestamp;
	    new_process.execution_endtime = timestamp + new_process.total_bursttime;
	    new_process.remaining_bursttime = new_process.total_bursttime;
	    return new_process;
	}
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (queue_cnt == 0) {
	return NULLPCB;
    } else {
	struct PCB shortest = ready_queue[0];
	int len = ready_queue[0].remaining_bursttime;
	int count = 0;
	for (int i = 0; i < queue_cnt; i++) {
	    if (ready_queue[i].remaining_bursttime < len) {
		len = ready_queue[i].remaining_bursttime;
		shortest = ready_queue[i];
		count = i;
	    }
	}
	shortest.execution_starttime = timestamp;
	shortest.execution_endtime = timestamp + shortest.remaining_bursttime;
	for (int j = count + 1; j < queue_cnt; j++) {
	    ready_queue[j - 1] = ready_queue[j];
	}
	ready_queue[queue_cnt - 1] = NULLPCB;
	return shortest;
    }
}

struct PCB handle_process_arrive_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    new_process.arrival_timestamp = timestamp;
    if (current_process == NULLPCB) {
	new_process.execution_starttime = timestamp;
	new_process.execution_endtime = timestamp + MIN(time_quantum, new_process.total_bursttime);
	new_process.remaining_bursttime = new_process.total_bursttime;
	return new_process;
    } else {
	new_process.execution_starttime = 0;
	new_process.execution_endtime = 0;
	new_process.remaining_bursttime = new_process.total_bursttime;
	ready_queue[queue_cnt] = new_process;
	return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if (queue_cnt == 0) {
	return NULLPCB;
    } else {
	struct PCB earliest = ready_queue[0];
	int time = ready_queue[0].arrival_timestamp;
	int count = 0;
	for (int i = 0; i < queue_cnt; i++) {
	    if (ready_queue[i].arrival_timestamp < time) {
		earliest = ready_queue[i];
		time = ready_queue[i].arrival_timestamp;
		count = i;
	    }
	}
	earliest.execution_starttime = timestamp;
	earliest.execution_endtime = timestamp + MIN(time_quantum, earliest.total_bursttime);
	for (int j = count + 1; j < queue_cnt; j++){
	    ready_queue[j - 1] = ready_queue[j];
	}
	ready_queue[queue_cnt - 1] = NULLPCB;
	return earliest;
    }
}
