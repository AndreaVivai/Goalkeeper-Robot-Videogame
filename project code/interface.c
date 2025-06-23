#include <time.h>
#include <stdio.h>
#include <math.h>
#include <allegro.h>
#include <semaphore.h>
//----------------------------------------
#include "utility.h"
#include "interface.h"
#include "ball_physics.h"
#include "colors_set.h"
#include "motor.h"
#include "vision.h"
#include "scan.h"
#include "graphics.h"

//---------------------------------------- 
// graphic variables
//----------------------------------------
BITMAP	*background_buff;           // buffer for the static part of the scenario, in which we draw every time. NON TOCCARE
BITMAP	*backup_background_buff;    // used as a backup, when a ball is drawn in the background
int     need_refresh_backgrund;     // 0 : don't need a new background   ||  1 : need to place a ball in the background ||  2: go back to basic background
int     shot_position_x, shot_position_y; // when it's needed to place the ball, we memorize the position here

//----------------------------------------
int crazy_wind_active = 0;

//---------------------------------------- 
// mouse variables
//----------------------------------------
struct mouse    mouse_status;

//--------------------------------------
//  Function prototypes
//--------------------------------------
void draw_shot_arrow(int x1, int y1, int x2, int y2, int color);

//--------------------------------------
//  Mouse functions
//--------------------------------------

void draw_shot_arrow(int x1, int y1, int x2, int y2, int color) {
    const float arrow_head_angle = PI / 6; // Angle for the arrowhead
    const int arrow_head_length = 20;      // Length of the arrowhead
    const int arrow_thickness = 2;         // Thickness of the arrow

    // Calculate the angle of the arrow
    float angle = atan2(y2 - y1, x2 - x1);

    // Add translation to start the arrow from the center of the ball
    x1 += ball_bitmap->w / 2;
    y1 += ball_bitmap->h / 2;

    // Draw the thicker main line of the arrow
    float offset_angle = angle + PI / 2; // compute the angle of the perpendicular to the line
    for (int i = 0; i <= 2*arrow_thickness; i++) {
        // Calculate the offset perpendicular to the arrow direction 
        int offset_x = cos(offset_angle) * (i-arrow_thickness);
        int offset_y = sin(offset_angle) * (i-arrow_thickness);
        line(screen_buff, x1 + offset_x , y1 + offset_y , x2 + offset_x , y2 + offset_y , color);
    }

    // Calculate points for the arrowhead
    int arrow_x1 = x2 - arrow_head_length * cos(angle - arrow_head_angle);
    int arrow_y1 = y2 - arrow_head_length * sin(angle - arrow_head_angle);
    int arrow_x2 = x2 - arrow_head_length * cos(angle + arrow_head_angle);
    int arrow_y2 = y2 - arrow_head_length * sin(angle + arrow_head_angle);

    // Draw the "filled" arrowhead
    triangle(screen_buff, x2, y2, arrow_x1, arrow_y1, arrow_x2, arrow_y2, color);
}



void display_mouse_icon() {
    if(mouse_status.icon_choosen != mouse_status.icon_displayed) {
        if(mouse_status.icon_choosen == 0)
            make_mouse_ball();
        else
            show_mouse(NULL);
    }
}



void choose_mouse_icon() {

    if(mouse_status.icon_choosen != mouse_status.icon_displayed) {
        if(mouse_status.icon_choosen == 0){
            make_mouse_ball();
            mouse_status.icon_displayed = 0;
            show_mouse(screen);
        }
        else{
            show_mouse(NULL);
            mouse_status.icon_displayed = 1;
        }
    }
}



