//----------------------------------------------------------------
//  library costants
//----------------------------------------------------------------
#define WIND_SPEED_MAX      20
#define WIND_SPEED_MIN      0

//----------------------------------------------------------------
//  library struct
//----------------------------------------------------------------

struct ball {

    int position_x;
    int position_y;

    float speed_x;
    float speed_y;

    float acceleration_x;
    float acceleration_y;     

    int activation;  // 0 : not shot  |  1 : currently moving  |  2 : placed, ready to be shot
};

//----------------------------------------------------------------
//  library variables
//----------------------------------------------------------------
extern struct ball  ball_status[MAX_BALLS];
//-------------------------

extern int goals_count;        
extern int shoots_cout;
extern int goalkeeper_saves_count;
extern int posts_count;       
//-------------------------

extern int wind_speed;      
extern int wind_angle_deg; 
//-------------------------

extern float dt;  // time unit for our ball physics


//----------------------------------------------------------------
//  library functions
//----------------------------------------------------------------
void *balls_task(void* arg);