## Goalkeeper Robot Videogame

## Click to see the YouTube video
[![YouTube](./video_presentation.png)](https://www.youtube.com/watch?v=CQSC1y-ab6w)



# Project description

A real-time, multithreaded 2D simulation game written in C, developed for a university course on Real-Time Systems.
The player controls a robotic goalkeeper trying to block a ball launched from random positions.
The project demonstrates threading, synchronization, low-level graphics, and input handling.

Technologies Used:
 - C (C99)
 - Allegro 4.2
 - POSIX threads (pthreads)

## Project Structure:
 main.c           -> Main game loop and initialization
 ball_physics.*   -> Ball trajectory simulation
 graphics.*       -> Drawing functions and bitmap handling
 vision.*         -> Basic ball detection (simulation)
 interface.*      -> User input and UI
 motor.*          -> Motor control simulation (goalkeeper movement)
 utility.*        -> Helper utilities
 scan.*           -> Scanning logic (field awareness)
 colors_set.h     -> Color definitions
 makefile         -> Build script

## How to play:

 Prerequisites:
 - Linux OS
 - GCC compiler
 - Allegro 4 development libraries

In order to install Allegro 4:
```bash
sudo apt-get install liballegro4-dev
```

To run the game:
```C
sudo ./GOALKEEPER
```

controls:
Use the mouse to move the ball.
Click once to choose the shot location
Click a second time to shoot

## Authors:
 - Andrea Vivai - GitHub: https://github.com/AndreaVivai
 - Enea Dragoni - GitHub: https://github.com/EneaDrago

