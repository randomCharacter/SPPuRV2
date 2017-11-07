#ifndef TIMER_EVENT_H_
#define TIMER_EVENT_H_

typedef void* (*func_pointer_t)(void*);

typedef enum TE_KIND {
	TE_KIND_ONCE,
	TE_KIND_REPETITIVE,
} timer_event_kind_t;

typedef struct timer_event {
	func_pointer_t func;
	void* func_arg;
	timer_event_kind_t kind;
	int abs_time;

	int rel_time;
	struct timer_event* next;
} timer_event_s_t;

typedef timer_event_s_t* timer_event_t;

int timer_event_set (timer_event_t* timer_var, unsigned int delay, void* (*routine)(void*), void* arg, int te_kind);
int timer_event_kill (timer_event_t timer_var);

#endif /* TIMER_EVENT_H_ */
