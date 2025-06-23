#include <time.h>
#include <stdio.h>
#include <math.h>
#include <allegro.h>
#include <pthread.h>
//----------------------------------------
#include "utility.h"
#include "interface.h"
#include "ball_physics.h"
#include "motor.h"
#include "graphics.h"


//---------------------------------------- 
// variables
//----------------------------------------

struct ball     ball_status[MAX_BALLS];
//----------------------------

int goals_count = 0;        
int shoots_cout = 0;        
int goalkeeper_saves_count = 0;
int posts_count = 0;

//----------------------------

int wind_speed = WIND_SPEED_MIN;      
int wind_angle_deg = 0;  

//----------------------------
float dt; // interval time between 2 instant of the system


//---------------------------------------- 
// balls functions
//----------------------------------------

void init_ball(int index) { 

    pthread_mutex_lock(&phys_sem);
    ball_status[index].speed_x = 0; 
    ball_status[index].speed_y = 0; 
    ball_status[index].acceleration_x = 0; 
    ball_status[index].acceleration_y = 0; 
    ball_status[index].position_x = 0; 
    ball_status[index].position_y= 0; 
    ball_status[index].activation= 0; 
    pthread_mutex_unlock(&phys_sem);
}


void update_acceleration(int index) {

    float wind_angle_rad = wind_angle_deg * (PI/180);
    float vx; 
    float vy;

    float gamma = 0.001; // proportionality constant between the wind speed and the acceleration
    float beta = 0.003;
    
    pthread_mutex_lock(&phys_sem);

    vx = ball_status[index].speed_x; 
    vy = ball_status[index].speed_y;

    // supposed a simple linear relationship
    ball_status[index].acceleration_x =  beta*wind_speed*cos(wind_angle_rad) - gamma*vx;
    ball_status[index].acceleration_y = -beta*wind_speed*sin(wind_angle_rad) - gamma*vy;
    
    pthread_mutex_unlock(&phys_sem);
}



void update_speed(int index) {

    float vx; 
    float vy;
    float ax;
    float ay;

    pthread_mutex_lock(&phys_sem);

    vx = ball_status[index].speed_x; 
    vy = ball_status[index].speed_y;
    ax = ball_status[index].acceleration_x;
    ay = ball_status[index].acceleration_y;

    vx = vx + ax * dt ;
    vy = vy + ay * dt ;

    ball_status[index].speed_x = vx; 
    ball_status[index].speed_y = vy;

    pthread_mutex_unlock(&phys_sem);
}



void update_position(int index) {

    int px; 
    int py;
    float vx; 
    float vy;

    pthread_mutex_lock(&phys_sem);

    px = ball_status[index].position_x; 
    py = ball_status[index].position_y;
    vx = ball_status[index].speed_x; 
    vy = ball_status[index].speed_y;

    px = px + vx * dt ;
    py = py + vy * dt ;

    ball_status[index].position_x = px; 
    ball_status[index].position_y = py;

    pthread_mutex_unlock(&phys_sem);
}



void check_goal_or_save(int index, int ball_min_x) {

    int y_ball = ball_status[index].position_y; 

    // goalkeeper reach
    int goalkeeper_reach_ymax;
    int goalkeeper_reach_ymin;

     // goalkeeper reach
    pthread_mutex_lock(&phys_sem);
    goalkeeper_reach_ymax = goalkeeper.y + HALF_GOALKEEPER + ball_bitmap->h/4;
    goalkeeper_reach_ymin = goalkeeper.y - HALF_GOALKEEPER - ball_bitmap->h/4;
    pthread_mutex_unlock(&phys_sem);

    // posts reach
    int post_up_reach_ymax = POST_UP_Y + POST_RADIUS + ball_bitmap->h/4;;
    int post_up_reach_ymin = POST_UP_Y - POST_RADIUS - ball_bitmap->h/4; ;

    int post_down_reach_ymax = POST_DOWN_Y + POST_RADIUS + ball_bitmap->h/4;;
    int post_down_reach_ymin = POST_DOWN_Y - POST_RADIUS - ball_bitmap->h/4;;

    if (ball_status[index].position_x <= ball_min_x) {

        // if the shot donesn' t hit the post (AND it's inside) AND if the shot donesn' t hit the goalkeeper
        if ((y_ball < post_down_reach_ymin && y_ball > post_up_reach_ymax) &&       
            (y_ball > goalkeeper_reach_ymax || y_ball < goalkeeper_reach_ymin)) {   
            
            goals_count++; // Goal scored 
            init_ball(index);
        }
            
        // if the shot hit the post
        else if ( (y_ball > post_down_reach_ymin && y_ball < post_down_reach_ymax) ||  
                  (y_ball < post_up_reach_ymax   && y_ball > post_up_reach_ymin)) {
            
            posts_count++;

            pthread_mutex_lock(&phys_sem);
            // post collision
            ball_status[index].speed_x = -ball_status[index].speed_x + 0.6; // 0.4 to prevent from stopping on the post
            ball_status[index].position_x = ball_status[index].position_x;
            pthread_mutex_unlock(&phys_sem);

        // if the shot hit the goalkeeper 
        } else if (y_ball < goalkeeper_reach_ymax && y_ball > goalkeeper_reach_ymin) {
            
            goalkeeper_saves_count++; 

            pthread_mutex_lock(&phys_sem);
            // goalkeeper collision
            ball_status[index].speed_x = -ball_status[index].speed_x + 0.8; // 0.4 to prevent from stopping on the post
            ball_status[index].position_x = ball_status[index].position_x + 15;
            pthread_mutex_unlock(&phys_sem);
        } 

        else{
            init_ball(index);
        }
    }
}



void check_if_active(int index) { 

    int ball_max_x, ball_max_y, ball_min_x, ball_min_y;

    // calculate ball field coordinates
    ball_max_x = FOOTBALL_FIELD_X2 - (ball_bitmap->w);
    ball_min_x = FOOTBALL_FIELD_X1 + (ball_bitmap->w)/2;

    ball_max_y = FOOTBALL_FIELD_Y2 - (ball_bitmap->h);
    ball_min_y = FOOTBALL_FIELD_Y1 + (ball_bitmap->h)/6;

    // check if the ball is outside the field 
    if(( ball_status[index].position_x >= ball_max_x  || 
         ball_status[index].position_y <= ball_min_y  || ball_status[index].position_y >= ball_max_y ) &&
         index != find_shooting_ball() &&  ball_status[index].activation == 1) {
            init_ball(index);
    }

    else if (ball_status[index].position_x <= ball_min_x  &&  index != find_shooting_ball()  &&  ball_status[index].activation == 1) {     
            check_goal_or_save(index, ball_min_x);      
    }
}




void *balls_task(void* arg) {

    for (int ind = 0; ind < MAX_BALLS ; ind++) {
        init_ball(ind);
    }

    int balls_task_index = get_task_index(arg);
    set_period_deadline(balls_task_index); 

    struct timespec t_prec , t_now;
    clock_gettime(CLOCK_MONOTONIC, &t_now);

    // periodic task
    while(!termination_signal) {

        t_prec = t_now;
        clock_gettime(CLOCK_MONOTONIC, &t_now);
        dt = time_diff_ms(t_now,t_prec)/20;


        for (int ball_index = 0; ball_index<MAX_BALLS; ball_index++) {
            check_if_active(ball_index);

            if(ball_status[ball_index].activation == 1) {

                update_acceleration(ball_index);
                update_speed(ball_index);
                update_position(ball_index);
            }
        }
        deadline_miss(balls_task_index);
        wait_for_period(balls_task_index);
    }
	return NULL;
}
