#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h> 
#include "Displayable.h"
using namespace std;
 
// Global game state variables
const static int Border = 1;
const static int BufferSize = 10;
const static int width = 800;
const static int height = 600;

// Function to put out a message on error exits
void error(string str) {
	cerr << str << endl;
	exit(0);
}

// Initialize X and create a window
void initX(int argc, char *argv[], XInfo * xInfo) {
	XSizeHints hints;
	unsigned long white, black;

	// Display opening uses the DISPLAY	environment variable.
	// It can go wrong if DISPLAY isn't set, or you don't have permission.
	xInfo->display = XOpenDisplay("");
	if (!xInfo->display) { error("Can't open display."); }
	
	// Find out some things about the display you're using.
	xInfo->screen = DefaultScreen( xInfo->display );

	white = XWhitePixel(xInfo->display, xInfo->screen);
	black = XBlackPixel(xInfo->display, xInfo->screen);

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xInfo->window = XCreateSimpleWindow( 
		xInfo->display, // display where window appears
		DefaultRootWindow( xInfo->display ), // window's parent in window tree
		hints.x, hints.y, // upper left corner location
		hints.width, hints.height, // size of the window
		Border, // width of window's border
		black, // window border colour
		white); // window background colour
		
	XSetStandardProperties(
		xInfo->display, // display containing the window
		xInfo->window, // window whose properties are set
		"animation", // window's title
		"Animate", // icon's title
		None, // pixmap for the icon
		argv, argc, // applications command line args
		&hints); // size hints for the window

	// Create Graphics Contexts
	int i = 0;
	xInfo->gc[i] = XCreateGC(xInfo->display, xInfo->window, 0, 0);
	XSetForeground(xInfo->display, xInfo->gc[i], BlackPixel(xInfo->display, xInfo->screen));
	XSetBackground(xInfo->display, xInfo->gc[i], WhitePixel(xInfo->display, xInfo->screen));
	XSetFillStyle(xInfo->display, xInfo->gc[i], FillSolid);
	XSetLineAttributes(xInfo->display, xInfo->gc[i], 1, LineSolid, CapButt, JoinRound);

	i = 1;
	xInfo->gc[i] = XCreateGC(xInfo->display, xInfo->window, 0, 0);
	XSetForeground(xInfo->display, xInfo->gc[i], BlackPixel(xInfo->display, xInfo->screen));
	XSetBackground(xInfo->display, xInfo->gc[i], WhitePixel(xInfo->display, xInfo->screen));
	XSetFillStyle(xInfo->display, xInfo->gc[i], FillSolid);
	XSetLineAttributes(xInfo->display, xInfo->gc[i], 10, LineSolid, CapButt, JoinBevel);
	// load a larger font
	XFontStruct * font;
	font = XLoadQueryFont (xInfo->display, "12x24");
	XSetFont (xInfo->display, xInfo->gc[i], font->fid);

	i = 2;
	xInfo->gc[i] = XCreateGC(xInfo->display, xInfo->window, 0, 0);
	XSetForeground(xInfo->display, xInfo->gc[i], WhitePixel(xInfo->display, xInfo->screen));
	XSetBackground(xInfo->display, xInfo->gc[i], BlackPixel(xInfo->display, xInfo->screen));
	// load a larger font
	XFontStruct * font_2;
	font_2 = XLoadQueryFont (xInfo->display, "12x24");
	XSetFont (xInfo->display, xInfo->gc[i], font_2->fid);


	XSelectInput(xInfo->display, xInfo->window, 
		ButtonPressMask | KeyPressMask | 
		PointerMotionMask | 
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	// Put the window on the screen
	XMapRaised( xInfo->display, xInfo->window );
	XFlush(xInfo->display);
	int temp = XFreeFont(xInfo->display, font);
	int temp2 = XFreeFont(xInfo->display, font_2);
}

// Function to repaint a display list
void repaint(XInfo * xinfo, vector<Displayable *> displayable_vector) {
	XClearWindow( xinfo->display, xinfo->window );
	
	// get height and width of window (might have changed since last repaint)
	XWindowAttributes windowInfo;
	//XGetWindowAttributes(xinfo->display, xinfo->window, &windowInfo);
	unsigned int height = windowInfo.height;
	unsigned int width = windowInfo.width;

	// big black rectangle to clear background
    
	// draw display list
	for (vector<Displayable *>::iterator it = displayable_vector.begin(); it != displayable_vector.end(); ++it) {
		(*it)->paint(xinfo);
	}

	XFlush( xinfo->display );
}

void handle_key_press(XInfo * xinfo, XEvent * event, vector<Displayable *> displayable_vector) {
	cerr << "check segmentation fault 1" << endl;
	KeySym key;
	char text[BufferSize];

	int i = XLookupString( 
		(XKeyEvent *) event, // the keyboard event
		text, // buffer when text will be written
		BufferSize, // size of the text buffer
		&key, // workstation-independent key symbol
		NULL); // pointer to a composeStatus structure (unused)

	cerr << "check segmentation fault 2" << endl;

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
				((Snake *) displayable_vector[1])->change_keyboard(3);
				break;
			case 'a':
			case 'A':
				((Snake *) displayable_vector[1])->change_keyboard(2);
				break;
			case 's':
			case 'S':
				((Snake *) displayable_vector[1])->change_keyboard(1);
				break;
			case 'd':
			case 'D':
				((Snake *) displayable_vector[1])->change_keyboard(0);
				break;
			case 'k':
			case 'K':
				if (start_game == true) {
					hit_pause = 0;
				}
				start_game = false;
				break;
		}
	}
	// switch(key) {
	// 		case XK_Up:
	// 			cerr << "double here" << endl;
	// 			((Snake *) displayable_vector[1])->change_keyboard(3);
	// 			break;
	// 		case XK_Down:
	// 			((Snake *) displayable_vector[1])->change_keyboard(1);
	// 			break;
	// 		case XK_Left:
	// 			((Snake *) displayable_vector[1])->change_keyboard(2);
	// 			break;
	// 		case XK_Right:
	// 			((Snake *) displayable_vector[1])->change_keyboard(0);
	// 			break;
	// }
}

