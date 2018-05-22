/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h> // needed for sleep
using namespace std;
 
/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
int FPS = 30;
int speed = 5;
const int width = 800;
const int height = 600;
int hit_pause = 0;
int fruit_x = 305;
int fruit_y = 455;
int eat_fruit = 0;
bool press_restart_fruit = false;
bool press_restart_snake = false;
bool press_restart_score = false;
bool start_game = true;
bool end_game = false;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display *display;
	int screen;
	Window window;
	GC gc[3];
	int width; // size of window
	int height;
};

/*
 * Function to put out a message on error exits.
 */
void error(string str) {
	cerr << str << endl;
	exit(0);
}

/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};

class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			if (press_restart_fruit == true) {
				x = 305;
				y = 455;
				fruit_x = x;
				fruit_y = y;
				press_restart_fruit = false;
				end_game = false;
			}
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10);
		}
		
		Fruit() {
			x = 305;
			y = 455;
		}
		
		void re_generate(){
			x = (rand() % 78) * 10 + 5; //x [5, 785]
			y = (rand() % 53) * 10 + 55; //y [55, 585]
			fruit_x = x;
			fruit_y = y;
        }

	private:
		int x;
		int y;
};

Fruit fruit;

class Start_Display : public Displayable {
	public:
		virtual void paint(XInfo& xinfo) {
			if (start_game == true) {
				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, 800, 600);

				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 100, s_1.c_str(), s_1.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 150, s_2.c_str(), s_2.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 175, s_3.c_str(), s_3.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 200, s_4.c_str(), s_4.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 225, s_5.c_str(), s_5.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 250, s_6.c_str(), s_6.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 275, s_7.c_str(), s_7.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 325, s_8.c_str(), s_8.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 350, s_9.c_str(), s_9.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 375, s_10.c_str(), s_10.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 400, s_11.c_str(), s_11.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 425, s_12.c_str(), s_12.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 450, s_13.c_str(), s_13.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 475, s_14.c_str(), s_14.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], 100, 525, s_15.c_str(), s_15.length());
			}
		}

	    Start_Display() {
			s_1 = "First name: Chen, Last name: Li, student ID: 20597755.";
			s_2 = "Welcome to this snake game.";
			s_3 = "Control the snake to eat fruits and avoid obstacles";
			s_4 = " and its own body.";
			s_5 = "The snake will grow longer when it eats a fruit.";
			s_6 = "The more fruits the snake eats, the higher score";
			s_7 = " you will receive.";
			s_8 = "Use gaps at the border to go to the other side.";
			s_9 = "Please press k/K to start the game.";
			s_10 = "Use w/a/s/d or arrows keys to control";
			s_11 = " the direction of the snake.";
			s_12 = "Use r/R to restart.";
			s_13 = "Use p/P to pause.";
			s_14 = "Use q/Q to quit the game.";
			s_15 = "Good luck and have fun!";
		}
	private:
		string s_1;
		string s_2;
		string s_3;
		string s_4;
		string s_5;
		string s_6;
		string s_7;
		string s_8;
		string s_9;
		string s_10;
		string s_11;
		string s_12;
		string s_13;
		string s_14;
		string s_15;
};

Start_Display start_display;

class End_Display : public Displayable {
	public:
		virtual void paint(XInfo& xinfo) {
			if (end_game == true) {
				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, 800, 600);
				ostringstream stream_4;
				stream_4 << eat_fruit;
				string temp = "Score: ";
				temp.append(stream_4.str());
				string ss = "Press r/R to restart.";
				string sss = "Press q/Q to quit.";
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], x, y, s.c_str(), s.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], x, y + 50, temp.c_str(), temp.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], x, y + 100, ss.c_str(), ss.length());
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[2], x, y + 150, sss.c_str(), sss.length());
			}
		}

		End_Display() {
			x = 250;
			y = 250;
			s = "Game Over!";
		}
	private:
		string s;
		int x;
		int y;
};

End_Display end_display;

