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

//--------------------------------------
//  Static background functions
//--------------------------------------

void draw_football_field() {
    rectfill(background_buff, FOOTBALL_FIELD_X1, FOOTBALL_FIELD_Y1, FOOTBALL_FIELD_X2, FOOTBALL_FIELD_Y2, FOOTBALL_FIELD_COL);
	rect(background_buff, FOOTBALL_FIELD_X1, FOOTBALL_FIELD_Y1, FOOTBALL_FIELD_X2, FOOTBALL_FIELD_Y2, LINES_FOOTBALL_FIELD_COL);
    rect(background_buff, PENALTY_AREA_X1, PENALTY_AREA_Y1, PENALTY_AREA_X2, PENALTY_AREA_Y2, LINES_FOOTBALL_FIELD_COL);
    circlefill(background_buff, POST_UP_X, POST_UP_Y, POST_RADIUS, POST_COL);
    circlefill(background_buff, POST_DOWN_X, POST_DOWN_Y, POST_RADIUS, POST_COL);
}



void draw_main_menu() {
    char str[50]; // for text outputs
    rect(background_buff, MAIN_MENU_X1, MAIN_MENU_Y1, MAIN_MENU_X2, MAIN_MENU_Y2, BORDER_MENU_COL);
    sprintf(str, "Legend:");
    textout_ex(background_buff, font, str, LEGEND_TITLE_X,  LEGEND_TITLE_Y, LEGENDA_COL, -1);
    sprintf(str, "ESC           --> Terminate program");
    textout_ex(background_buff, font, str, LEGEND_X1,  LEGEND_Y1, LEGENDA_COL, -1);
    sprintf(str, "UP/DOWN       --> +/- wind speed");
    textout_ex(background_buff, font, str, LEGEND_X2,  LEGEND_Y2, LEGENDA_COL, -1);
    sprintf(str, "LEFT/RIGHT    --> +/- wind angle");
    textout_ex(background_buff, font, str, LEGEND_X3,  LEGEND_Y3, LEGENDA_COL, -1);
    sprintf(str, "D/A           --> +/- Kp engine");
    textout_ex(background_buff, font, str, LEGEND_X4,  LEGEND_Y4, LEGENDA_COL, -1);
    sprintf(str, "W/S           --> +/- Kd engine");
    textout_ex(background_buff, font, str, LEGEND_X5,  LEGEND_Y5, LEGENDA_COL, -1);
    sprintf(str, "L/J           --> +/- Delay engine");
    textout_ex(background_buff, font, str, LEGEND_X6,  LEGEND_Y6, LEGENDA_COL, -1);
    sprintf(str, "I/K           --> +/- Max speed engine");
    textout_ex(background_buff, font, str, LEGEND_X7,  LEGEND_Y7, LEGENDA_COL, -1);
    sprintf(str, "SPACE         --> 0/1 Crazy wind");
    textout_ex(background_buff, font, str, LEGEND_X10,  LEGEND_Y10, LEGENDA_COL, -1);
}



void draw_track_static_menu() {
    rect(background_buff, TRACK_MENU_X1, TRACK_MENU_Y1, TRACK_MENU_X2, TRACK_MENU_Y2, BORDER_MENU_COL);
}



void draw_track_dinamin_menu() {
    char str[50]; // for text outputs
    sprintf(str, "Tracking window");
	textout_ex(screen_buff, font, str, TRACK_TITLE_X, TRACK_TITLE_Y, TRACK_TEXT_COL, -1);
    rectfill(screen_buff, TRACK_WIN_X1, TRACK_WIN_Y1, TRACK_WIN_X2, TRACK_WIN_Y2, BACKGROUND_COL);
    sprintf(str, "Searching");
	textout_ex(screen_buff, font, str, TRACK_TEXT_X1, TRACK_TEXT_Y1, TRACK_TEXT_COL, -1);
    sprintf(str, "balls...");
	textout_ex(screen_buff, font, str, TRACK_TEXT_X2, TRACK_TEXT_Y2, TRACK_TEXT_COL, -1);
    put_image();
}