void check_mouse_in_out_field() {

    int mouse_max_x, mouse_max_y, mouse_min_x, mouse_min_y, offset = 10;

    // calculate mouse field coordinates
    mouse_max_x = FOOTBALL_FIELD_X2 - (ball_bitmap->w);
    mouse_min_x = FOOTBALL_FIELD_X1 + (ball_bitmap->w)/2 + offset;

    mouse_max_y = FOOTBALL_FIELD_Y2 - (ball_bitmap->h);
    mouse_min_y = FOOTBALL_FIELD_Y1 + (ball_bitmap->h)/6;

    // check if the mouse has to be displayed
    if (mouse_status.position_x < mouse_min_x || mouse_status.position_x > mouse_max_x || 
         mouse_status.position_y < mouse_min_y || mouse_status.position_y > mouse_max_y ) {
            mouse_status.wanna_be_displayed = 0;

            // abort current shot
            for (int ind = 0; ind<MAX_BALLS; ind++) {
                if (ball_status[ind].activation == 2) {
                    ball_status[ind].activation = 0;
                }
            }
        }

    else{
        mouse_status.wanna_be_displayed = 1;
    }
}



void display_mouse_in_out_field() {

    // if the ball comes out/in
    if(mouse_status.wanna_be_displayed != mouse_status.is_displayed) {

        // OUTSIDE -> INSIDE
        if(mouse_status.wanna_be_displayed == 1) {
            mouse_status.is_displayed = 1;
            show_mouse(screen);
        }

        // INTSIDE -> OUTSIDE
        else {
            mouse_status.is_displayed = 0;
            need_refresh_backgrund = 2;
            show_mouse(NULL);

            // if the ball goes out I don't want it to shot
            mouse_status.is_placing_ball = 1;
            
            //HAS TO BE FINISHED     
        }
    }
}



void init_mouse_variables() {
    mouse_status.wanna_be_displayed = 1;
    mouse_status.is_displayed = 1;
    mouse_status.is_placing_ball = 1;
    mouse_status.left_button_currently_pressed = 0;
    mouse_status.left_button_previously_pressed = 0;
    mouse_status.icon_choosen = 0;
    mouse_status.icon_displayed = 1;
}



void choose_mouse_current_action() {

    mouse_status.left_button_previously_pressed = mouse_status.left_button_currently_pressed; // update the status

    // check if the button is beign pressed
    mouse_status.left_button_currently_pressed = 0; 
    if (mouse_b & 1) 
        mouse_status.left_button_currently_pressed = 1; 

    // to chage the action (place the ball / choose shot intensity), check if we stop pressing the left mouse button: 
    // this complication solves the problem of holded click
    // also check if the ball is inside the field: shot from outside the field are not allowed

   if ((mouse_status.left_button_previously_pressed > mouse_status.left_button_currently_pressed) &&  mouse_status.wanna_be_displayed==1) {
        int free_index = find_free_ball(), shooting_index = find_shooting_ball();

        // STOP: choose shot intensity
        if (mouse_status.is_placing_ball == 1 && free_index>=0 && shooting_index == -1) {
            
            // shot data update
            pthread_mutex_lock(&phys_sem);
            ball_status[free_index].activation = 2; 
            ball_status[free_index].position_x = mouse_status.position_x;  // we choose this shot position
            ball_status[free_index].position_y = mouse_status.position_y;  // we choose this shot position
            pthread_mutex_unlock(&phys_sem);

            // drawing 
            mouse_status.is_placing_ball = 0;
            need_refresh_backgrund = 1;
            shot_position_x = mouse_status.position_x;
            shot_position_y = mouse_status.position_y;
            show_mouse(NULL);

        }   

        // SHOT: choose new shot position
        else if(mouse_status.is_placing_ball == 0 && shooting_index>=0) {
               
            float prop = 0.03;
            float speedx = prop*(mouse_x - shot_position_x);
            float speedy = prop*(mouse_y - shot_position_y);

            // shot data update
            pthread_mutex_lock(&phys_sem);
            ball_status[shooting_index].speed_x = speedx;
            ball_status[shooting_index].speed_y = speedy;
            ball_status[shooting_index].activation = 1; 
            pthread_mutex_unlock(&phys_sem);

            show_mouse(NULL);
            position_mouse(700,400);

            // drawing 
            mouse_status.is_placing_ball = 1;
            need_refresh_backgrund = 2;
            //show_mouse(screen);
            shoots_cout++;
        }
    }
}
    


//--------------------------------------
//  Periodic threads functions
//--------------------------------------

