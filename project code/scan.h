
//-----------------------------------------------------
// RADAR SENSOR PARAMETERS
//-----------------------------------------------------
#define RPOSX		5							// scan positions
#define RPOSY		5
#define Y_MIN		30							// min and max sensor distance (px)
#define Y_MAX		730
#define Y_STEP		1							// sensor resolution (px)
#define X_MIN		10							// min and max x coord for scan scanning (px)
#define X_MAX		700	
#define ARES		(X_MAX - X_MIN)


//-----------------------------------------------------
// STRUCT
//-----------------------------------------------------

struct scan {	// point acquired by scanner, with relative distances from vertex
	int scan_x;		// position detected
	int scan_y;
};



//-----------------------------------------------------
// PUBLIC FUNCTION
//-----------------------------------------------------
void *scan_task(void* arg);