void draw_dmiss_static_menu() {
    char str[50]; // for text outputs
    sprintf(str, "D_miss count:");
	textout_ex(background_buff, font, str, DMISS_TITLE_X, DMISS_TITLE_Y, DMISS_TEXT_COL, -1);
    rect(background_buff, DDLM_MENU_X1, DDLM_MENU_Y1, DDLM_MENU_X2, DDLM_MENU_Y2, BORDER_MENU_COL);
}



void draw_dmiss_dinamic_menu() {
    char str[50]; // for text outputs
    sprintf(str, "Graphic task:  %d", tp[GRAPHIC_TASK_INDEX].dmiss);
	textout_ex(background_buff, font, str, DMISS_GRAPH_X, DMISS_GRAPH_Y, DMISS_TEXT_COL, -1);
    sprintf(str, "Keyboard task: %d", tp[KEYBOARD_TASK_INDEX].dmiss);
	textout_ex(background_buff, font, str, DMISS_KEYBRD_X, DMISS_KEYBRD_Y, DMISS_TEXT_COL, -1);
    sprintf(str, "Mouse task:    %d", tp[MOUSE_TASK_INDEX].dmiss);
	textout_ex(background_buff, font, str, DMISS_MOUSE_X, DMISS_MOUSE_Y, DMISS_TEXT_COL, -1);
    sprintf(str, "Motor task:    %d", tp[MOTOR_TASK_INDEX].dmiss);
	textout_ex(background_buff, font, str, DMISS_MOTOR_X, DMISS_MOTOR_Y, DMISS_TEXT_COL, -1);
    sprintf(str, "Ball task:     %d", tp[BALLS_TASK_INDEX].dmiss);
	textout_ex(background_buff, font, str, DMISS_BALLS_X, DMISS_BALLS_Y, DMISS_TEXT_COL, -1);
    sprintf(str, "Radar task:    %d", tp[RADAR_TASK_INDEX].dmiss);
	textout_ex(background_buff, font, str, DMISS_RADAR_X, DMISS_RADAR_Y, DMISS_TEXT_COL, -1);
}



void draw_motor_static_menu() {
    char str[50]; // for text outputs
    rect(background_buff, MOTOR_MENU_X1, MOTOR_MENU_Y1, MOTOR_MENU_X2, MOTOR_MENU_Y2, BORDER_MENU_COL);
    sprintf(str, "Motor setting:");
	textout_ex(background_buff, font, str, MOTOR_TITLE_X, MOTOR_TITLE_Y, MOTOR_TEXT_COL, -1);

    // Draw the engine
    circle(background_buff, ENGINE_DRAWING_X, ENGINE_DRAWING_Y, ENG_DRAW_RADIUS_1, MOTOR_DRAWING_COL);
    circlefill(background_buff, ENGINE_DRAWING_X, ENGINE_DRAWING_Y, ENG_DRAW_RADIUS_2, MOTOR_DRAWING_COL);
    sprintf(str, "MOTOR SHAFT");
	textout_centre_ex(background_buff, font, str, MOTOR_TIT_DRAWING_X, MOTOR_TIT_DRAWING_Y, MOTOR_TEXT_COL, -1);
}



void draw_wind_static_menu() {
    char str[50]; // for text outputs
    rect(background_buff, WIND_MENU_X1, WIND_MENU_Y1, WIND_MENU_X2, WIND_MENU_Y2, BORDER_MENU_COL);
    sprintf(str, "Wind setting:");
	textout_ex(background_buff, font, str, WIND_TITLE_X, WIND_TITLE_Y, TEXT_TITL_COL, -1);

    

    // Draw the wind rose
    circle(background_buff, WIND_ROSE_X, WIND_ROSE_Y, WIND_ROSE_RADIUS_1, TEXT_TITL_COL);
    circle(background_buff, WIND_ROSE_X, WIND_ROSE_Y, WIND_ROSE_RADIUS_2, TEXT_TITL_COL);
    sprintf(str, "N");
	textout_ex(background_buff, font, str, NORD_TEXT_X, NORD_TEXT_Y, TEXT_TITL_COL, -1);
    sprintf(str, "E");
	textout_ex(background_buff, font, str, EST_TEXT_X, EST_TEXT_Y, TEXT_TITL_COL, -1);
    sprintf(str, "O");
	textout_ex(background_buff, font, str, OVEST_TEXT_X, OVEST_TEXT_Y, TEXT_TITL_COL, -1);
    sprintf(str, "S");
	textout_ex(background_buff, font, str, SUD_TEXT_X, SUD_TEXT_Y, TEXT_TITL_COL, -1);
}


 
void draw_score_static_menu() {
    char str[50]; // for text outputs
    rect(background_buff, SCORE_MENU_X1, SCORE_MENU_Y1, SCORE_MENU_X2, SCORE_MENU_Y2, BORDER_MENU_COL);
    sprintf(str, "Score:");
	textout_ex(background_buff, font, str, SCORE_TITLE_X, SCORE_TITLE_Y, SCORE_TEXT_COL, -1);

    sprintf(str, "Goals count     Saves count");
	textout_centre_ex(background_buff, font, str, SCORE_X1, SCORE_Y1, SCORE_TEXT_COL, -1);
    sprintf(str, "  Post count     Shoots count");
	textout_centre_ex(background_buff, font, str, SCORE_X3, SCORE_Y3, SCORE_TEXT_COL, -1);
}



