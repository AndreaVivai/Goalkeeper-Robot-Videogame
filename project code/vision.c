#include <stdio.h>
#include <math.h>
#include <allegro.h>
//-------------------------
#include "utility.h"
#include "interface.h"
#include "vision.h"
#include "colors_set.h"
#include "motor.h"
#include "ball_physics.h"
#include "scan.h"
#include "graphics.h"



//---------------------------------------- 
// struct 
//----------------------------------------
struct point			current_points_tracked[MAX_VISION];	// array of all currently tracked points 
struct vision_tracker	tracked_points[MAX_VISION]; 		// memory for trackers, containing points acquired and vel and acc evaluated
struct point 			targert_centroid;					// coord. of centroid					

//---------------------------------------- 
// variables 
//----------------------------------------
int	vision_tracker_is_active[MAX_VISION]; // the corrisponding index for the trackers: 1 is tracking || 0 is NOT tracking
int vision_tracker_view[MAX_VISION][MAX_DIM_WIN][MAX_DIM_WIN]; // image buffer for trackers
int window_size;
int tracker_index_passed;
int can_draw_detection;


//---------------------------------------- 
// vision functions
//----------------------------------------


int get_image(int tracker_index, int x0, int y0) {
    int i, j; // Image matrix indexes
    int x, y; // Graphical coordinates
    int found = 0; 			// Flag to indicate if the desired color is found

    do {
        for (i = 0; i < window_size; i = i + 2) {
            for (j = 0; j < window_size; j = j + 2) {
                x = x0 - window_size / 2 + i;
                y = y0 - window_size / 2 + j;

                // Take only points in the world
                if (x > FOOTBALL_FIELD_X1 && x < FOOTBALL_FIELD_X2 && y > FOOTBALL_FIELD_Y1 && y < FOOTBALL_FIELD_Y2) {

                    vision_tracker_view[tracker_index][i][j] = getpixel(screen, x, y);
                    if (vision_tracker_view[tracker_index][i][j] == BALL_CENTER_COL) {
						found = 1; // Found a point of BALL_CENTER_COL color
                    }
                } else {
                    vision_tracker_view[tracker_index][i][j] = BACKGROUND_COL;
                    return 0;
                }
            }
        }
        if (!found) { // If the desired color is not found, enlarge the window
            if (window_size < MAX_DIM_WIN) {
                window_size = MAX_DIM_WIN; 

			}

			else return 0;	// Reached the maximum number of allowed enlargements       
        }
    } while (!found); // Repeat until a point of BALL_CENTER_COL color is found or the maximum number of enlargements is reached

    // Reduce the window size to the original dimensions
    window_size = MIN_DIM_WIN;

    return 1; // Return 1 to indicate that the desired color is found and the window has been brought back to original size
}




// Displays the image store in vision_tracker_view
void put_image() {
    int i, j; // Image matrix indexes
    int x, y; // Graphical coordinates
	
	if (can_draw_detection) {
		for (i = 0; i < window_size; i++) {
			for (j = 0; j < window_size; j++) {
					x = TRACK_WIN_CENT_X - window_size / 2 + i;
					y = TRACK_WIN_CENT_Y - window_size / 2 + j;
						putpixel(screen_buff, x, y, vision_tracker_view[tracker_index_passed][i][j]);
			}
		}
	}
	can_draw_detection = 0;
}



// Evaluate centroid of element seen by tracker_index, relative to the center of image.
void compute_centroid(int tracker_index) {

	int num_points_found = 0;			
	int i, j;

	targert_centroid.point_x = 0;
	targert_centroid.point_y = 0;
	for (i = 0; i < window_size; i++) {
		for (j = 0; j < window_size; j++) {
			if (vision_tracker_view[tracker_index][i][j] == BALL_CENTER_COL) {
				targert_centroid.point_x += i - window_size / 2; 	// update the coordinates relative to the window center
				targert_centroid.point_y += j - window_size / 2;
				num_points_found++;
			}
		}
	}
	// check if some point is seen, otherwise deactivate the tracker
	if (num_points_found != 0) {
		targert_centroid.point_x /= num_points_found;
		targert_centroid.point_y /= num_points_found;
	}
	else {
		// Assigns the value 0 to all cells of the matrix vision_tracker_view[tracker_index]
		for (int i = 0; i < window_size; i++) {
			for (int j = 0; j < window_size; j++) {
				vision_tracker_view[tracker_index][i][j] = 0;
			}
		}
	}
}