void event_loop(XInfo * xinfo, vector<Displayable *> displayable_vector) {
	XEvent * event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while(true) {
		if (XPending(xinfo->display) > 0) {
			XNextEvent(xinfo->display, event);
			//cerr << "event->type = " << event->type << endl;
			switch(event->type) {
				case KeyPress:
					handle_key_press(xinfo, event, displayable_vector);
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
			((Snake *) displayable_vector[1])->move();
			repaint(xinfo, displayable_vector);
			lastRepaint = now();
		}

		// IMPORTANT: sleep for a bit to let other processes work
		if (XPending(xinfo->display) == 0) {
			usleep(1000000 / FPS - (end - lastRepaint));
		}
	}
}

void receive_input(int argc, char *argv[]) {
	cerr << "init FPS = " << FPS << " init speed = " << speed << endl;
	if (argc == 2 && atoi(argv[1]) >= 1 && atoi(argv[1]) <= 100) {
		FPS = atoi(argv[1]);
	}
	if (argc == 3) {
		if (atoi(argv[1]) >= 1 && atoi(argv[1]) <= 100) {
			FPS = atoi(argv[1]);
		}
		if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 10) {
			speed = atoi(argv[2]);
		}
	}
	cerr << "use FPS = " << FPS << " use speed = " << speed << endl;
}

// First initialize window.
// Next loop responding to events.
// Exit forcing window manager to clean up.
int main (int argc, char * argv[]) {
	vector<Displayable *> displayable_vector; // list of Displayables

	XInfo * xInfo = new XInfo();
	receive_input(argc, argv);

	Fruit * fruit = new Fruit();
	Snake * snake = new Snake(0, 0, fruit);
	Start_Display * start_display = new Start_Display();
	End_Display * end_display = new End_Display();
	Edge * edge = new Edge();
	Text * text_line = new Text(25, 35);
	
	displayable_vector.push_back(text_line);
	displayable_vector.push_back(snake);
	displayable_vector.push_back(fruit);
	displayable_vector.push_back(edge);
	displayable_vector.push_back(end_display);
	displayable_vector.push_back(start_display);

	initX(argc, argv, xInfo);
	event_loop(xInfo, displayable_vector);
	XCloseDisplay(xInfo->display);

	delete fruit;
	fruit = NULL;
	delete snake;
	snake = NULL;
	delete start_display;
	start_display = NULL;
	delete end_display;
	end_display = NULL;
	delete edge;
	edge = NULL;
	delete text_line;
	text_line = NULL;
}
