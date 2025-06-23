//-----------------------------------
// SCREEN DIMENSIONS
//-----------------------------------
#define MARGIN				    5
#define MENU_BOX_WIDTH		    319
#define MENU_BOX_HEIGHT		    758
#define FOOTBALL_FIELD_WIDTH	695         
#define FOOTBALL_FIELD_HEIGHT   758
//-----------------------------------
#define MENU_BOX_X1             MARGIN + FOOTBALL_FIELD_WIDTH
#define MENU_BOX_Y1		        MARGIN 
#define MENU_BOX_X2		        SCREEN_WIDTH - MARGIN
#define MENU_BOX_Y2		        SCREEN_HEIGHT - MARGIN
//-----------------------------------
#define FOOTBALL_FIELD_X1	    MARGIN
#define FOOTBALL_FIELD_Y1	    MARGIN
#define FOOTBALL_FIELD_X2	    FOOTBALL_FIELD_WIDTH + MARGIN
#define FOOTBALL_FIELD_Y2	    FOOTBALL_FIELD_HEIGHT + MARGIN

//-----------------------------------
// FOOTBALL FIELD 
//-----------------------------------
#define PENALTY_AREA_X1     MARGIN
#define PENALTY_AREA_Y1     MARGIN + 81
#define PENALTY_AREA_X2     MARGIN + 200
#define PENALTY_AREA_Y2     FOOTBALL_FIELD_Y2 - 81
//-----------------------------------

// post up: above on the screen (smaller y coordinate)   ||   post down: below on the screen (bigger y coordinate) 
#define POST_UP_X           MARGIN + POST_RADIUS
#define POST_UP_Y           PENALTY_AREA_Y1 + 100
#define POST_DOWN_X         POST_UP_X
#define POST_DOWN_Y         POST_UP_Y + 400
#define POST_RADIUS         5

//-----------------------------------
#define HALF_GOALKEEPER     40
#define GOALKEEPER_X1       MARGIN
#define GOALKEEPER_X2       MARGIN + 10
#define GOALKEEPER_MIN_Y    POST_UP_Y + HALF_GOALKEEPER + POST_RADIUS
#define GOALKEEPER_MAX_Y    POST_DOWN_Y - HALF_GOALKEEPER - POST_RADIUS
#define X_COORD_GOALKEEPER  (GOALKEEPER_X1 + GOALKEEPER_X2)/2