// Store last VISION_TRACKER_POINT_STORE points tracked.
void store_point(int tracker_index) {

	if (tracker_index >= MAX_VISION) return;

	int last_stored_point = tracked_points[tracker_index].top;

	// don't store same coord.
	if (current_points_tracked[tracker_index].point_x == tracked_points[tracker_index].x[last_stored_point] ||
	        current_points_tracked[tracker_index].point_y == tracked_points[tracker_index].y[last_stored_point])
		return;

	last_stored_point = (last_stored_point + 1) % VISION_TRACKER_POINT_STORE;
	tracked_points[tracker_index].x[last_stored_point] = current_points_tracked[tracker_index].point_x;
	tracked_points[tracker_index].y[last_stored_point] = Y_MAX - current_points_tracked[tracker_index].point_y;
	tracked_points[tracker_index].top = last_stored_point;

	tracked_points[tracker_index].gathered_points++;
}



float evaluate_vx(int tracker_i) {

    // Initialize sum variables for calculations
    float sum_time = 0, sum_x = 0, sum_time_x = 0, sum_time_squared = 0;
    int n = VISION_TRACKER_POINT_STORE; // Number of points to consider

    // Iterate over each point
    for (int i = 0; i < n; i++) {
        // Time since the first point in seconds, using the constant interval dt
        float time = i * dt;

        // Get the x-coordinate for the current point
        int x = tracked_points[tracker_i].x[(tracked_points[tracker_i].top + 1 + i) % VISION_TRACKER_POINT_STORE];

        // Accumulate sums required for the least squares calculation
        sum_time += time;
        sum_x += x;
        sum_time_x += time * x;
        sum_time_squared += time * time;
    }

    // Calculate the slope of the best fit line using the least squares method, which represents the horizontal velocity (v_x)
    float v_x = (n * sum_time_x - sum_time * sum_x) / (n * sum_time_squared - sum_time * sum_time);

    return v_x;
}



float evaluate_vy(int tracker_i) {
    // Initialize sum variables for calculations
    float sum_time = 0, sum_y = 0, sum_time_y = 0, sum_time_squared = 0;
    int n = VISION_TRACKER_POINT_STORE; // Number of points to consider

    // Iterate over each point
    for (int i = 0; i < n; i++) {
        // Time since the first point in seconds, using the constant interval dt
        float time = i * dt;

        // Get the y-coordinate for the current point
        int y = tracked_points[tracker_i].y[(tracked_points[tracker_i].top + 1 + i) % VISION_TRACKER_POINT_STORE];

        // Accumulate sums required for the least squares calculation
        sum_time += time;
        sum_y += y;
        sum_time_y += time * y;
        sum_time_squared += time * time;
    }

    // Calculate the slope of the best fit line using the least squares method, which represents the vertical velocity (v_y)
    float v_y = - ((n * sum_time_y - sum_time * sum_y) / (n * sum_time_squared - sum_time * sum_time));

    return v_y;
}



// Compute velocities along x and y for the tracked object.
void evaluate_v(int tracker_index, int currently_shooting_tracker) {

	// if it's too early we have not enough points
	if (tracked_points[tracker_index].gathered_points < 2) return;

	// if the ball we' re tracking hasn't shot, it means that it's not actually moving
	else if (currently_shooting_tracker == 1) {
		tracked_points[tracker_index].vx = 0;
		tracked_points[tracker_index].vy = 0;
	}

	else{
		tracked_points[tracker_index].vx = evaluate_vx(tracker_index);
		tracked_points[tracker_index].vy = evaluate_vy(tracker_index);
	}		

}



// Destroy the struct content of tracker_index.
void clear_vision_tracker_struct(int task_index, int tracker_index) {
	tracked_points[tracker_index].vx = 0;
	tracked_points[tracker_index].vy = 0;
	tracked_points[tracker_index].gathered_points = 0;

	vision_tracker_is_active[tracker_index] = 0;
	tp[task_index].index = -1;
}



