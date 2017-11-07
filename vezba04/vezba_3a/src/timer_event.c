#include "timer_event.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// CaS primitive
static pthread_mutex_t CaS_mutex = PTHREAD_MUTEX_INITIALIZER;

int CaS (int* x, int old, int new) {
	int ret_val = 0;

	pthread_mutex_lock(&CaS_mutex);
	if (*x == old) {
		*x = new;
		ret_val = 1;
	}
	pthread_mutex_unlock(&CaS_mutex);

	return ret_val;
}
// --------------------

// Globals
static timer_event_s_t list_head = {0,0,0,0,0, NULL};

static pthread_attr_t attr;
static pthread_mutex_t list_mux;

static unsigned int resolution = 10;

static int init = 0;

// List functions

// AddToList
void add_to_list (timer_event_s_t* x) {
	timer_event_s_t* iter;
	int sum = 0;

	pthread_mutex_lock(&list_mux);

	// iterate through list and find where to place element x
	iter = &list_head;
	while ( (iter->next != NULL) && (x->abs_time > iter->next->rel_time + sum) ) {
		sum += iter->next->rel_time;
		iter = iter->next;
	}

	// set rel_time
	x->rel_time = x->abs_time - sum;
	// add to list after iter
	x->next = iter->next;
	iter->next = x;

	// update next's rel_time
	if (x->next != NULL) {
		// delete added element's rel_time from next's rel_time
		x->next->rel_time = x->next->rel_time - x->rel_time;
	}

	pthread_mutex_unlock(&list_mux);
}

// RemoveFromList
// Returns removed element pointer. If there is no such element in the list
// returns NULL
timer_event_s_t* remove_from_list (timer_event_s_t* x) {
	timer_event_s_t* iter;
	timer_event_s_t* ret_val = NULL;

	pthread_mutex_lock(&list_mux);

	// find if that timer exists in the list
	iter = &list_head;
	while ( (iter->next != x) && (iter->next != NULL) ) {
		iter = iter->next;
	}

	if (iter->next == x) {
		ret_val = iter->next;
		// remove it from list
		iter->next = iter->next->next;

		// update next's rel_time
		if (iter->next != NULL) {
			// add deleted element's rel_time to next's rel_time
			iter->next->rel_time = iter->next->rel_time + x->rel_time;
		}
	}

	pthread_mutex_unlock(&list_mux);

	return ret_val;
}
// --------------------

// this function is called from catcher
void timer_event_deinit (void);

// function for catching timer signal
void catcher (int x) {
	timer_event_s_t* current;

	current = list_head.next;

	if (current != NULL) {
		current->rel_time -= resolution;
		if (current->rel_time <= 0)	{
			do {
				pthread_t tmp; // just for starting thread
				func_pointer_t func;
				void* func_arg;

				// gather informations needed for thread starting
				func = current->func;
				func_arg = current->func_arg;

				// remove from list
				current = remove_from_list(current);

				if (current->kind == TE_KIND_REPETITIVE) {   
					// if repetitive
					// put back to the list -> it needs to be triggered again
					add_to_list(current);
					// don't check if list is empty, we just added an entry
				} else {   
					// if only once
					// delete time event descriptor, it has done it's purpose
					free(current);
				}

				// finally start thread
				pthread_create(&tmp, &attr, func, func_arg);

				current = list_head.next;

			} while ( (current != NULL) && (current->rel_time <= 0) );

			// check if list is empty
			if (list_head.next == NULL) {
				// do deinit if there are no timer events any more
				timer_event_deinit();
			}

		}
	}
}
// --------------------

// Timer_event init and deinit functions
void timer_event_init (void) {
	struct sigaction sact;
	struct itimerval timer_val;

	if (CaS(&init,0,1)) {
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_mutex_init(&list_mux, NULL);

		// assign timer signal cathcer
		sigemptyset(&sact.sa_mask);
		sact.sa_flags = SA_NODEFER;
		sact.sa_handler = catcher;
		sigaction(SIGALRM, &sact, NULL);

		// start timer
		timer_val.it_interval.tv_sec = 0;
		timer_val.it_interval.tv_usec = resolution * 1000; // 10ms
		timer_val.it_value.tv_sec = 0;
		timer_val.it_value.tv_usec = resolution * 1000; // 10ms

		setitimer(ITIMER_REAL, &timer_val, NULL);
	}
}

void timer_event_deinit (void) {
	timer_event_s_t* temp;
	struct itimerval timer_val;

	if (CaS(&init,1,0)) {
		// stop timer
		timer_val.it_interval.tv_sec = 0;
		timer_val.it_interval.tv_usec = 0;
		timer_val.it_value.tv_sec = 0;
		timer_val.it_value.tv_usec = 0;
		setitimer(ITIMER_REAL, &timer_val, NULL);

		// delete list
		pthread_mutex_lock(&list_mux);
		temp = list_head.next;
		while (temp != NULL) {
			timer_event_s_t* p = temp;
			temp = temp->next;
			free(p);
		}
		pthread_mutex_unlock(&list_mux);

		// destroy global attribute
		pthread_attr_destroy(&attr);
		pthread_mutex_destroy(&list_mux);
	}
}
// --------------------

// Timer_event user functions
int timer_event_set (timer_event_t* timer_var, unsigned int delay, void* (*routine)(void*), void* arg, int te_kind) {
	timer_event_s_t* new_timer = malloc(sizeof(timer_event_s_t));

	timer_event_init();

	new_timer->abs_time = delay;
	new_timer->func = routine;
	new_timer->func_arg = arg;
	new_timer->kind = te_kind;
	// just to have every element initialized
	new_timer->rel_time = 0;
	new_timer->next = 0;

	add_to_list(new_timer);

	*timer_var = new_timer;

	return 0;
}

int timer_event_kill (timer_event_s_t* x) {
	int ret_val = -1;
	timer_event_s_t* temp;

	temp = remove_from_list(x);

	if (temp != NULL) {
		free(temp);
		ret_val = 0;
	}

	return ret_val;
}
// --------------------
