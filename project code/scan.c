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


struct scan	scan[ARES];			// struct  of points detected


// Check if a point is yet tracked, if not proceed spawning a new vision_task.
void lock_new_target(int x_object_spotted_rel) {

    int task_index, tracker_ind; 		// task_index: 6-7-8... we' re counting all the tasks  ||  tracker_ind: 0,1,2... (more natural here)
    
    // proceed locking vision tracker on current point, if there's not yet too many 
	task_index = find_free_index(VISION_TASK_MIN_INDEX, VISION_TASK_MAX_INDEX);
    tracker_ind = task_index - VISION_TASK_MIN_INDEX;

	// check if not yet tracked
	for (int i = 0; i < MAX_VISION; i++) {

		// if inside a rectangle of any active tracker I'll consider it already monitored
		if( abs(current_points_tracked[i].point_x - scan[x_object_spotted_rel].scan_x) <= window_size / 2 &&
		    abs(current_points_tracked[i].point_y - scan[x_object_spotted_rel].scan_y) <= window_size / 2 &&
		    vision_tracker_is_active[i] == 1  &&  i != tracker_ind){

                kill_task(task_index);
                vision_tracker_is_active[tracker_ind] = 0;
                return;
        }	
	}

	if (task_index != -1) {

		current_points_tracked[tracker_ind].point_x = scan[x_object_spotted_rel].scan_x;
		current_points_tracked[tracker_ind].point_y = scan[x_object_spotted_rel].scan_y;
        
		start_task(vision_task, task_index, VISION_PER, VISION_DREL, VISION_PRI);
	}
}



// Makes a scan in direction imposed by the passed angle (in degree).
void read_sensor(int x) {
    int cord_x, cord_y, c;

    cord_x = x;
    cord_y = Y_MIN;

    do {
        c = getpixel(screen_buff, cord_x, cord_y);  
        cord_y += Y_STEP;    
    } while ((cord_y < Y_MAX) && (c != BALL_CENTER_COL));
    
    // Store absolute values
    scan[cord_x - X_MIN].scan_x = cord_x;
    scan[cord_x - X_MIN].scan_y = cord_y;

	// If no valid scan is found, set scan_y to Y_MAX
    if (cord_y >= Y_MAX)
        scan[x - X_MIN].scan_y = Y_MAX;
}


void *scan_task(void *arg) {
    int x_object_spotted_rel, x_object_spotted_abs = X_MIN, index = get_task_index(arg);
    
    // x_object_spotted_rel:  x coordinate where the scan is looking for an object (translated by X_MIN)
    // x_object_spotted_abs:  x coordinate where the scan is looking for an object (translated by X_MIN)                      
    x_object_spotted_abs = X_MIN; // Start x coordinate scan

    // Init scan detected distances
    for (int j = 0; j < ARES; j++)
        scan[j].scan_y = Y_MAX; 

    // Set all trackers inactive
    for (int j = 0; j < MAX_VISION; j++)
        vision_tracker_is_active[j] = 0;

    set_period_deadline(index);
    while (!termination_signal) {
        read_sensor(x_object_spotted_abs);
        x_object_spotted_rel = x_object_spotted_abs - X_MIN;

        if (scan[x_object_spotted_rel].scan_y < Y_MAX) {
   
            vision_tracker_is_active[x_object_spotted_rel] = 1;
            lock_new_target(x_object_spotted_rel);
        }

        x_object_spotted_abs++;
        if (x_object_spotted_abs > X_MAX)
            x_object_spotted_abs = X_MIN;

        deadline_miss(index);
        wait_for_period(index);
    }
	return NULL;
}