void vision_init(int tracker_index) {

	tracked_points[tracker_index].vx = 0;
	tracked_points[tracker_index].vy = 0;
	tracked_points[tracker_index].gathered_points = 0;

	vision_tracker_is_active[tracker_index] = 1;

	window_size = MIN_DIM_WIN;

}



// Function to calculate the intersection between the trajectory of the ball and the y-axis (x = X_COORD_GOALKEEPER, where the goalkeeper is)
struct ww calculate_goalkeeper_y_coordinate(int index) {
	
    float vx = tracked_points[index].vx; // ball speed in the x direction
    float vy = tracked_points[index].vy; // ball speed in the y direction

	int x = current_points_tracked[index].point_x; // ball position in the x axis
	int y = current_points_tracked[index].point_y; // ball position in the y axis

	// Calculate the time it takes for the ball to reach the goalkeeper's x position
    float time_to_reach_goalkeeper_x = (X_COORD_GOALKEEPER - x) / vx;

    // Calculate the predicted y coordinate when the ball intersects the goalkeeper's x-axis
	int intersection_y = y + (int)(vy * time_to_reach_goalkeeper_x);
	
	struct ww where_when;

	where_when.when = time_to_reach_goalkeeper_x;
	where_when.where = intersection_y;

    return where_when;
}



// for each ball we spot, we use this function to track it (called once for every ball spotted)
void *vision_task(void* arg) {

	// vision variables
	int tolerance = 100;		// we need some tolerance when we decide if we stop tracking a ball
	int img_not_taken_straight = 0;
	can_draw_detection = 0;
	

	// task variables
	int task_index, tracker_index; 		// task_index: 6-7-8... we' re counting all the tasks  ||  tracker_index: 0,1,2... (more natural here)
	task_index = get_task_index(arg);
	tracker_index = task_index - VISION_TASK_MIN_INDEX;  
	tracker_index_passed = tracker_index;
	set_period_deadline(task_index);

	vision_init(tracker_index);
	
	while (!termination_signal && vision_tracker_is_active[tracker_index]) {	

		int img_is_taken = get_image(tracker_index, current_points_tracked[tracker_index].point_x, current_points_tracked[tracker_index].point_y);
		if (img_is_taken == 0 &&  img_is_taken < 10) {
			img_not_taken_straight++;
		}
		else if(img_is_taken == 1) {
			img_not_taken_straight = 0;
		}
			
		// updating the point we need to follow
		compute_centroid(tracker_index); // relative from center of tracking box
		current_points_tracked[tracker_index].point_x += targert_centroid.point_x;
		current_points_tracked[tracker_index].point_y += targert_centroid.point_y; 
		store_point(tracker_index);
		// update speed of the tracked object 
		evaluate_v(tracker_index, !mouse_status.is_placing_ball);  
		float vx = tracked_points[tracker_index].vx; 

		
		// calculate when and where it will match the goalkeeper
		struct ww when_where = calculate_goalkeeper_y_coordinate(tracker_index);
		int candidte_y_des = when_where.where;	// (desired y) where the goalkeeper will match the ball


		if (((candidte_y_des < -300 ||  candidte_y_des  >  FOOTBALL_FIELD_Y2 + 300)  &&  tolerance < 5 ) || tolerance < 0) {
			vision_tracker_is_active[tracker_index] = 0;
			clear_vision_tracker_struct(task_index, tracker_index);
			kill_task(task_index);

		}

		else if(candidte_y_des > 0   &&  candidte_y_des < FOOTBALL_FIELD_Y2  &&  vx <= -0.0001  &&   img_not_taken_straight < 9) {
			pthread_mutex_lock(&motor_sem);
			goalkeeper.y_des = candidte_y_des;
			pthread_mutex_unlock(&motor_sem);
		}

		// if it's moving
		else if(vx < -0.0001 ||  vx > 0.0001  ||  img_not_taken_straight > 9) {
			tolerance--; 
		}
		
		can_draw_detection = 1;

		deadline_miss(task_index);
		wait_for_period(task_index);
	}
	return NULL;
}



