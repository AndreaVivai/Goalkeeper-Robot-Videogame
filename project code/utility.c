#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <allegro.h>
#include <time.h>
#include <semaphore.h>
//-----------------------
#include "utility.h"
#include "colors_set.h"
#include "interface.h"
#include "ball_physics.h"
#include "graphics.h"


//-----------------------------------------------------
// TASK VARIABLES
//-----------------------------------------------------
struct task_param	tp[MAX_THREADS];        // Array to store the information about the executing tasks
int		            termination_signal = 0;	// 1:terminate all task  |  0:continue task
struct timespec     start_time;             // will be initialize when the program start
//-----------------------------------------------------

int sigterm_tasks = 0;  // termination signal for every task

//-----------------------------------------------------
// GRAPHIC VARIABLES
//-----------------------------------------------------
BITMAP	*screen_buff;	// buffer that is ultimately placed on the screen
BITMAP  *ball_bitmap;   // picture of a ball, downloaded from internet


//-----------------------------------------------------
// SEMAPHORES
//-----------------------------------------------------
pthread_mutex_t phys_sem; 
pthread_mutex_t motor_sem;
pthread_mutex_t vision_sem;


//-----------------------------------------------------
//  TIME FUNTIONS:
//-----------------------------------------------------

// return the current time (since start_time)
long get_systime(int unit) {

    struct timespec t;
    long tu, mul, div;

    switch (unit) {
        case 6: mul = 1000000; div = 1000; break;  // MICROSEC
        case 3: mul = 1000; div = 1000000; break;  // MILLISEC
        default: mul = 1000; div = 1000000; break; // SEC
    }

    clock_gettime(CLOCK_MONOTONIC, &t);
    tu = (t.tv_sec - start_time.tv_sec) * mul;
    tu += (t.tv_nsec - start_time.tv_nsec) / div;

    return tu;
}


// td : destination variable   ;   ts: starting variable 
void time_copy(struct timespec *td,struct timespec ts) {
    td->tv_sec = ts.tv_sec;
    td->tv_nsec = ts.tv_nsec;
}


// addition of milliseconds
void time_add_ms(struct timespec *t, int ms) {

    t->tv_sec += ms/1000;
    t->tv_nsec += (ms%1000)*1000000;

    if (t->tv_nsec > 1000000000) {
        t->tv_nsec -= 1000000000;
        t->tv_sec += 1;
    }   
}


// Compares two given times and returns 1 if first 
// is bigger, -1 if it's lower, 0 if equals to t2.

// time comparison
int time_cmp(struct timespec t1, struct timespec t2) {

    if (t1.tv_sec > t2.tv_sec) return 1;
    if (t1.tv_sec < t2.tv_sec) return -1;
    if (t1.tv_nsec > t2.tv_nsec) return 1;
    if (t1.tv_nsec < t2.tv_nsec) return -1;
    return 0;
}




//-----------------------------------------------------
//  TASK FUNTIONS:
//-----------------------------------------------------

// Function to create a periodic task and associate it with a thread.
//   - task_function:   Pointer to the task callback function to be executed by the thread.
//   - ind:             Index representing the task, used for tracking and identification.
//   - period:          Period of the task in milliseconds.
//   - drel:            Relative deadline of the task in milliseconds.
//   - priority:        Priority of the task in the range [0, 99].
// Returns the result of pthread_create or -1 if the task index exceeds the maximum allowed.

pthread_t start_task(void* (*task_function)(void *), int ind, int period, int drel, int priority) {

    // Check if the task index exceeds the maximum allowed
    if (ind >= MAX_THREADS) 
        return -1;

    // Set task parameters in tp
    tp[ind].index = ind; 
    tp[ind].period = period;
    tp[ind].deadline = drel;
    tp[ind].priority = priority;
    tp[ind].dmiss = 0;

    // set attributes
    pthread_attr_t myatt;                                           
    pthread_attr_init(&myatt);
    pthread_attr_setinheritsched(&myatt, PTHREAD_EXPLICIT_SCHED);   // set thread scheduling policy
    pthread_attr_setschedpolicy(&myatt, SCHED_FIFO);                // set thread priority
    
    // set  parameters (inside tid)
    struct sched_param my_sched_par;                                
    my_sched_par.sched_priority = tp[ind].priority;
    pthread_attr_setschedparam(&myatt, &my_sched_par);
    
    // Create a thread associated with the task
    pthread_t thread_ID = tp[ind].tid;
    pthread_create(&thread_ID, &myatt, task_function, (void*)(&tp[ind]));

    return thread_ID;  // 1:created | 0:error
}