void init_background_variables() {
    background_buff = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    clear_bitmap(background_buff);
    clear_to_color(background_buff, BACKGROUND_COL);

    backup_background_buff = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    clear_bitmap(backup_background_buff);
    clear_to_color(backup_background_buff, BACKGROUND_COL);

    need_refresh_backgrund = 0;
}



void draw_static_background() {
    init_background_variables();
    draw_football_field();
    draw_wind_static_menu();
    draw_motor_static_menu();
    draw_score_static_menu();
    draw_track_static_menu();
    draw_dmiss_static_menu();
    draw_main_menu();
    blit(background_buff, backup_background_buff, 0, 0, 0, 0, screen_buff->w, screen_buff->h);
    blit(background_buff, screen_buff, 0, 0, 0, 0, screen_buff->w, screen_buff->h);
    blit(screen_buff, screen, 0, 0, 0, 0, screen_buff->w, screen_buff->h);
}



//--------------------------------------
//  Dinamic scene functions
//--------------------------------------


void draw_motor_setting() {
    char str[50]; // for text outputs
    sprintf(str, "Kd: %.2f", kd);
	textout_ex(screen_buff, font, str, MOTOR_KD_X, MOTOR_KD_Y, MOTOR_TEXT_COL, -1);
    sprintf(str, "Kp: %.2f", kp);
	textout_ex(screen_buff, font, str, MOTOR_KP_X, MOTOR_KP_Y, MOTOR_TEXT_COL, -1);
    sprintf(str, "Delay: %d", delay);
	textout_ex(screen_buff, font, str, MOTOR_DELAY_X, MOTOR_DELAY_Y, MOTOR_TEXT_COL, -1);
    sprintf(str, "Max speed: %.1f", v_max_des);
	textout_ex(screen_buff, font, str, MOTOR_MAX_SPEED_X, MOTOR_MAX_SPEED_Y, MOTOR_TEXT_COL, -1);
}



void draw_arrow_wing_motor() {
    // Calculates the direction vector from the tail of the arrow to the tip of the arrow
    float motor_angle_rad_abs = fmodf(theta_animation, 2.0f * PI);
    float dx = cos(motor_angle_rad_abs);
    float dy = sin(motor_angle_rad_abs);

    // Calculates the coordinates of the arrowhead
    int x1 = ARROW_TAIL_MOTOR_X + ARROW_MOTOR_LENGTH * dx;
    int y1 = ARROW_TAIL_MOTOR_Y + ARROW_MOTOR_LENGTH * dy;
    
    // Calculates the points for the wings
    int left_Wing_X = x1 - ARROW_WING_MOTOR_LNG * dx - ARROW_WING_MOTOR_WDT * dy;
    int left_Wing_Y = y1 - ARROW_WING_MOTOR_LNG * dy + ARROW_WING_MOTOR_WDT * dx;
    int right_Wing_X = x1 - ARROW_WING_MOTOR_LNG * dx + ARROW_WING_MOTOR_WDT * dy;
    int right_Wing_Y = y1 - ARROW_WING_MOTOR_LNG * dy - ARROW_WING_MOTOR_WDT * dx;
    
    // Draw arrow
    line(screen_buff, ARROW_TAIL_MOTOR_X, ARROW_TAIL_MOTOR_Y, x1, y1, MOTOR_DRAWING_COL);   // Arrow               
    line(screen_buff, x1, y1, left_Wing_X, left_Wing_Y, MOTOR_DRAWING_COL);                 // Left wing 
    line(screen_buff, x1, y1, right_Wing_X, right_Wing_Y, MOTOR_DRAWING_COL);               // Right wing 
}