class Snake : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			if (press_restart_snake == true) {
				block_list.clear();
				dir = 0;
				receive = 0;
				wait = 0;
				edge_gap = false;
				received_turn = false;
				int xx_1 = 105;
				int xx_2 = 95;
				int xx_3 = 85;
				int yy_1 = 255;
				int yy_2 = 255;
				int yy_3 = 255;
				block_list.push_back(make_pair(xx_1, yy_2));
				block_list.push_back(make_pair(xx_2, yy_2));
				block_list.push_back(make_pair(xx_3, yy_3));
				hit_pause = 0;
				press_restart_snake = false;
			}
			for (vector<pair<int, int> >::iterator it = block_list.begin(); it != block_list.end(); ++it) {
				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], (*it).first, (*it).second, 10, blockSize);
			}
		}

		void trun () {
			if ((dir == 0 && (receive == 1 || receive == 3)) || 
				(dir == 1 && (receive == 2 || receive == 0)) || 
				(dir == 2 && (receive == 1 || receive == 3)) ||
				(dir == 3 && (receive == 0 || receive == 2))) {
				dir = receive;
				received_turn = true;
			}
		}

		void update_list() {
			for (vector<pair<int, int> >::reverse_iterator it = block_list.rbegin(); it != block_list.rend() - 1; ++it) {
						(*it).first = (*(it + 1)).first;
						(*it).second = (*(it + 1)).second;
			}
		}
		
		void move(XInfo &xinfo) {
			//x [5, 785]
			//y [55, 585]
			if ((hit_pause != 0 && (hit_pause % 2)) || (end_game == true) || (start_game == true)) {return;}
			if (received_turn != true) {trun();}

			if (wait < 10 - speed) {
				wait++;
				return;
			} else {
				if (didHitObstacle(dir)) {return;}
				didHitObstacle(dir);
				didEatFruit(dir);
				wait = 0;
			}
			received_turn = false;
			//cerr << block_list.front().first << " " << block_list.front().second << endl;
		}

		bool check_regenerate() {
			//left block   [55, 465]  [65, 485]
			//middle block [195, 255] [635, 285]
			//right block  [655, 95]  [715, 125]
			if ((fruit_x >= 55 && fruit_x <= 65 && fruit_y >= 465 && fruit_y <= 485) ||
				(fruit_x >= 195 && fruit_x <= 635 && fruit_y >= 255 && fruit_y <= 285) ||
				(fruit_x >= 655 && fruit_x <= 715 && fruit_y >= 95 && fruit_y <= 125)) {
				return true;
			}

			for (vector<pair<int, int> >::iterator it = block_list.begin(); it != block_list.end(); ++it) {
				if (fruit_x == (*it).first && fruit_y == (*it).second) {
					return true;
				}
			}
			return false;
		}

		void eat_add() {
			block_list.insert(block_list.begin(), make_pair(fruit_x, fruit_y));
			eat_fruit++;
			do {
				fruit.re_generate();
			} while (check_regenerate());	
		}

		void didEatFruit(int where) {
			if (where == 0) {
				if (block_list.front().first + 10 == fruit_x && 
					block_list.front().second == fruit_y) {
					eat_add();
				} else {
					if (edge_gap == true) {
						edge_gap = false;
					} else {
						update_list();
						block_list.front().first += 10;
					}
				}	
			} else if (where == 1) {
				if (block_list.front().first == fruit_x && 
					block_list.front().second + 10 == fruit_y) {
					eat_add();
				} else {
					if (edge_gap == true) {
						edge_gap = false;
					} else {
						update_list();
						block_list.front().second += 10;
					}
				}
			} else if (where == 2) {
				if (block_list.front().first - 10 == fruit_x && 
					block_list.front().second == fruit_y) {
					eat_add();
				} else {
					if (edge_gap == true) {
						edge_gap = false;
					} else {
						update_list();
						block_list.front().first -= 10;
					}
				}
			} else if (where == 3) {
				if (block_list.front().first == fruit_x && 
					block_list.front().second - 10 == fruit_y) {
					eat_add();
				} else {
					if (edge_gap == true) {
						edge_gap = false;
					} else {
						update_list();
						block_list.front().second -= 10;
					}
				}
			}
		}

		bool didHitObstacle(int where) {
			//hit edges
			//x [5, 785]
			//y [55, 585]
			if ((where == 0 && block_list.front().first >= 785 && block_list.front().second != 355) ||
				(where == 1 && block_list.front().second >= 585 && block_list.front().first != 605) ||
				(where == 2 && block_list.front().first <= 5 && block_list.front().second != 255) ||
				(where == 3 && block_list.front().second <= 55 && block_list.front().first != 305) ||
				((where == 1 || where == 3) && block_list.front().first == 795 && block_list.front().second == 355) ||
				((where == 0 || where == 2) && block_list.front().first == 605 && block_list.front().second == 595) || 
				((where == 1 || where == 3) && block_list.front().first == 5 && block_list.front().second == 255) ||
				((where == 0 || where == 2) && block_list.front().first == 305 && block_list.front().second == 45)) {
				end_game = true;
				return true;
			}

			//hit a block
			//left block   [55, 465]  [65, 485]
			//middle block [195, 255] [635, 285]
			//right block  [655, 95]  [715, 125]
			if (where == 0 && 
				((block_list.front().first == 45 && block_list.front().second >= 465 && block_list.front().second <= 485) || 
				 (block_list.front().first == 185 && block_list.front().second >= 255 && block_list.front().second <= 285) || 
				 (block_list.front().first == 645 && block_list.front().second >= 95 && block_list.front().second <= 125))) {
				end_game = true;
				return true;
			} else if (where == 1 && 
				((block_list.front().first >= 55 && block_list.front().first <= 65 && block_list.front().second == 455) ||
				 (block_list.front().first >= 195 && block_list.front().first <= 635 && block_list.front().second == 245) ||
				 (block_list.front().first >= 655 && block_list.front().first <= 715 && block_list.front().second == 85))) {
				end_game = true;
				return true;
			} else if (where == 2 && 
				((block_list.front().first == 75 && block_list.front().second >= 465 && block_list.front().second <= 485) || 
				 (block_list.front().first == 645 && block_list.front().second >= 255 && block_list.front().second <= 285) || 
				 (block_list.front().first == 725 && block_list.front().second >= 95 && block_list.front().second <= 125))) {
				end_game = true;
				return true;
			} else if (where == 3 &&
				((block_list.front().first >= 55 && block_list.front().first <= 65 && block_list.front().second == 495) ||
				 (block_list.front().first >= 195 && block_list.front().first <= 635 && block_list.front().second == 295) ||
				 (block_list.front().first >= 655 && block_list.front().first <= 715 && block_list.front().second == 135))) {
				end_game = true;
				return true;
			}

			//hit itself
			if (where == 0) {
				for (vector<pair<int, int> >::iterator it = block_list.begin() + 1; it != block_list.end(); ++it) {
					if ((block_list.front().first + 10 == (*it).first) && (block_list.front().second == (*it).second)) {
						end_game = true;
						return true;
					}
				}
			} else if (where == 1) {
				for (vector<pair<int, int> >::iterator it = block_list.begin() + 1; it != block_list.end(); ++it) {
					if ((block_list.front().first == (*it).first) && (block_list.front().second + 10 == (*it).second)) {
						end_game = true;
						return true;
					}
				}
			} else if (where == 2) {
				for (vector<pair<int, int> >::iterator it = block_list.begin() + 1; it != block_list.end(); ++it) {
					if ((block_list.front().first - 10 == (*it).first) && (block_list.front().second == (*it).second)) {
						end_game = true;
						return true;
					}
				}
			} else if (where == 3) {
				for (vector<pair<int, int> >::iterator it = block_list.begin() + 1; it != block_list.end(); ++it) {
					if ((block_list.front().first == (*it).first) && (block_list.front().second - 10 == (*it).second)) {
						end_game = true;
						return true;
					}
				}
			}

			//gaps
			//(5, 255), (305, 45), (605, 585), (785, 355)
			if (where == 0 && block_list.front().first == 795 && block_list.front().second == 355) {
				update_list();
				edge_gap = true;
				block_list.front().first = 5;
				block_list.front().second = 255;
				return false;
			} else if (where == 1 && block_list.front().first == 605 && block_list.front().second == 595) {
				update_list();
				edge_gap = true;
				block_list.front().first = 305;
				block_list.front().second = 45;
				return false;
			} else if (where == 2 && block_list.front().first == 5 && block_list.front().second == 255) {
				update_list();
				edge_gap = true;
				block_list.front().first = 795;
				block_list.front().second = 355;
				return false;
			} else if (where == 3 && block_list.front().first == 305 && block_list.front().second == 45) {
				update_list();
				edge_gap = true;
				block_list.front().first = 605;
				block_list.front().second = 595;
				return false;
			}

			return false;
		}

		Snake(int dir, int receive): dir(dir), receive(receive) {
			blockSize = 10;
			wait = 0;
			received_turn = false;
			edge_gap = false;
			int x_1 = 105;
			int x_2 = 95;
			int x_3 = 85;
			int y_1 = 255;
			int y_2 = 255;
			int y_3 = 255;
			block_list.push_back(make_pair(x_1, y_2));
			block_list.push_back(make_pair(x_2, y_2));
			block_list.push_back(make_pair(x_3, y_3));
		}

		void change_keyboard(int re) {
			receive = re;
		}

	private:
		int blockSize;
		int dir; // 0 right; 1 down; 2 left; 3 up
		int receive; // 0 d; 1 s; 2 a; 3 w
		int wait;
		bool received_turn;
		bool edge_gap;
		std::vector< pair<int, int> > block_list;
};