void kill_task(int ind) {
    tp[ind].index = -1; 
    tp[ind].period = 0;
    tp[ind].deadline = 0;
    tp[ind].priority = 0;
    tp[ind].dmiss = 0;
}




// Find the first element i of ball_status[i].activation with value 0 (ball not active)
int find_free_ball() {
    int max_index = MAX_BALLS;
	for (int i = 0 ; i < max_index; i++) {
		if (ball_status[i].activation == 0)
			return i;
	}
	return -1; // not found
}



// find the index of the ball who's shooting
int find_shooting_ball() {
    int max_index = MAX_BALLS;
	for (int i = 0 ; i < max_index; i++) {
		if (ball_status[i].activation == 2)
			return i;
	}
	return -1; // not found
}





// Function to get the task index from its index
// Parameters:
//   - index: A void pointer to the index containing a task parameter structure
// Returns:
//   - The task index extracted from the index structure

int get_task_index(void* index) {
    struct task_param *tpar;
    tpar = (struct task_param *)index;
    return tpar->index;  
}




// Function to wait for the activation of a task
// Parameters:
//   - i: Index representing the task
// Behavior:
//   - Waits for the activation semaphore of the specified task.
//   - Retrieves the current time and updates the activation time (at) and absolute deadline (dl) of the task.
//   - Computes the next activation time and deadline based on the task's period and deadline.
// Note:
//   - Assumes the existence of time_copy, time_add_ms, and sem_wait functions.

void wait_for_activation(int i) {
    struct timespec t;

    sem_wait(&tp[i].tsem);
    clock_gettime(CLOCK_MONOTONIC, &t);
    time_copy(&(tp[i].at), t);
    time_copy(&(tp[i].dl), t);
    time_add_ms(&(tp[i].at), tp[i].period);
    time_add_ms(&(tp[i].dl), tp[i].deadline);
}




// Function to check if a deadline is missed
// Parameters:
//   - i: Index representing the task
// Returns:
//   - 1 if the task has missed its deadline, 0 otherwise

int deadline_miss(int i) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (time_cmp(now, tp[i].dl) > 0) {
        tp[i].dmiss++;
        return 1;
    }
    return 0;
}



// Function to wait for the next period of a task, then updates period and deadline
//   - index: Index representing the task

void wait_for_period(int index) {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(tp[index].at), NULL); // sleep until activation time

    // update deadline and period
    time_add_ms(&(tp[index].at), tp[index].period);
    time_add_ms(&(tp[index].dl), tp[index].period);
}



// Update tp array computing next activation time and absolute deadline of a task
// -index : index of the task that we want to update the perdiod and deadline

void set_period_deadline(int index) {

    // update tp array
    // set period and deadline to now
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	time_copy(&(tp[index].at), now);
	time_copy(&(tp[index].dl), now);

    // add the corrisponding time to deadline and period
	time_add_ms(&(tp[index].at), tp[index].period);
	time_add_ms(&(tp[index].dl), tp[index].deadline);
}



// Function to get the period of a task
// Parameters:
//   - i: Index representing the task
// Returns:
//   - The period of the specified task

int task_period(int i) {
    return tp[i].period;
}




// Function to get the deadline of a task
// Parameters:
//   - i: Index representing the task
// Returns:
//   - The deadline of the specified task

int task_deadline(int i) {
    return tp[i].deadline;
}




// Function to get the number of deadline misses for a task
// Parameters:
//   - i: Index representing the task
// Returns:
//   - The number of deadline misses for the specified task

int task_dmiss(int i) {
    return tp[i].dmiss;
}




// Function to get the activation time of a task
// Parameters:
//   - i: Index representing the task
//   - at: Pointer to a timespec structure to store the activation time