void draw_arrow_wing_wind() {
    // Calculates the direction vector from the tail of the arrow to the tip of the arrow
    float wind_angle_rad = -wind_angle_deg * PI / 180.0;
    float dx = cos(wind_angle_rad);
    float dy = sin(wind_angle_rad);
    
    // Calculates the coordinates of the arrowhead
    int x1 = ARROW_TAIL_WIND_X + ARROW_WIND_LENGTH * dx;
    int y1 = ARROW_TAIL_WIND_Y + ARROW_WIND_LENGTH * dy;
    
    // Calculates the points for the wings
    int left_Wing_X = x1 - ARROW_WING_WIND_LNG * dx - ARROW_WING_WIND_WDT * dy;
    int left_Wing_Y = y1 - ARROW_WING_WIND_LNG * dy + ARROW_WING_WIND_WDT * dx;
    int right_Wing_X = x1 - ARROW_WING_WIND_LNG * dx + ARROW_WING_WIND_WDT * dy;
    int right_Wing_Y = y1 - ARROW_WING_WIND_LNG * dy - ARROW_WING_WIND_WDT * dx;
    
    // Draw arrow
    line(screen_buff, ARROW_TAIL_WIND_X, ARROW_TAIL_WIND_Y, x1, y1, TEXT_TITL_COL);   // Arrow               
    line(screen_buff, x1, y1, left_Wing_X, left_Wing_Y, TEXT_TITL_COL);     // Left wing 
    line(screen_buff, x1, y1, right_Wing_X, right_Wing_Y, TEXT_TITL_COL);   // Right wing 
}



void draw_increased_score() {
    char str[50]; // for text outputs
    sprintf(str, "%d               %d", goals_count, goalkeeper_saves_count);
	textout_centre_ex(screen_buff, font, str, SCORE_X2, SCORE_Y2, SCORE_TEXT_COL, -1);
    sprintf(str, "%d               %d", posts_count, shoots_cout);
	textout_centre_ex(screen_buff, font, str, SCORE_X4, SCORE_Y4, SCORE_TEXT_COL, -1);
}



void draw_goalkeeper() {

	pthread_mutex_lock(&motor_sem);
    rectfill(screen_buff, GOALKEEPER_X1, goalkeeper.y - HALF_GOALKEEPER, GOALKEEPER_X2, goalkeeper.y + HALF_GOALKEEPER, GOALKEEPER_COL);
    rect(screen_buff, GOALKEEPER_X1, goalkeeper.y - HALF_GOALKEEPER, GOALKEEPER_X2, goalkeeper.y + HALF_GOALKEEPER, GOALKEEPER_COL_EXT);
	pthread_mutex_unlock(&motor_sem);
}



void draw_wind_change() {
    char str[50]; // for text outputs
    sprintf(str, "Direction: %d°", wind_angle_deg);
	textout_ex(screen_buff, font, str, DIRECTION_TEXT_X, DIRECTION_TEXT_Y, TEXT_TITL_COL, -1);
    sprintf(str, "Speed: %d", wind_speed);
	textout_ex(screen_buff, font, str, SPEED_WIND_TEXT_X, SPEED_WIND_TEXT_Y, TEXT_TITL_COL, -1);
    if(crazy_wind_active == 1) {
        sprintf(str, "Crazy wind: active");
	    textout_ex(screen_buff, font, str, CRAZY_WIND_TEXT_X, CRAZY_WIND_TEXT_Y, TEXT_TITL_COL, -1);
    }
    else {
        sprintf(str, "Crazy wind: not active");
	    textout_ex(screen_buff, font, str, CRAZY_WIND_TEXT_X, CRAZY_WIND_TEXT_Y, TEXT_TITL_COL, -1);
    }
}



