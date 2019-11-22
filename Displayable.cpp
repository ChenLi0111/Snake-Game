#include "Displayable.h"
using namespace std;

int FPS = 30;
int speed = 5;
int hit_pause = 0;
bool press_restart_fruit = false;
bool press_restart_snake = false;
bool press_restart_score = false;
bool start_game = true;

void Fruit::paint(XInfo * xinfo) {
    if (press_restart_fruit == true) {
        x = 300;
        y = 450;
        fruit_x = x;
        fruit_y = y;
        press_restart_fruit = false;
        end_game = false;
    }
    XFillRectangle(xinfo->display, xinfo->window, xinfo->gc[0], x, y, 10, 10);
}

Fruit::Fruit(): x(300), y(450) {}

void Fruit::re_generate() {
    x = (rand() % 78) * 10 + 10;
    y = (rand() % 54) * 10 + 50;
    fruit_x = x;
    fruit_y = y;
}

void Start_Display::paint(XInfo * xinfo) {
    if (start_game == true) {
        XFillRectangle(xinfo->display, xinfo->window, xinfo->gc[0], 0, 0, 800, 600);
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 100, s_1.c_str(), s_1.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 150, s_2.c_str(), s_2.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 175, s_3.c_str(), s_3.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 200, s_4.c_str(), s_4.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 225, s_5.c_str(), s_5.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 250, s_6.c_str(), s_6.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 275, s_7.c_str(), s_7.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 325, s_8.c_str(), s_8.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 350, s_9.c_str(), s_9.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 375, s_10.c_str(), s_10.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 400, s_11.c_str(), s_11.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 425, s_12.c_str(), s_12.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 450, s_13.c_str(), s_13.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 475, s_14.c_str(), s_14.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], 100, 525, s_15.c_str(), s_15.length());
    }
}

Start_Display::Start_Display() {
    s_1 = "Author: Chen Li";
    s_2 = "Welcome to this snake game.";
    s_3 = "Control the snake to eat fruits and avoid obstacles";
    s_4 = " and its own body.";
    s_5 = "The snake will grow longer when it eats a fruit.";
    s_6 = "The more fruits the snake eats, the higher score";
    s_7 = " you will receive.";
    s_8 = "Use gaps at the border to go to the oppsite side.";
    s_9 = "Please press k/K to start the game.";
    s_10 = "Use w/W/a/A/s/S/d/D or arrows keys to control";
    s_11 = " the direction of the snake.";
    s_12 = "Use r/R to restart.";
    s_13 = "Use p/P to pause.";
    s_14 = "Use q/Q to quit the game.";
    s_15 = "Good luck and have fun!";
}

void End_Display::paint(XInfo * xinfo) {
    if (end_game == true) {
        XFillRectangle(xinfo->display, xinfo->window, xinfo->gc[0], 0, 0, 800, 600);
        string temp = "Score: ";
        temp.append(to_string(eat_fruit));
        string ss = "Press r/R to restart.";
        string sss = "Press q/Q to quit.";
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], x, y, s.c_str(), s.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], x, y + 50, temp.c_str(), temp.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], x, y + 100, ss.c_str(), ss.length());
        XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[2], x, y + 150, sss.c_str(), sss.length());
    }
}

End_Display::End_Display(): x(250), y(250), s("Game Over!") {}

void Snake::paint(XInfo * xinfo) {
    if (press_restart_snake == true) {
        block_list.clear();
        dir = 0;
        receive = 0;
        edge_gap = false;
        received_turn = false;
        int xx_1 = 100;
        int xx_2 = 90;
        int xx_3 = 80;
        int yy_1 = 260;
        int yy_2 = 260;
        int yy_3 = 260;
        block_list.push_back(make_pair(xx_1, yy_1));
        block_list.push_back(make_pair(xx_2, yy_2));
        block_list.push_back(make_pair(xx_3, yy_3));
        hit_pause = 0;
        press_restart_snake = false;
        last_move_time = now();
    }
    for (vector<pair<int, int> >::iterator it = block_list.begin(); it != block_list.end(); ++it) {
        XFillRectangle(xinfo->display, xinfo->window, xinfo->gc[0], (*it).first, (*it).second, 10, blockSize);
    }
}

void Snake::trun () {
    if ((dir == 0 && (receive == 1 || receive == 3)) || 
        (dir == 1 && (receive == 2 || receive == 0)) || 
        (dir == 2 && (receive == 1 || receive == 3)) ||
        (dir == 3 && (receive == 0 || receive == 2))) {
        dir = receive;
        received_turn = true;
    }
}