class Edge : public Displayable {
	public:
		virtual void paint(XInfo& xinfo) {
			//(5, 255), (305, 55), (605, 585), (785, 355)
			XPoint points_1[] = {{0, 255}, {0, 0}, {800, 0}, {800, 355}};
			int npoints_1 = sizeof(points_1) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[1], points_1, npoints_1, CoordModeOrigin);

			XPoint points_2[] = {{0, 265}, {0, 600}, {605, 600}};
			int npoints_2 = sizeof(points_2) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[1], points_2, npoints_2, CoordModeOrigin);

			XPoint points_3[] = {{615, 600}, {800, 600}, {800, 365}};
			int npoints_3 = sizeof(points_3) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[1], points_3, npoints_3, CoordModeOrigin);

			XPoint points_4[] = {{0, 50}, {305, 50}};
			int npoints_4 = sizeof(points_4) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[1], points_4, npoints_4, CoordModeOrigin);

			XPoint points_5[] = {{315, 50}, {800, 50}};
			int npoints_5 = sizeof(points_5) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[1], points_5, npoints_5, CoordModeOrigin);

			XPoint points_6[] = {{55, 465}, {55, 495}, {75, 495}, {75, 465}, {55, 465}};
			int npoints_6 = sizeof(points_6) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[0], points_6, npoints_6, CoordModeOrigin);
			XPoint points_9[] = {{55, 465}, {75, 495}, {75, 465},{55, 495}};
			int npoints_9 = sizeof(points_6) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[0], points_9, npoints_9, CoordModeOrigin);
						
			XPoint points_7[] = {{195, 255}, {195, 295}, {645, 295}, {645, 255}, {195, 255}};
			int npoints_7 = sizeof(points_7) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[0], points_7, npoints_7, CoordModeOrigin);
			XPoint points_10[] = {{195, 255},{645, 295}, {645, 255}, {195, 295}};
			int npoints_10 = sizeof(points_10) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[0], points_10, npoints_10, CoordModeOrigin);
						
			XPoint points_8[] = {{655, 95}, {655, 135}, {725, 135}, {725, 95}, {655, 95}};
			int npoints_8 = sizeof(points_8) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[0], points_8, npoints_8, CoordModeOrigin);
			XPoint points_11[] = {{655, 95}, {725, 135}, {725, 95}, {655, 135}};
			int npoints_11 = sizeof(points_11) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[0], points_11, npoints_11, CoordModeOrigin);
		}
};