//-----------------------------------
// MAIN MENU POSITIONS
//-----------------------------------
#define MAIN_MENU_X1        MENU_BOX_X1
#define MAIN_MENU_Y1        MENU_BOX_Y1
#define MAIN_MENU_X2        MENU_BOX_X2
#define MAIN_MENU_Y2        MAIN_MENU_Y1 + 235
//-----------------------------------
#define LEGEND_TITLE_X      MENU_BOX_X1 + 10
#define LEGEND_TITLE_Y      MENU_BOX_Y1 + 10
//-----------------------------------
#define LEGEND_X1           LEGEND_TITLE_X 
#define LEGEND_Y1           LEGEND_TITLE_Y + 20
#define LEGEND_X2           LEGEND_X1
#define LEGEND_Y2           LEGEND_Y1 + 20
#define LEGEND_X3           LEGEND_X1
#define LEGEND_Y3           LEGEND_Y2 + 20
#define LEGEND_X4           LEGEND_X1
#define LEGEND_Y4           LEGEND_Y3 + 20
#define LEGEND_X5           LEGEND_X1
#define LEGEND_Y5           LEGEND_Y4 + 20
#define LEGEND_X6           LEGEND_X1
#define LEGEND_Y6           LEGEND_Y5 + 20
#define LEGEND_X7           LEGEND_X1
#define LEGEND_Y7           LEGEND_Y6 + 20
#define LEGEND_X10          LEGEND_X1
#define LEGEND_Y10          LEGEND_Y7 + 20
#define LEGEND_X8           LEGEND_X1 + 160
#define LEGEND_Y8           LEGEND_Y10 + 40
#define LEGEND_X9           MAIN_MENU_X2 - 5
#define LEGEND_Y9           LEGEND_Y8 + 20
//-----------------------------------
// TRACKING WINDOW DRAWING
//-----------------------------------
#define TRACK_MENU_X1       MAIN_MENU_X1
#define TRACK_MENU_Y1       MAIN_MENU_Y2
#define TRACK_MENU_X2       MAIN_MENU_X1 + 150
#define TRACK_MENU_Y2       MAIN_MENU_Y2 + 160
//-----------------------------------
#define TRACK_WIN_X1        TRACK_MENU_X1 + 5
#define TRACK_WIN_Y1        TRACK_MENU_Y1 + 25
#define TRACK_WIN_X2        TRACK_MENU_X2 - 5
#define TRACK_WIN_Y2        TRACK_MENU_Y2 - 5
#define TRACK_WIN_CENT_X    TRACK_MENU_X1 + 75
#define TRACK_WIN_CENT_Y    TRACK_MENU_Y1 + 90
//-----------------------------------
#define TRACK_TITLE_X       TRACK_MENU_X1 + 5
#define TRACK_TITLE_Y       TRACK_MENU_Y1 + 10
#define TRACK_TEXT_X1       TRACK_WIN_CENT_X - 36
#define TRACK_TEXT_Y1       TRACK_WIN_CENT_Y - 10
#define TRACK_TEXT_X2       TRACK_TEXT_X1
#define TRACK_TEXT_Y2       TRACK_TEXT_Y1 + 10
//-----------------------------------
// DEADLINE MISS MENU POSITIONS
//-----------------------------------
#define DDLM_MENU_X1        TRACK_MENU_X2
#define DDLM_MENU_Y1        MAIN_MENU_Y2
#define DDLM_MENU_X2        MAIN_MENU_X2
#define DDLM_MENU_Y2        TRACK_MENU_Y2
//-----------------------------------
#define DMISS_TITLE_X       DDLM_MENU_X1 + 10
#define DMISS_TITLE_Y       DDLM_MENU_Y1 + 10
#define DMISS_GRAPH_X       DMISS_TITLE_X + 5
#define DMISS_GRAPH_Y       DMISS_TITLE_Y + 20
#define DMISS_KEYBRD_X      DMISS_GRAPH_X 
#define DMISS_KEYBRD_Y      DMISS_GRAPH_Y + 20
#define DMISS_MOUSE_X       DMISS_KEYBRD_X 
#define DMISS_MOUSE_Y       DMISS_KEYBRD_Y + 20
#define DMISS_MOTOR_X       DMISS_MOUSE_X 
#define DMISS_MOTOR_Y       DMISS_MOUSE_Y + 20
#define DMISS_BALLS_X       DMISS_MOTOR_X 
#define DMISS_BALLS_Y       DMISS_MOTOR_Y + 20
#define DMISS_RADAR_X       DMISS_BALLS_X 
#define DMISS_RADAR_Y       DMISS_BALLS_Y + 20
//-----------------------------------
// MOTOR MENU POSITIONS
//-----------------------------------
#define MOTOR_MENU_X1       TRACK_MENU_X1
#define MOTOR_MENU_Y1       DDLM_MENU_Y2
#define MOTOR_MENU_X2       DDLM_MENU_X2
#define MOTOR_MENU_Y2       MOTOR_MENU_Y1 + 130
//-----------------------------------
#define MOTOR_TITLE_X       MOTOR_MENU_X1 + 10
#define MOTOR_TITLE_Y       MOTOR_MENU_Y1 + 10
#define MOTOR_KD_X          MOTOR_TITLE_X
#define MOTOR_KD_Y          MOTOR_TITLE_Y + 30
#define MOTOR_KP_X          MOTOR_TITLE_X
#define MOTOR_KP_Y          MOTOR_KD_Y + 15
#define MOTOR_DELAY_X       MOTOR_TITLE_X
#define MOTOR_DELAY_Y       MOTOR_KP_Y + 15
#define MOTOR_MAX_SPEED_X   MOTOR_TITLE_X
#define MOTOR_MAX_SPEED_Y   MOTOR_DELAY_Y + 15
#define MOTOR_RPM_X         MOTOR_TITLE_X
#define MOTOR_RPM_Y         MOTOR_MAX_SPEED_Y + 15
//-----------------------------------
#define MOTOR_TIT_DRAWING_X     MOTOR_MENU_X2 - 70
#define MOTOR_TIT_DRAWING_Y     MOTOR_TITLE_Y 
#define ENGINE_DRAWING_X        MOTOR_TIT_DRAWING_X
#define ENGINE_DRAWING_Y        MOTOR_TIT_DRAWING_Y + 60
#define ENG_DRAW_RADIUS_1       40
#define ENG_DRAW_RADIUS_2       10
#define ARROW_TAIL_MOTOR_X      ENGINE_DRAWING_X
#define ARROW_TAIL_MOTOR_Y      ENGINE_DRAWING_Y
#define ARROW_MOTOR_LENGTH      ENG_DRAW_RADIUS_1
#define ARROW_WING_MOTOR_LNG    10
#define ARROW_WING_MOTOR_WDT    5
//-----------------------------------
// WIND MENU POSITIONS
//-----------------------------------
#define WIND_MENU_X1	    MOTOR_MENU_X1
#define WIND_MENU_Y1	    MOTOR_MENU_Y2
#define WIND_MENU_X2	    MOTOR_MENU_X2
#define WIND_MENU_Y2	    WIND_MENU_Y1 + 130
//-----------------------------------
#define WIND_TITLE_X        WIND_MENU_X1 + 10 
#define WIND_TITLE_Y        WIND_MENU_Y1 + 10
#define SPEED_WIND_TEXT_X   WIND_TITLE_X 
#define SPEED_WIND_TEXT_Y   WIND_TITLE_Y + 40 
#define DIRECTION_TEXT_X    SPEED_WIND_TEXT_X
#define DIRECTION_TEXT_Y    SPEED_WIND_TEXT_Y + 15
#define CRAZY_WIND_TEXT_X   DIRECTION_TEXT_X
#define CRAZY_WIND_TEXT_Y   DIRECTION_TEXT_Y + 15
//-----------------------------------
#define WIND_ROSE_X             WIND_MENU_X2 - 70
#define WIND_ROSE_Y             WIND_MENU_Y2 - 65
#define WIND_ROSE_RADIUS_1      40
#define WIND_ROSE_RADIUS_2      10
#define NORD_TEXT_X             WIND_ROSE_X - 5
#define NORD_TEXT_Y             WIND_ROSE_Y - WIND_ROSE_RADIUS_1 - 10
#define EST_TEXT_X              WIND_ROSE_X + WIND_ROSE_RADIUS_1 + 4
#define EST_TEXT_Y              WIND_ROSE_Y - 4
#define OVEST_TEXT_X            WIND_ROSE_X - WIND_ROSE_RADIUS_1 - 10
#define OVEST_TEXT_Y            WIND_ROSE_Y - 5
#define SUD_TEXT_X              WIND_ROSE_X - 3
#define SUD_TEXT_Y              WIND_ROSE_Y + WIND_ROSE_RADIUS_1 + 5
#define ARROW_TAIL_WIND_X       WIND_ROSE_X
#define ARROW_TAIL_WIND_Y       WIND_ROSE_Y
#define ARROW_WIND_LENGTH       WIND_ROSE_RADIUS_1
#define ARROW_WING_WIND_LNG     10
#define ARROW_WING_WIND_WDT     5
//-----------------------------------
// SCORE MENU POSITIONS
//-----------------------------------
#define SCORE_MENU_X1       MENU_BOX_X1
#define SCORE_MENU_Y1       WIND_MENU_Y2
#define SCORE_MENU_X2       MENU_BOX_X2
#define SCORE_MENU_Y2       FOOTBALL_FIELD_Y2
//-----------------------------------
#define SCORE_TITLE_X       SCORE_MENU_X1 + 10
#define SCORE_TITLE_Y       SCORE_MENU_Y1 + 15
//-----------------------------------
#define SCORE_X1            SCORE_MENU_X1 + 190
#define SCORE_Y1            SCORE_TITLE_Y 
#define SCORE_X2            SCORE_X1
#define SCORE_Y2            SCORE_Y1 + 20
#define SCORE_X3            SCORE_X1
#define SCORE_Y3            SCORE_Y2 + 30
#define SCORE_X4            SCORE_X1
#define SCORE_Y4            SCORE_Y3 + 20

//-----------------------------------
void *graphic_task(void* arg);

//-----------------------------------