void Snake::update_list() {
    for (vector<pair<int, int> >::reverse_iterator it = block_list.rbegin(); it != block_list.rend() - 1; ++it) {
        (*it).first = (*(it + 1)).first;
        (*it).second = (*(it + 1)).second;
    }
}

void Snake::move() {
    if ((hit_pause != 0 && (hit_pause % 2)) ||
        (end_game == true) ||
        (start_game == true) ||
        (should_repaint() == false)) { return; }
    if (received_turn != true) { trun(); }
    if (did_hit_obstacle(dir)) { return; }
    did_eat_fruit(dir);
    received_turn = false;
    last_move_time = now();
}

bool Snake::check_regenerate() {
    if ((fruit_x >= 60 && fruit_x <= 80 && fruit_y >= 470 && fruit_y <= 500) ||
        (fruit_x >= 200 && fruit_x <= 650 && fruit_y >= 260 && fruit_y <= 300) ||
        (fruit_x >= 660 && fruit_x <= 730 && fruit_y >= 100 && fruit_y <= 140)) {
        return true;
    }

    for (vector<pair<int, int> >::iterator it = block_list.begin(); it != block_list.end(); ++it) {
        if (fruit_x == (*it).first && fruit_y == (*it).second) {
            return true;
        }
    }
    return false;
}

void Snake::eat_add() {
    block_list.insert(block_list.begin(), make_pair(fruit_x, fruit_y));
    eat_fruit++;
    do {
        fruit->re_generate();
    } while (check_regenerate());	
}

