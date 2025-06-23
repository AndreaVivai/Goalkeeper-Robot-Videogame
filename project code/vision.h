//-----------------------------------------------------
// TRACKER PARAMETERS
//-----------------------------------------------------
#define VISION_TRACKER_POINT_STORE		6		// number of points stored by the vision tracker

#define MAX_DIM_WIN					135
#define MIN_DIM_WIN					75

//-----------------------------------------------------
// STRUCT
//-----------------------------------------------------

struct point {
	int point_x;
	int point_y;
};


// used to return where the ball will touch the goalkeeper, and when
struct ww {
	int 	where;
	float 	when;
};


struct vision_tracker {

	// circular buffer for last VISION_POINT_STORE points acquired
	int top;		                       		 	// index of the current point element
	
	int x[VISION_TRACKER_POINT_STORE];	            // array for x coordinates (world coord)
	int y[VISION_TRACKER_POINT_STORE];	            // array for y coordinates (world coord)

	int gathered_points;	                  		// number of points in this circular buffer (matters only for first steps)
	
	// estimated speeds 
	float vx;  
	float vy; 

	int y_interseption;	// y coordinates in which the ball would collide with the goalkeeper (world coord)
	int t_interseption; // time at witch the collision happens
};



//-----------------------------------------------------
// PUBLIC VARIABLES
//-----------------------------------------------------
extern struct point	current_points_tracked[MAX_VISION];
extern int	vision_tracker_is_active[MAX_VISION];
extern int window_size;
extern int tracker_index_passed;


//-----------------------------------------------------
// PUBLIC FUNCTION
//----------------------------------------------------
void *vision_task(void* arg);
void put_image();