void task_atime(int i, struct timespec *at) {
    at->tv_sec = tp[i].at.tv_sec;
    at->tv_nsec = tp[i].at.tv_nsec;
}




// Function to get the absolute deadline of a task
// Parameters:
//   - i: Index representing the task
//   - dl: Pointer to a timespec structure to store the absolute deadline

void task_adline(int i, struct timespec *dl) {
    dl->tv_sec = tp[i].dl.tv_sec;
    dl->tv_nsec = tp[i].dl.tv_nsec;
}




// Function to wait for the completion of a task
// Parameters:
//   - i: Index representing the task
// Behavior:
//   - Waits for the associated thread to complete using pthread_join

void wait_for_task_end(int i) {
    pthread_join(tp[i].tid, NULL);
}



//Find the first element of tp array 
int find_free_index(int min_index, int max_index) {
	int i = min_index;
	while (i < max_index) {
		if (tp[i].index == -1)
			return i;
		i++;
	}
	return -1;
}



// Difference from time t1 to t2, in milliseconds. (t1>t2)
float time_diff_ms(struct timespec t1, struct timespec t2) {
	// Check consistency
	if (t1.tv_sec < t2.tv_sec || (t1.tv_sec == t2.tv_sec && t1.tv_nsec < t2.tv_nsec))
		return -1;

	return (t1.tv_sec - t2.tv_sec) * 1000.0 + (t1.tv_nsec - t2.tv_nsec) / 1000000.0;
}



//-----------------------------------------------------
//  ALLEGRO FUNTIONS:
//-----------------------------------------------------

//  Blocking function that wait for an input from keyboard and returns scancode and ascii.	
//  scan:  scancode of the pressed key
//  ascii: ascii code of the pressed key

void get_keycodes(char *scan, char *ascii) {
	int k;
	k = readkey();	// block until a key is pressed
	*ascii = k;		// get ascii code
	*scan = k >> 8;	// get scan code
}




// Non-blocking function that check if a key is pressed and, if true, return the corresponding scancode.

int listen_scancode() {
	if (keypressed())
		return readkey() >> 8;
	return 0;
}



void init_graphic_mouse_keyboard() {
    
	// set all thread as not activated
	for (int i = 0; i <= MAX_THREADS; i++){
        tp[i].index = -1;
    }	

    // screen
	allegro_init();
	set_color_depth(COLOR_DEPTH);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    // keyboard
	install_keyboard();

    //mouse
	install_mouse();
    show_mouse(screen);

	// this bitmap will be the one print on the screen | visible to every task
	screen_buff = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	clear_bitmap(screen_buff);
	clear_to_color(screen_buff, GREY);

    ball_bitmap = load_bitmap("ball_bitmap.bmp", NULL);

    // init semaphores

    // PHYSICS SEMAPHORE    
    pthread_mutex_init(&phys_sem, NULL); 
    pthread_mutexattr_t phys_sem_attr;    
    pthread_mutexattr_init(&phys_sem_attr);
    pthread_mutexattr_setprotocol(&phys_sem_attr, PTHREAD_PRIO_PROTECT);  
    pthread_mutexattr_setprioceiling(&phys_sem_attr, 90);

    // MOTOR VARIABLES SEMAPHORE    
    pthread_mutex_init(&motor_sem, NULL); 
    pthread_mutexattr_t motor_sem_attr;    
    pthread_mutexattr_init(&motor_sem_attr);
    pthread_mutexattr_setprotocol(&motor_sem_attr, PTHREAD_PRIO_PROTECT);  
    pthread_mutexattr_setprioceiling(&motor_sem_attr, 90);

    // VISION VARIABLES SEMAPHORE    
    pthread_mutex_init(&vision_sem, NULL); 
    pthread_mutexattr_t vision_sem_attr;    
    pthread_mutexattr_init(&vision_sem_attr);
    pthread_mutexattr_setprotocol(&vision_sem_attr, PTHREAD_PRIO_PROTECT);  
    pthread_mutexattr_setprioceiling(&vision_sem_attr, 90);


}   



void make_mouse_ball(){
    if (ball_bitmap != NULL) {
		set_mouse_sprite(ball_bitmap);
		set_mouse_sprite_focus(0, 0);
	}
	else
		printf("bitmap not found\n");
}