class Text : public Displayable {
	public:
		virtual void paint(XInfo& xinfo) {
			if (press_restart_score == true) {
				eat_fruit = 0;
				press_restart_score = false;
			}
			ostringstream stream_1, stream_2, stream_3;
			stream_1 << print_Speed;
			stream_2 << print_FPS;
			stream_3 << eat_fruit;
			s = "";
			s.append("Score: ");
			s.append(stream_3.str());
			s.append(" FPS: ");
			s.append(stream_2.str());
			s.append(" Speed: ");
			s.append(stream_1.str());

			XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[1], 
				x, y, s.c_str(), s.length());
		}

		Text(int x, int y): x(x), y(y) {
			s = "";
			print_Speed = 5;
			print_FPS = 30;
		}

		void set_print_direction(int d){
			print_Speed = d;
		}

		void set_print_FPS(int F){
			print_FPS = F;
		}

	private:
		int x;
		int y;
		int print_Speed;
		int print_FPS;
		string s;
};

list<Displayable *> dList; // list of Displayables
Snake snake(0, 0);

Edge edge;
Text text_line(25, 30);

/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {
	//command line input
	if (argc == 2 && atoi(argv[1]) >= 25 && atoi(argv[1]) <= 60){
		FPS = atoi(argv[1]);
		text_line.set_print_FPS(atoi(argv[1]));
	}
	if (argc == 3 && atoi(argv[1]) >= 25 && atoi(argv[1]) <= 60) {
		FPS = atoi(argv[1]);
		text_line.set_print_FPS(atoi(argv[1]));
		if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 10) {
			speed = atoi(argv[2]);
			text_line.set_print_direction(atoi(argv[2]));
		}
	}

	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xInfo.display = XOpenDisplay("");
	if (!xInfo.display) {
		error("Can't open display.");
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xInfo.window = XCreateSimpleWindow( 
		xInfo.display, // display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y, // upper left corner location
		hints.width, hints.height, // size of the window
		Border, // width of window's border
		black, // window border colour
		white); // window background colour
		
	XSetStandardProperties(
		xInfo.display, // display containing the window
		xInfo.window, // window whose properties are set
		"animation", // window's title
		"Animate", // icon's title
		None, // pixmap for the icon
		argv, argc, // applications command line args
		&hints); // size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i], 1, LineSolid, CapButt, JoinRound);

	i = 1;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i], 10, LineSolid, CapButt, JoinBevel);
	// load a larger font
	XFontStruct * font;
	font = XLoadQueryFont (xInfo.display, "12x24");
	XSetFont (xInfo.display, xInfo.gc[i], font->fid);

	i = 2;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	// load a larger font
	XFontStruct * font_2;
	font_2 = XLoadQueryFont (xInfo.display, "12x24");
	XSetFont (xInfo.display, xInfo.gc[i], font_2->fid);


	XSelectInput(xInfo.display, xInfo.window, 
		ButtonPressMask | KeyPressMask | 
		PointerMotionMask | 
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint(XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	XClearWindow( xinfo.display, xinfo.window );
	
	// get height and width of window (might have changed since last repaint)

	XWindowAttributes windowInfo;
	//XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	unsigned int height = windowInfo.height;
	unsigned int width = windowInfo.width;

	// big black rectangle to clear background
    
	// draw display list
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}
	XFlush( xinfo.display );
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, // the keyboard event
		text, // buffer when text will be written
		BufferSize, // size of the text buffer
		&key, // workstation-independent key symbol
		NULL); // pointer to a composeStatus structure (unused)
	if (i == 1) {
		printf("Got key press -- %c\n", text[0]);
		switch(text[0]) {
			case 'q':
			case 'Q':
				error("Terminating normally.");
				break;
			case 'p': //pause
			case 'P':
				hit_pause++;
				break;
			case 'r': //restart
			case 'R':
				press_restart_fruit = true;
				press_restart_snake = true;
				press_restart_score = true;
				break;
			case 'w':
			case 'W':
				snake.change_keyboard(3);
				break;
			case 'a':
			case 'A':
				snake.change_keyboard(2);
				break;
			case 's':
			case 'S':
				snake.change_keyboard(1);
				break;
			case 'd':
			case 'D':
				snake.change_keyboard(0);
				break;
			case 'k':
			case 'K':
				start_game = false;
				break;
		}
	}
	switch(key) {
			case XK_Up:
				snake.change_keyboard(3);
				break;
			case XK_Down:
				snake.change_keyboard(1);
				break;
			case XK_Left:
				snake.change_keyboard(2);
				break;
			case XK_Right:
				snake.change_keyboard(0);
				break;
	}
}

void handleAnimation(XInfo &xinfo, int inside) {
	snake.move(xinfo);
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	dList.push_front(&start_display);
	dList.push_front(&end_display);
	dList.push_front(&edge);
	dList.push_front(&fruit);
	dList.push_front(&snake);
	dList.push_front(&text_line);
	
	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while(true) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent(xinfo.display, &event);
			//cout << "event.type=" << event.type << "\n";
			switch(event.type) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
			}
		}

		unsigned long end = now();
		if (end - lastRepaint > 1000000 / FPS) {
			handleAnimation(xinfo, inside);
			repaint(xinfo);
			lastRepaint = now();
		}

		// IMPORTANT: sleep for a bit to let other processes work
		if (XPending(xinfo.display) == 0) {
			usleep(1000000 / FPS - (end - lastRepaint));
		}
	}
}

/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main (int argc, char *argv[]) {
	XInfo xInfo;
	initX(argc, argv, xInfo);
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
