#include <stdlib.h>

#include "utility.h"
#include "interface.h"
#include "graphics.h"

// to execute:  make  ->   sudo ./goalkeeper

int main() {

    init_graphic_mouse_keyboard();
    
    // start the graphic task
    pthread_t graphic_ID = start_task(graphic_task, GRAPHIC_TASK_INDEX, GRAPH_PER, GRAPH_DREL, GRAPH_PRI);

    pthread_join(graphic_ID,NULL);

    allegro_exit();
    return 0;
}
