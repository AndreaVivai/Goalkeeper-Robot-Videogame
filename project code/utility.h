#ifndef UTIL
#define UTIL

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <allegro.h>
#include <semaphore.h>
#include <time.h>


//-----------------------------------------------------
// PUBLIC COSTANTS
//-----------------------------------------------------

//-----------------------------------
// TASKS PERIODS AND PRIORITIES
//-----------------------------------

//--------------------
// TASK
//--------------------

#define MAX_BALLS           2
#define MAX_VISION          MAX_BALLS*2 

#define GRAPHIC_TASK_INDEX       0
#define KEYBOARD_TASK_INDEX      1
#define MOUSE_TASK_INDEX         2
#define MOTOR_TASK_INDEX         3
#define BALLS_TASK_INDEX         4
#define RADAR_TASK_INDEX         5
 
#define VISION_TASK_MIN_INDEX    (RADAR_TASK_INDEX + 1)
#define VISION_TASK_MAX_INDEX    (RADAR_TASK_INDEX + MAX_VISION)

#define MAX_THREADS         VISION_TASK_MAX_INDEX


//--------------------
// GRAPHIC
//--------------------
#define SCREEN_WIDTH       1024
#define SCREEN_HEIGHT      768
#define COLOR_DEPTH        8


//--------------------
// OTHER
//--------------------
#define PI	    3.141592 // and so on


//--------------------------------------
// PERIODS AND PRIORITIES for each task
//--------------------------------------

//----------------------
// GRAPHIC 
//----------------------
#define GRAPH_PER		20			// period for graphic task (ms)
#define GRAPH_DREL		GRAPH_PER	// relative deadline for graphic task (ms)
#define GRAPH_PRI		90			// priority of graphic task


//----------------------
// MOUSE 
//----------------------
#define MOUSE_PER		20			// period for graphic task (ms)
#define MOUSE_DREL		MOUSE_PER	// relative deadline for graphic task (ms)
#define MOUSE_PRI		80			// priority of graphic task


//----------------------
// KEYBOARD
//----------------------
#define KEY_PER		    40			// period for keyboard task (ms)
#define KEY_DREL	    KEY_PER	    // relative deadline for keyboard task (ms)
#define KEY_PRI		    30			// priority of keyboard task


//----------------------
// MOTOR
//----------------------
#define MOTOR_PER		20			// period for motor task (ms)
#define MOTOR_DREL		MOTOR_PER	// relative deadline for motor task (ms)
#define MOTOR_PRI		50			// priority of motor task


//----------------------
// RADAR
//----------------------
#define RADAR_PER		5			// period for RADAR task (ms)
#define RADAR_DREL		RADAR_PER	// relative deadline for RADAR task (ms)
#define RADAR_PRI		40			// priority of RADAR task


//----------------------
// VISION
//----------------------
#define VISION_PER		5			// period for VISION task (ms)
#define VISION_DREL		VISION_PER	// relative deadline for VISION task (ms)
#define VISION_PRI		60			// priority of VISION task


//----------------------
// BALLS PHYSICS
//----------------------             
#define BALLS_PER		20			// period for ball physics task (ms)
#define BALLS_DREL		BALLS_PER	// relative deadline for ball physics task (ms)
#define BALLS_PRI		70			// priority of ball physics task


//-----------------------------------------------------
// STRUCT:
//-----------------------------------------------------

struct task_param {
    int         index;          // Task numerical index (1,2... -1 IF NOT ACTIVE)
    int         period;         // Task period in milliseconds
    int         deadline;       // Relative deadline in milliseconds
    int         priority;       // Task priority in [0, 99]
    int         dmiss;          // Number of deadline misses

    struct      timespec at;    // Next activation time
    struct      timespec dl;    // Current absolute deadline
    
    pthread_t   tid;            // Thread ID
    sem_t       tsem;           // Activation semaphore
};


//-----------------------------------------------------
// TASK VARIABLES
//-----------------------------------------------------
extern struct task_param	tp[MAX_THREADS];
extern struct timespec      start_time;           
extern int		            termination_signal;	  

//-----------------------------------------------------
// GRAPHIC VARIABLES
//-----------------------------------------------------
extern BITMAP  *screen_buff;
extern BITMAP  *ball_bitmap;	   


//-----------------------------------------------------
// SEMAPHORES
//-----------------------------------------------------
extern pthread_mutex_t phys_sem;  
extern pthread_mutex_t motor_sem;
extern pthread_mutex_t vision_sem;


//-----------------------------------------------------
//  FUNTIONS DECLARATIONS:
//-----------------------------------------------------

//-----------------------
//  TIME FUNTIONS:
//-----------------------
long    get_systime(int unit); 
void    time_copy(struct timespec *td,struct timespec ts);
void    time_add_ms(struct timespec *t, int ms);
int     time_cmp(struct timespec t1, struct timespec t2);


//-----------------------
//  TASK FUNTIONS:
//-----------------------
pthread_t start_task(void* (*task_function)(void *), int ind, int period, int drel, int priority);
void      kill_task(int ind);
int       get_task_index(void* index);
void      wait_for_activation(int i); 
int       deadline_miss(int i);
void      wait_for_period(int i);
void      set_period_deadline(int index); 
void      task_set_period(int i, int per);
void      task_set_deadline(int i, int dline);
int       task_period(int i); 
int       task_deadline(int i);
int       task_dmiss(int i); 
void      task_atime(int i, struct timespec *at); 
void      task_adline(int i, struct timespec *dl);
void      wait_for_task_end(int i);
void      task_activate(int i);
int       find_free_ball();
int       find_shooting_ball();
int       find_free_index(int min_index, int max_index);
float     time_diff_ms(struct timespec t1, struct timespec t2);


//------------------------
//  ALLEGRO FUNTIONS:
//------------------------
void get_keycodes(char *scan, char *ascii);
int  listen_scancode();
void init_graphic_mouse_keyboard();
void make_mouse_ball();

#endif