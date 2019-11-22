#ifndef _DISPLAYABLE_H_
#define _DISPLAYABLE_H_

#include <iostream>
#include <vector>
#include <vector>
#include <unistd.h>
#include <string>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
using namespace std;

/* 
exit:
(0, 260) left
(790, 360) right
(600, 590) down
(300, 40) up

left:
x [60, 80]
y [470, 500]
s
middle:
x [200, 650]
y [260, 300]

right:
x [660, 730]
y [100, 140]
*/

static int fruit_x = 300;
static int fruit_y = 450;
static int eat_fruit = 0;
static bool end_game = false;

extern int FPS;
extern int speed;
extern int hit_pause;
extern bool press_restart_fruit;
extern bool press_restart_snake;
extern bool press_restart_score;
extern bool start_game;

// Information to draw on the window
struct XInfo {
	Display *display;
	int screen;
	Window window;
	GC gc[3];
	int width; // size of window
	int height;
};

// An abstract class representing displayable things
class Displayable {
	public:
		virtual void paint(XInfo * xinfo) = 0;
};

class Fruit : public Displayable {
	public:
		virtual void paint(XInfo * xinfo);
		Fruit();
		void re_generate();

	private:
		int x;
		int y;
};

class Start_Display : public Displayable {
	public:
		virtual void paint(XInfo * xinfo);
	    Start_Display();

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

class End_Display : public Displayable {
	public:
		virtual void paint(XInfo * xinfo);
		End_Display();

	private:
		string s;
		int x;
		int y;
};

class Snake : public Displayable {
	public:
		virtual void paint(XInfo * xinfo);
		void trun ();
		void update_list();
		void move();
		bool check_regenerate();
		void eat_add();
		void did_eat_fruit(int where);
		bool did_hit_obstacle(int where);
		Snake(int dir, int receive, Fruit * fruit);
		void change_keyboard(int re);
		bool should_repaint();

	private:
		int blockSize;
		int dir; // 0 right; 1 down; 2 left; 3 up
		int receive; // 0 d; 1 s; 2 a; 3 w
		bool received_turn;
		bool edge_gap;
		vector<pair<int, int> > block_list;
		unsigned long last_move_time;
        Fruit * fruit;
};

class Edge : public Displayable {
	public:
		virtual void paint(XInfo * xinfo);
};

class Text : public Displayable {
	public:
		virtual void paint(XInfo * xinfo);
		Text(int x, int y);

	private:
		int x;
		int y;
		int print_Speed;
		int print_FPS;
		string s;
};

// get microseconds
unsigned long now();

#endif