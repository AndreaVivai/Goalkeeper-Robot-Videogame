//----------------------------------------------------------------
//  Functions dedicated to graphics, keyboard and mouse control
//----------------------------------------------------------------
void draw_shot_arrow(int x1, int y1, int x2, int y2, int color);
void *keyboard_task(void* arg);
void *mouse_task(void* arg);


//----------------------------------------------------------------
//  struct
//----------------------------------------------------------------

struct mouse {

    int position_x;
    int position_y;   

    int icon_choosen;           // 0 : ball   |  1 : arrow
    int icon_displayed;         // 0 : ball   |  1 : arrow

    int is_displayed;           // 0 : not displayed   |  1 : displayed
    int wanna_be_displayed;     // 0 : not displayed   |  1 : displayed

    int is_placing_ball;        // 0 : we are loading the shot speed  |  1 : we are placing the shot position
    
    int left_button_currently_pressed;     // 0 : left button is not pressed     |  1 : left button is pressed
    int left_button_previously_pressed;    // 0 : left button was not pressed in the previous instant     
                                           // 1 : left button was pressed in the previous instant 
};


//----------------------------------------------
//  global variables
//----------------------------------------------
extern struct mouse mouse_status;

extern int new_placement_allowed;
extern int new_shot_allowed;
extern int need_refresh_backgrund;
extern int shot_position_x, shot_position_y;
extern int crazy_wind_active;

extern BITMAP	*background_buff; 
extern BITMAP	*backup_background_buff;
