//---------------------------------------- 
// struct
//----------------------------------------
#define     MAX_DELAY   10


//---------------------------------------- 
// struct
//----------------------------------------

// goalkeeper struct
struct gk {

    // when they increase  the goalkeeper goes DOWN, when decrease goes UP   
    // y, y_p  have to stay inside GOALKEEPER_MIN_Y and GOALKEEPER_MAX_Y

    int   y;           // position at currrent time
    int   y_p;         // 1 dt before currrent time
    int   y_des;       // desired coordinate 
};


//---------------------------------------- 
// global variables
//----------------------------------------

void *motor_task(void* arg);
extern struct gk goalkeeper;
extern float error_distance;
extern float kp, kd;
extern int delay;            
extern float v_max_des;
extern float theta_animation;
