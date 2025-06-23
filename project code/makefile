CC = gcc
CFLAGS = -Wall 
LIBS = -lm -lrt -pthread

all: GOALKEEPER

GOALKEEPER: main.o interface.o utility.o ball_physics.o motor.o vision.o scan.o graphics.o
	$(CC) $(CFLAGS) -o GOALKEEPER main.o interface.o utility.o ball_physics.o motor.o vision.o scan.o graphics.o `allegro-config --libs` $(LIBS)

main.o: main.c utility.h interface.h ball_physics.h ball_physics.h vision.h motor.h graphics.h
	$(CC) $(CFLAGS) -c main.c

interface.o: interface.c utility.h interface.h ball_physics.h ball_physics.h vision.h motor.h colors_set.h graphics.h
	$(CC) $(CFLAGS) -c interface.c 

utility.o: utility.c utility.h interface.h ball_physics.h vision.h motor.h colors_set.h graphics.h
	$(CC) $(CFLAGS) -c utility.c

ball_physics.o: ball_physics.c utility.h interface.h ball_physics.h motor.h vision.h colors_set.h graphics.h
	$(CC) $(CFLAGS) -c ball_physics.c

motor.o: motor.c motor.h utility.h interface.h ball_physics.h vision.h colors_set.h graphics.h
	$(CC) $(CFLAGS) -c motor.c

vision.o: vision.c vision.h utility.h interface.h ball_physics.h motor.h colors_set.h graphics.h
	$(CC) $(CFLAGS) -c vision.c

scan.o: scan.c scan.h utility.h interface.h ball_physics.h motor.h vision.h colors_set.h graphics.h
	$(CC) $(CFLAGS) -c scan.c

graphics.o: graphics.c graphics.h scan.h utility.h interface.h ball_physics.h motor.h vision.h colors_set.h 
	$(CC) $(CFLAGS) -c graphics.c

clean:
	rm -f *.o GOALKEEPER
