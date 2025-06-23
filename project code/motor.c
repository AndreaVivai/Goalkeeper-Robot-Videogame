#include <time.h>
#include <stdio.h>
#include <math.h>
#include <allegro.h>
//----------------------------------------
#include "utility.h"
#include "interface.h"
#include "ball_physics.h"
#include "motor.h"
#include "vision.h"
#include "graphics.h"

//---------------------------------------- 
// variables
//----------------------------------------

// position_y, position_y_p, position_y_pp have to stay inside GOALKEEPER_MIN_Y and GOALKEEPER_MAX_Y
struct gk goalkeeper;

// distance between the goalkeeper and its goal, and its derivatives (in discrete time)
// stored from MAX_PAST units in the past
float error_distances[MAX_DELAY]; 
float error_speeds[MAX_DELAY]; 

// VARIABILI DA FAR CONTROLLARE ALL' UTENTE
float   kp, kd;                    // proportional and derivative gain
int     delay;                     // delay of the motor response: how many times dt the system responds behind
float   v_max, v_min, v_max_des = 30;   // MAX/MIN voltage that can be given to the motor (and desired max, set by the user)

float theta_animation;  // animation motor angle from the positive x axis, positive counterclockwise



//---------------------------------------- 
// motor functions
//----------------------------------------

void init_motor() {

    // errors
    for (int i = 0 ; i < MAX_DELAY ; i++) {
        error_distances[i] = 0;
        error_speeds[i] = 0;
    }

    // motor variables
    goalkeeper.y     = 350;
    goalkeeper.y_p   = goalkeeper.y;
    goalkeeper.y_des = goalkeeper.y;

    delay = 0;  
    v_max = v_max_des;
    v_min = -v_max;
}



void check_danger_area(float next_goalkeeper_y, int danger_area, int super_danger_area) {

    // check for collision with borders: goalkeeper MIN : 231  ||  goalkeeper MAX : 541  
    if (next_goalkeeper_y < GOALKEEPER_MAX_Y && next_goalkeeper_y > GOALKEEPER_MIN_Y)
        goalkeeper.y = next_goalkeeper_y;
    else if (next_goalkeeper_y > GOALKEEPER_MAX_Y)
        goalkeeper.y = GOALKEEPER_MAX_Y;
    else
        goalkeeper.y = GOALKEEPER_MIN_Y;

    // to avoid collision with the borders, set a limit in the danger_area in the outward direction
    if        (next_goalkeeper_y > GOALKEEPER_MAX_Y - danger_area ) {
        v_max = 6;
            
    } else if (goalkeeper.y < GOALKEEPER_MIN_Y + danger_area ) {
        v_min = -6; 
    
    } else if (next_goalkeeper_y > GOALKEEPER_MAX_Y - super_danger_area ) {
        v_max = 2;

    } else if (goalkeeper.y < GOALKEEPER_MIN_Y + super_danger_area) {
        v_min = -2;

    } else {
        v_max = v_max_des;     
        v_min = -v_max;    
    }
}



void update_errors() {

    // delay_control
    if (delay > MAX_DELAY)
        delay = MAX_DELAY;
    else if(delay < 0)
        delay = 0;

    // position and speed error update (pay attention to the order)
    for (int i = MAX_DELAY-1 ; i > 0 ; i--) {
        error_distances[i] = error_distances[i-1];   
        error_speeds[i] = error_speeds[i-1];   
    }

    // new position error
    error_distances[0] = goalkeeper.y_des - goalkeeper.y;

    // new speed error
    error_speeds[0] = (error_distances[0] - error_distances[1])/dt;
}



float control_system() {

    float u = kp*error_distances[delay] + kd*error_speeds[delay]; // PD control, with simulated delay  ||  u: control signal

    // motor saturation control
    float V = u;

    pthread_mutex_lock(&motor_sem);
    if (V > v_max)
        V = v_max;

    else if (V < v_min)
        V = v_min;
    pthread_mutex_unlock(&motor_sem);

    return V;
}



void *motor_task(void* arg) {

    init_motor();

    // contol parameters
    float V[2];                 // input tension to the motor [V]:  V[0] current time  ||  V[1] previous time
    V[0] = 0;
    V[1] = 0;
    pthread_mutex_unlock(&motor_sem);
    kp = 1;                     
    kd = 0.4;      
    pthread_mutex_unlock(&motor_sem);

    int danger_area = 30;       // if the goalkeeper is here, slow down
    int super_danger_area = 5;  // if the goalkeeper is here, slow down a lot

    // motor parameters
    float R = 1;                       // 1         [pixels]         shaft radius   
    float T = 0.03;                    // 0.03      [s]              sampling time
    float tau = 0.19;                  // 0.19      [s]              motor physical constant
    float p = 0.9;                     // 0.9       []               motor physical constant
    float K = 19.23;                   // 19.23     [rad*V^-1*s^-1]  motor physical constant                
    float A = K*(T - tau + p*tau);     // 0,211530  [rad*V^-1]       motor physical constant
    float B = -K*(tau - p*T - p*tau);  // 0,153840  [rad*V^-1]       motor physical constant
    float C = p/R;                     // 0.9       [pixels^-1]      motor physical constant

	int index_motor = get_task_index(arg);
	set_period_deadline(index_motor);



    while (!termination_signal) {   

        update_errors();

        // control system
        V[1] = V[0];
        V[0] = control_system();

        // motor dynamics
        goalkeeper.y_p  = goalkeeper.y;
        float tetha_next = A*V[0] + B*V[1] + C*(goalkeeper.y - goalkeeper.y_p) + goalkeeper.y/R; // motor physicsl model 

        check_danger_area(tetha_next*R, danger_area, super_danger_area);

        // motor animation
        float theta_animation_value;

        pthread_mutex_lock(&phys_sem);
        theta_animation_value = (goalkeeper.y - GOALKEEPER_MIN_Y)*(PI/200);
        pthread_mutex_unlock(&phys_sem);

        pthread_mutex_lock(&motor_sem);
        theta_animation = theta_animation_value;
        pthread_mutex_unlock(&motor_sem);

        
        deadline_miss(index_motor);
        wait_for_period(index_motor);
    }
	return NULL;
}
