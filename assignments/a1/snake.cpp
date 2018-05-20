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
bool end_game = false;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;
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
		// ** ADD YOUR LOGIC **
		// generate the x and y value for the fruit
			x = 305;
			y = 455;
		}
		
		void re_generate(){
			x = (rand() % 78) * 10 + 5; //x [5, 785]
			y = (rand() % 53) * 10 + 55; //y [55, 585]
			fruit_x = x;
			fruit_y = y;
        }

        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */
	private:
		int x;
		int y;
};

Fruit fruit;


class End_Display : public Displayable {
	public:
		virtual void paint(XInfo& xinfo) {
			if (end_game == true) {
				ostringstream stream_4;
				stream_4 << eat_fruit;
				s = "";
				s.append("Score: ");
				s.append(stream_4.str());
				s.append(". Press r/R to restart.");
				XDrawImageString(xinfo.display, xinfo.window, xinfo.gc[1], 
					x, y, s.c_str(), s.length());
			}
		}

		End_Display() {
			x = 210;
			y = 290;
			s = "";
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
			if (hit_pause != 0 && (hit_pause % 2)) {return;}
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
			// ** ADD YOUR LOGIC **
			// Here, you will be performing collision detection between the snake, 
			// the fruit, and the obstacles depending on what the snake lands on.
		}

		 /* ** ADD YOUR LOGIC **
		 * Use these placeholder methods as guidance for implementing the snake behaviour.
		 * You do not have to use these methods, feel free to implement your own.*/
		bool check_regenerate() {
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
					update_list();
					block_list.front().first += 10;
				}	
			} else if (where == 1) {
				if (block_list.front().first == fruit_x && 
					block_list.front().second + 10 == fruit_y) {
					eat_add();
				} else {
					update_list();
					block_list.front().second += 10;
				}
			} else if (where == 2) {
				if (block_list.front().first - 10 == fruit_x && 
					block_list.front().second == fruit_y) {
					eat_add();
				} else {
					update_list();
					block_list.front().first -= 10;
				}
			} else if (where == 3) {
				if (block_list.front().first == fruit_x && 
					block_list.front().second - 10 == fruit_y) {
					eat_add();
				} else {
					update_list();
					block_list.front().second -= 10;
				}
			}
		}

		bool didHitObstacle(int where) {
			//x [5, 785]
			//y [55, 585]
			if ((where == 0 && block_list.front().first >= 785) ||
				(where == 1 && block_list.front().second >= 585) ||
				(where == 2 && block_list.front().first <= 5) ||
				(where == 3 && block_list.front().second <= 55)) {
				end_game = true;
				return true;
			}
			return false;
        }

		Snake(int dir, int receive): dir(dir), receive(receive) {
			blockSize = 10;
			wait = 0;
			received_turn = false;
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
		std::vector< pair<int, int> > block_list;
};

class Edge : public Displayable {
	public:
		virtual void paint(XInfo& xinfo) {
			XPoint points[] = {{0, 0}, {800, 0}, {800, 600}, {0, 600}, {0,0}};
			int npoints = sizeof(points) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[1], points, npoints, CoordModeOrigin);

			XPoint points_2[] = {{0, 50}, {800, 50}};
			int npoints_2 = sizeof(points_2) / sizeof(XPoint);
			XDrawLines(xinfo.display, xinfo.window, xinfo.gc[1], points_2, npoints_2, CoordModeOrigin);
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

		// constructor
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
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"animation",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	i = 1;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i], 10, // 5 is line width
						 LineSolid, CapRound, JoinMiter); // other line options
	// load a larger font
	XFontStruct * font;
	font = XLoadQueryFont (xInfo.display, "12x24");
	XSetFont (xInfo.display, xInfo.gc[i], font->fid);


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

	// give server time to setup before sending drawing commands
	//sleep(1);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	XClearWindow( xinfo.display, xinfo.window );
	
	// get height and width of window (might have changed since last repaint)

	XWindowAttributes windowInfo;
	XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
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
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL);					// pointer to a composeStatus structure (unused)
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
		}
	}
	switch(event.xkey.keycode){
			case 134: //up
				snake.change_keyboard(3);
				break;
			case 131: //left
				snake.change_keyboard(2);
				break;
			case 133: //down
				snake.change_keyboard(1);
				break;
			case 132: //right
				snake.change_keyboard(0);
				break;
	}
}

void handleAnimation(XInfo &xinfo, int inside) {
	/*
	 * ADD YOUR OWN LOGIC
	 * This method handles animation for different objects on the screen 
	 * and readies the next frame before the screen is re-painted.
	 */

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
	dList.push_front(&edge);
	dList.push_front(&fruit);
	dList.push_front(&snake);
	dList.push_front(&text_line);
	dList.push_front(&end_display);
	
	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while(true) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */
		
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
			//cerr << "here" << endl;
			usleep(1000000 / FPS - (end - lastRepaint));
		}

		//cerr << FPS << endl;
		//usleep(1000000/FPS);
		//handleAnimation(xinfo, inside);
		//repaint(xinfo);
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
