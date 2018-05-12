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

class Snake : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, blockSize);
		}

		void trun () {
			if ((dir == 0 && (receive == 1 || receive == 3)) || 
				(dir == 1 && (receive == 2 || receive == 0)) || 
				(dir == 2 && (receive == 1 || receive == 3)) ||
				(dir == 3 && (receive == 0 || receive == 2))) {
				dir = receive;
				direction = abs(direction);
			}
		}
		
		void move(XInfo &xinfo) {
			//cerr << direction << endl;
			if (hit_pause != 0 && (hit_pause % 2)) {return;}

			trun();
			if (dir == 0) {
				x = x + direction;
				if (x < 0 || x > width - 10) {
					direction = -direction;
				}
			} else if (dir == 1) {
				y = y + direction;
				if (y < 60 || y > height) {
					direction = -direction;
				}
			} else if (dir == 2) {
				x = x - direction;
				if (x < 10 || x > width) {
					direction = -direction;
				}
			} else if (dir == 3) {
				y = y - direction;
				if (y < 60 || y > height) {
					direction = -direction;
				}
			}

            // ** ADD YOUR LOGIC **
            // Here, you will be performing collision detection between the snake, 
            // the fruit, and the obstacles depending on what the snake lands on.
		}
		
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}

        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour. 
         * You do not have to use these methods, feel free to implement your own.
         */
		void didEatFruit() {
        }

        void didHitObstacle() {}

		Snake(int x, int y, int dir, int receive): x(x), y(y), dir(dir), receive(receive) {
			direction = speed;
			blockSize = 10;
		}

		void change_keyboard(int re) {
			receive = re;
		}

		void set_direction(int argv_2) {
			direction = argv_2;
		}

	private:
		int x;
		int y;
		int blockSize;
		int direction;
		int dir; // 0 right; 1 down; 2 left; 3 up
		int receive; // 0 d; 1 s; 2 a; 3 w
};

class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10);
        }

        Fruit() {
            // ** ADD YOUR LOGIC **
            // generate the x and y value for the fruit 
            x = 50;
            y = 50;
        }

        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */

    private:
        int x;
        int y;
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

		// constructor
		Edge() {}
};

class Text : public Displayable {
	public:
		virtual void paint(XInfo& xinfo) {
			ostringstream stream_1, stream_2;
			stream_1 << print_Speed;
			stream_2 << print_FPS;
			s = "";
			s.append("Score: ");
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
Snake snake(100, 450, 0, 0);
Fruit fruit;
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
			snake.set_direction(atoi(argv[2]));
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
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                   7, LineSolid, CapRound, JoinMiter);
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
	sleep(1);
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
		NULL );					// pointer to a composeStatus structure (unused)
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
				cerr << "received restart" << endl; 
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
	dList.push_front(&snake);
	dList.push_front(&fruit);
	dList.push_front(&edge);
	dList.push_front(&text_line);
	
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

		//cerr << FPS << endl;
		usleep(1000000/FPS);
		handleAnimation(xinfo, inside);
		repaint(xinfo);
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