void *mouse_task(void* arg) {

    init_mouse_variables();

    int mouse_index = get_task_index(arg);
    set_period_deadline(mouse_index); // set period

    // periodic task
    while(!termination_signal) { 

        mouse_status.position_x = mouse_x;
        mouse_status.position_y = mouse_y;

        check_mouse_in_out_field();
        display_mouse_in_out_field();

        choose_mouse_current_action();
        choose_mouse_icon(); 

        deadline_miss(mouse_index);
        wait_for_period(mouse_index);
    }
	return NULL;
}




void *keyboard_task(void* arg) {
	int keyboar_scan;
	int index_keyboard = get_task_index(arg);

	set_period_deadline(index_keyboard);
	while (!termination_signal) {
		keyboar_scan = listen_scancode();
		// if (keyboar_scan != 0) printf("Readed keyscan: %d\n", (int)keyboar_scan);

		switch (keyboar_scan) {

    //--------------------------------------------------//
    // ----------------- Wind setting ----------------- //

		case KEY_UP: // increase wind speed velocities
			if (wind_speed < WIND_SPEED_MAX) {
                pthread_mutex_lock(&phys_sem);
				wind_speed++;
                pthread_mutex_unlock(&phys_sem);
            }
            break;

		case KEY_DOWN: // reduce wind speed velocities
			if (wind_speed > WIND_SPEED_MIN) {
                pthread_mutex_lock(&phys_sem);
				wind_speed--;
                pthread_mutex_unlock(&phys_sem);
            }
            break;

		case KEY_LEFT:
			if (wind_angle_deg > 0 && wind_angle_deg < 360) {
                pthread_mutex_lock(&phys_sem);
				wind_angle_deg++;
                pthread_mutex_unlock(&phys_sem);

            } 
            else {
                pthread_mutex_lock(&phys_sem);
                wind_angle_deg = 0;
                wind_angle_deg++;
                pthread_mutex_unlock(&phys_sem);
            }
			break;

		case KEY_RIGHT: 
			if (wind_angle_deg > 0 && wind_angle_deg < 360) {
                pthread_mutex_lock(&phys_sem);
				wind_angle_deg--;
                pthread_mutex_unlock(&phys_sem);
            } 
            else { 
                pthread_mutex_lock(&phys_sem);
                wind_angle_deg = 360;
                wind_angle_deg--;
                pthread_mutex_unlock(&phys_sem);
            }
			break;

        case KEY_SPACE:
            
            if(crazy_wind_active == 1) {
                pthread_mutex_lock(&phys_sem);
                crazy_wind_active = 0;
                pthread_mutex_unlock(&phys_sem);
            }
            else {
                pthread_mutex_lock(&phys_sem);
                crazy_wind_active = 1;
                pthread_mutex_unlock(&phys_sem);
            } 
            break;
    //---------------------------------------------------//
    // ----------------- Motor setting ----------------- //
        case KEY_A: 
			if (kp > 0.1) {
                pthread_mutex_lock(&motor_sem);
				kp = kp - 0.2;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_D: 
			if (kp < 4.9) {
                pthread_mutex_lock(&motor_sem);
				kp = kp + 0.2;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_S: 
			if (kd > 0.1) {
                pthread_mutex_lock(&motor_sem);
				kd = kd - 0.2;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_W: 
			if (kd < 4.9) {
                pthread_mutex_lock(&motor_sem);
				kd = kd + 0.2;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_J: 
			if (delay > 0.1) { 
                pthread_mutex_lock(&motor_sem);
				delay--;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_L: 
			if (delay < 10) {
                pthread_mutex_lock(&motor_sem);
				delay++;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_K: 
			if (v_max_des > 2) {
                pthread_mutex_lock(&motor_sem);
				v_max_des--;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_I: 
			if (v_max_des < 60) {
                pthread_mutex_lock(&motor_sem);
				v_max_des++;
                pthread_mutex_unlock(&motor_sem);
            }
			break;

        case KEY_ESC:
			termination_signal = 1;
			break;
		}
        deadline_miss(index_keyboard);
		wait_for_period(index_keyboard);
	}
	return NULL;
}