void Snake::did_eat_fruit(int where) {
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

bool Snake::did_hit_obstacle(int where) {
    //hit edges
    if ((where == 0 && block_list.front().first >= 780 && block_list.front().second != 360) ||
        (where == 1 && block_list.front().second >= 580 && block_list.front().first != 600) ||
        (where == 2 && block_list.front().first <= 10 && block_list.front().second != 260) ||
        (where == 3 && block_list.front().second <= 50 && block_list.front().first != 300) ||
        ((where == 1 || where == 3) && block_list.front().first == 790 && block_list.front().second == 360) ||
        ((where == 0 || where == 2) && block_list.front().first == 600 && block_list.front().second == 590) || 
        ((where == 1 || where == 3) && block_list.front().first == 0 && block_list.front().second == 260) ||
        ((where == 0 || where == 2) && block_list.front().first == 300 && block_list.front().second == 40)) {
        end_game = true;
        return true;
    }

    //hit a block
    if (where == 0 && 
        ((block_list.front().first == 50 && block_list.front().second >= 470 && block_list.front().second <= 490) || 
            (block_list.front().first == 190 && block_list.front().second >= 260 && block_list.front().second <= 290) || 
            (block_list.front().first == 650 && block_list.front().second >= 100 && block_list.front().second <= 130))) {
        end_game = true;
        return true;
    } else if (where == 1 && 
        ((block_list.front().first >= 60 && block_list.front().first <= 70 && block_list.front().second == 460) ||
            (block_list.front().first >= 200 && block_list.front().first <= 640 && block_list.front().second == 250) ||
            (block_list.front().first >= 660 && block_list.front().first <= 720 && block_list.front().second == 90))) {
        end_game = true;
        return true;
    } else if (where == 2 && 
        ((block_list.front().first == 80 && block_list.front().second >= 470 && block_list.front().second <= 490) || 
            (block_list.front().first == 650 && block_list.front().second >= 260 && block_list.front().second <= 290) || 
            (block_list.front().first == 730 && block_list.front().second >= 100 && block_list.front().second <= 130))) {
        end_game = true;
        return true;
    } else if (where == 3 &&
        ((block_list.front().first >= 60 && block_list.front().first <= 70 && block_list.front().second == 500) ||
            (block_list.front().first >= 200 && block_list.front().first <= 640 && block_list.front().second == 300) ||
            (block_list.front().first >= 660 && block_list.front().first <= 720 && block_list.front().second == 140))) {
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
    if (where == 0 && block_list.front().first == 790 && block_list.front().second == 360) {
        update_list();
        edge_gap = true;
        block_list.front().first = 0;
        block_list.front().second = 260;
        return false;
    } else if (where == 1 && block_list.front().first == 600 && block_list.front().second == 590) {
        update_list();
        edge_gap = true;
        block_list.front().first = 300;
        block_list.front().second = 40;
        return false;
    } else if (where == 2 && block_list.front().first == 0 && block_list.front().second == 260) {
        update_list();
        edge_gap = true;
        block_list.front().first = 790;
        block_list.front().second = 360;
        return false;
    } else if (where == 3 && block_list.front().first == 300 && block_list.front().second == 40) {
        update_list();
        edge_gap = true;
        block_list.front().first = 600;
        block_list.front().second = 590;
        return false;
    }
    return false;
}

Snake::Snake(int dir, int receive, Fruit * fruit):
    dir(dir),
    receive(receive),
    fruit(fruit),
    blockSize(10),
    received_turn(false),
    edge_gap(false),
    last_move_time(0) {
    int x_1 = 100;
    int x_2 = 90;
    int x_3 = 80;
    int y_1 = 260;
    int y_2 = 260;
    int y_3 = 260;
    block_list.push_back(make_pair(x_1, y_1));
    block_list.push_back(make_pair(x_2, y_2));
    block_list.push_back(make_pair(x_3, y_3));
}

void Snake::change_keyboard(int re) {
    if (start_game == true || (hit_pause != 0 && (hit_pause % 2))) { return; }
    receive = re;
}

bool Snake::should_repaint() {
    unsigned long stay = 1000000 / speed;
    if (now() - last_move_time < stay) { return false; }
    return true;
}

void Edge::paint(XInfo * xinfo) {
    XPoint points_1[] = {{5, 260}, {5, 5}, {795, 5}, {795, 360}};
    int npoints_1 = sizeof(points_1) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[1], points_1, npoints_1, CoordModeOrigin);

    XPoint points_2[] = {{5, 270}, {5, 595}, {600, 595}};
    int npoints_2 = sizeof(points_2) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[1], points_2, npoints_2, CoordModeOrigin);

    XPoint points_3[] = {{610, 595}, {795, 595}, {795, 370}};
    int npoints_3 = sizeof(points_3) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[1], points_3, npoints_3, CoordModeOrigin);

    XPoint points_4[] = {{5, 45}, {300, 45}};
    int npoints_4 = sizeof(points_4) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[1], points_4, npoints_4, CoordModeOrigin);

    XPoint points_5[] = {{310, 45}, {795, 45}};
    int npoints_5 = sizeof(points_5) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[1], points_5, npoints_5, CoordModeOrigin);

    XPoint points_6[] = {{60, 470}, {60, 500}, {80, 500}, {80, 470}, {60, 470}};
    int npoints_6 = sizeof(points_6) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[0], points_6, npoints_6, CoordModeOrigin);
    XPoint points_9[] = {{60, 470}, {80, 500}, {80, 470},{60, 500}};
    int npoints_9 = sizeof(points_9) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[0], points_9, npoints_9, CoordModeOrigin);
                
    XPoint points_7[] = {{200, 260}, {200, 300}, {650, 300}, {650, 260}, {200, 260}};
    int npoints_7 = sizeof(points_7) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[0], points_7, npoints_7, CoordModeOrigin);
    XPoint points_10[] = {{200, 260},{650, 300}, {650, 260}, {200, 300}};
    int npoints_10 = sizeof(points_10) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[0], points_10, npoints_10, CoordModeOrigin);
                
    XPoint points_8[] = {{660, 100}, {660, 140}, {730, 140}, {730, 100}, {660, 100}};
    int npoints_8 = sizeof(points_8) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[0], points_8, npoints_8, CoordModeOrigin);
    XPoint points_11[] = {{660, 100}, {730, 140}, {730, 100}, {660, 140}};
    int npoints_11 = sizeof(points_11) / sizeof(XPoint);
    XDrawLines(xinfo->display, xinfo->window, xinfo->gc[0], points_11, npoints_11, CoordModeOrigin);
}

void Text::paint(XInfo * xinfo) {
    if (press_restart_score == true) {
        eat_fruit = 0;
        press_restart_score = false;
    }
    s = "";
    s.append("Score: ");
    s.append(to_string(eat_fruit));
    s.append(" FPS: ");
    s.append(to_string(FPS));
    s.append(" Speed: ");
    s.append(to_string(speed));
    XDrawImageString(xinfo->display, xinfo->window, xinfo->gc[1], x, y, s.c_str(), s.length());
}

Text::Text(int x, int y): x(x), y(y), s("") {}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}