void crazy_wind(int crazy_wind_active) {

    if (crazy_wind_active) {

        wind_angle_deg++;
        wind_speed = WIND_SPEED_MAX;
    
        if(wind_angle_deg > 270 || wind_angle_deg < 50)
            wind_angle_deg = 50;        
    }
}



void draw_shot_balls() {
    
    for (int ball_index = 0; ball_index<MAX_BALLS; ball_index++) {

        if (ball_status[ball_index].activation == 1) {

            int coordinate_ball_center_x = ball_status[ball_index].position_x - (ball_bitmap->w)/2;
            int coordinate_ball_center_y = ball_status[ball_index].position_y - (ball_bitmap->h)/2;
            blit(ball_bitmap, screen_buff, 0, 0, coordinate_ball_center_x, coordinate_ball_center_y, ball_bitmap->w, ball_bitmap->h);
        }
    }
}



void place_ball_to_shoot() {

    // if we just clicked to place the ball to load a shoot
    if (need_refresh_backgrund > 0) {

        // CHOOSEN THE PLACE: load the shoot
        if (need_refresh_backgrund == 2) {
            blit(backup_background_buff, background_buff, 0, 0, 0, 0, backup_background_buff->w, backup_background_buff->h);
            blit(background_buff, screen_buff, 0, 0, 0, 0, screen_buff->w, screen_buff->h); 
        }

        // SHOT: choose new shooting place
        else{
            blit(ball_bitmap, background_buff, 0, 0, mouse_status.position_x, mouse_status.position_y, ball_bitmap->w, ball_bitmap->h);   
            blit(background_buff, screen_buff, 0, 0, 0, 0, screen_buff->w, screen_buff->h); 
        }
        need_refresh_backgrund = 0;
    }
}



void draw_dinamic_elemets() {

    // initialize screen_buf to background_buff, then we draw on
    blit(background_buff, screen_buff, 0, 0, 0, 0, screen_buff->w, screen_buff->h);
    
    place_ball_to_shoot();
    draw_shot_balls();
    draw_goalkeeper();
    draw_increased_score();
    draw_arrow_wing_wind();
    draw_wind_change();
    crazy_wind(crazy_wind_active);
    draw_motor_setting();
    draw_arrow_wing_motor();
    draw_track_dinamin_menu();
    draw_dmiss_dinamic_menu();

    if (mouse_status.is_placing_ball == 0)
        draw_shot_arrow(shot_position_x, shot_position_y, mouse_status.position_x, mouse_status.position_y, SHOOT_ARROW_COL);
    
    // update screen with screen_buff, without covering the mouse
    scare_mouse();
    blit(screen_buff, screen, 0, 0, 0, 0, screen_buff->w, screen_buff->h);
    unscare_mouse();
}



void program_start() {
    clock_gettime(CLOCK_MONOTONIC, &start_time);  // initialize time
    start_task(keyboard_task,  KEYBOARD_TASK_INDEX,  KEY_PER, KEY_DREL, KEY_PRI);
    start_task(mouse_task,     MOUSE_TASK_INDEX,     MOUSE_PER,  MOUSE_DREL,   MOUSE_PER);
    start_task(balls_task,     BALLS_TASK_INDEX,     BALLS_PER,  BALLS_DREL,   BALLS_PRI);
    start_task(motor_task,     MOTOR_TASK_INDEX,     MOTOR_PER,  MOTOR_DREL,   MOTOR_PRI);
    start_task(scan_task,     RADAR_TASK_INDEX,     RADAR_PER,  RADAR_DREL,   RADAR_PRI);
}



void *graphic_task(void* arg) {

    // Create a basic background: background_buff. 
    // In every cycle, we initialize screen_buf to background_buff, then we draw on

    draw_static_background(); 
    program_start();  // start every other task

    int graphic_index = get_task_index(arg);
    set_period_deadline(graphic_index); // set period

	pthread_mutex_lock(&phys_sem);
    goalkeeper.y = 400;
	pthread_mutex_unlock(&phys_sem);
    
    // periodic task
    while(!termination_signal) { 

        draw_dinamic_elemets();

        deadline_miss(graphic_index);
        wait_for_period(graphic_index);
    }
	return NULL;
}



