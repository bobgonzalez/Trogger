/*************************************************************
*   Robert Gonzalez                                          *
*                                                            *
*   Trogger                                                  *
*                                                            *
**************************************************************

TODO: 
    map class with new function for fresh map
    make levels classes with level.advance function
    score = large number - cycles
    make level creation logic relative to LINES%80 COLS%28
    map of path taken after win
    ghost of  best time to compete with via ghost_lvl_text files
    randomly generated levels
*************************************************************/

#include "trogger.h"
#include <iostream>     // std::cout
#include <utility>      // std::pair, std::make_pair
#include <ncurses.h>
#include <vector>

using namespace std;
int cycle = 0;
car car1(0, 10, TRUE); 
car car2(20, 11, FALSE);
car car3(40, 19, TRUE);
car car4(30, 10, TRUE);
train train1(60, 8, FALSE);
log log1(70, 16, TRUE);
log log2(25, 16, TRUE);
log log3(30, 5, FALSE);
log log4(70, 5, FALSE);

void level_1 (frog& frogger, room& the_room){
  int j, i;
  
  //  the_room.new_exit(COLS/2, 1);
  //  the_room.refresh_exit();
  //  the_room.set_start(COLS/2, LINES-3);
  if (cycle % 2 == 1){
    for (j = 0; j < LINES; j++){
      for (i = 0; i < COLS; i++){
	      if (j == 16 || j == 5){
	        attrset(COLOR_PAIR(2));
	        the_room.cache[i][j] = 2;
	        mvaddch(j, i, ' ');
	      }
      }
    }
    log4.advance(the_room.cache, frogger);
    log3.advance(the_room.cache, frogger);
    log1.advance(the_room.cache, frogger);
    log2.advance(the_room.cache, frogger);
  }
  if (cycle % 2 == 1 || cycle % 3 == 2)
    train1.advance(the_room.cache, frogger);
  car4.advance(the_room.cache, frogger);
  car1.advance(the_room.cache, frogger);
  car2.advance(the_room.cache, frogger);
  car3.advance(the_room.cache, frogger);
  move(1, COLS/2);
  attrset(COLOR_PAIR(4));
  addch(' ');
  the_room.cache[COLS/2][1]=4;
  refresh();
  return;
}

void level_2(){
}

void level_3(){
}

void levels(int level, frog& frogger, room& the_room){
  switch (level){
  case 1: 
    level_1(frogger, the_room);
    break;
  case 2: 
    level_2();
    break;
  case 3: 
    level_3();
    break;
  }
  return;
}
bool death (int score, int level){
  init_pair(6,COLOR_WHITE, COLOR_BLACK);
  attrset(COLOR_PAIR(6));
  attron(A_REVERSE) ;
  mvprintw(LINES/2, (COLS/2)-20, "YOU DIED!!! at level %d with %d points", level, score);
  mvprintw(LINES/2-1, (COLS/2)-20, "press 1 to play again and 0 to quit");
  refresh();
  char key;
  do {
    key = getch();
    if (key == '1')
      return false;
    else if (key == '0')
      return true;
  } while (key != '\n');
}
void win (){
  return;
}
void erase (room& the_room){
  int j, i, y_pos, x_pos; 
  j=0;
  for (j; j<LINES; j++){
    y_pos=j;
    i=0; 
    x_pos=i;
    for (i; i<COLS; i++){
      attrset(COLOR_PAIR(6));
      the_room.cache[x_pos+i][y_pos]=6;
      move(y_pos, x_pos+i);
      addch(' ');
      refresh();
    }
  }
  clear();
}

int main (void){
  int key, moveNum, level, lifes;
  int score = 0;
  bool debug=false;
  level = 1;
  lifes = 0;
  room the_room;
  the_room.set_start(COLS/2, LINES-3);
  if (debug)
    lifes = -1;
  
  start_color();
  init_pair(1,COLOR_WHITE, COLOR_BLUE);
  init_pair(2,COLOR_WHITE, COLOR_CYAN);
  init_pair(3,COLOR_WHITE, COLOR_RED);
  init_pair(4,COLOR_RED, COLOR_WHITE);
  init_pair(5,COLOR_WHITE, COLOR_MAGENTA);
  init_pair(6,COLOR_WHITE, COLOR_BLACK);
  the_room.cache.resize(COLS, vector<int>(LINES, 6));
  noecho();
  moveNum=0;
  nodelay(stdscr,TRUE);
  frog frogger = frog(COLS/2, LINES-3);
  
  do{
    levels(level, frogger, the_room);
    cycle++;
    napms(15);
    the_room.cache.resize(COLS, vector<int>(LINES, 6));
    key=getch();
    int x = frogger.get_x();
    int y = frogger.get_y();
    switch(key){
    case 'j':
    case KEY_DOWN:
      moveNum++;
      move(y, x);
      attrset(COLOR_PAIR(the_room.cache[x][y]));
      frogger.set_y(y+1);
      addch(' ');
      if (frogger.get_y() > LINES-2) 
	      frogger.set_y(LINES-2);
      break;
    case 'k':
    case KEY_UP:
      moveNum++;
      move(y, x);
      attrset(COLOR_PAIR(the_room.cache[x][y]));
      frogger.set_y(y-1);
      addch(' ');
      if (frogger.get_y() < 0) 
	      frogger.set_y(0);
      break;
    case 'h':
    case KEY_LEFT:
      moveNum++;
      move(y, x);
      attrset(COLOR_PAIR(the_room.cache[x][y]));
      frogger.set_x(x-1);
      addch(' ');
      if (frogger.get_x() < 0) 
	      frogger.set_x(COLS-1);
      break;
    case 'l':
    case KEY_RIGHT:
      moveNum++;
      move(y, x);
      attrset(COLOR_PAIR(the_room.cache[x][y]));
      frogger.set_x(x+1);
      addch(' ');
      if (frogger.get_x() > COLS) 
	      frogger.set_x(0);
      break;
    default:
      break;
    }
    if (the_room.get_block(x,y) == 4){
      beep();
      flash();
//    level++;
      lifes++;
      frogger.set_x(COLS/2);
      frogger.set_y(LINES-3);
      score += 10000 - cycle;
      cycle = 0;
    }
    else if (the_room.cache[x][y] != 6){
      if (lifes == 0){
        bool quit = death(score, level);
        if (quit == true){
	        return 0;
        }
        else {
	        lifes = 0;
	        level = 1; 
	        score = 0;
	        frogger = frog(COLS/2, LINES-3);
          the_room.make_floor();
        }
      }
      else{
        frogger.set_x(COLS/2);
        frogger.set_y(LINES-2);
        frogger.remove_life();
        lifes--;
      }
    }
    x = frogger.get_x();
    y = frogger.get_y();
    move(1, COLS/2);
    attrset(COLOR_PAIR(4));
    addch(' ');
    the_room.refresh_exits();
    attrset(COLOR_PAIR(6));
    attron(A_REVERSE) ;
    mvprintw(LINES-1, 0, "       Time:  %d        Score:  %d    x:%d, y:%d     Lifes: %d    Level: %d          ", cycle, score, x, y, lifes, level);
    attroff(A_REVERSE);
    move(y, x);
    addch('+');
    refresh();
  } while(key!='\n');  endwin();
  return 0;
}
