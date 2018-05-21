# CS 349 Assignment 1: Snake game

Description of the game:<br />
Control the snake to eat fruits and avoid obstacles and its own body.<br />
The snake will grow longer when it eats a fruit.<br />
The more fruits the snake eats, the higher score you will receive.<br />

Start game:<br />
Use "make run" to compile and run snake.cpp<br />
Or use "make" to compile snake.cpp, and use "./snake" to run the game.<br />

Command-line parameters:<br />
"./snake xx"<br />
Use 1 parameter to input frame-rate(in range of [25, 60]), default game speed is 5.<br />

"./snake xx xx"<br />
Use 2 parameters to input frame-rate and game speed.<br />
First one should be frame-rate(in range of [25, 60]).<br />
Second one should be game speed(in range of [1, 10]).<br />

The controls:<br />
Press k/K to start the game at the splash screen.<br />
Use w/W/a/A/s/S/d/D or arrows keys to control the direction of the snake.<br />
Use r/R to restart.<br />
Use p/P to pause.<br />
Use q/Q to quit the game.<br />

The enhancements:<br />
1.Use texture graphics for the background, the snake, and other game objects. (10 marks)<br />

3.Creative level design. A classic level is a bounded square region with no obstacles inside. You can enhance this by adding obstacles that the snake must avoid and gaps on the border that lets the snake wrap around to other side of the screen.(10 marks)<br />

The development environment:<br />
macOS High Sierra version 10.13.4<br />
Ubuntu 16.04.4 LTS<br />
gcc (Ubuntu 5.5.0-12ubuntu1~16.04) 5.5.0 20171010<br />
Sublime 3.1.1 (BUILD 3176)