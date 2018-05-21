# CS 349 Assignment 1: Snake game

Description of the game:
Control the snake to eat fruits and avoid obstacles and its own body.
The snake will grow longer when it eats a fruit.
The more fruits the snake eats, the higher score you will receive.


Start game:
Use "make run" to compile and run snake.cpp
Or use "make" to compile snake.cpp, and use "./snake" to run the game.


Command-line parameters:
"./snake xx"
Use 1 parameter to input frame-rate(in range of [25, 60]), default game speed is 5.

"./snake xx xx"
Use 2 parameters to input frame-rate and game speed.
First one should be frame-rate(in range of [25, 60]).
Second one should be game speed(in range of [1, 10]).


The controls:
Press k/K to start the game at the splash screen.
Use w/W/a/A/s/S/d/D or arrows keys to control the direction of the snake.
Use r/R to restart.
Use p/P to pause.
Use q/Q to quit the game.


The enhancements:
1.Use texture graphics for the background, the snake, and other game objects. (10 marks)

3.Creative level design. A classic level is a bounded square region with no obstacles inside. You can enhance this by adding obstacles that the snake must avoid and gaps on the border that lets the snake wrap around to other side of the screen.(10 marks)

The development environment:
macOS High Sierra version 10.13.4
Ubuntu 16.04.4 LTS
gcc (Ubuntu 5.5.0-12ubuntu1~16.04) 5.5.0 20171010, ##Sublime 3.1.1????????????