#include <ncurses.h>
#include<vector>

void room::make_floor(){
  init_pair(6,COLOR_WHITE,COLOR_BLACK);
  bkgd(COLOR_PAIR(6));
  cache.resize(COLS, vector<int>(LINES, 6));
  move(x_y, x_x);
  attrset(COLOR_PAIR(4));
  addch(' ');
  refresh();
  for (int i = 0; i < COLS; i++){
    for (int j = 0; j < LINES; j++){
      if(i==x_x && j == x_y)
		    cache[i][j] = 4;
	    else
		    cache[i][j] = 6;
    }
  }
  return;
}
void room::refresh_exits(){
  int x, y;
//  for(int i = 0; i < num_of_exits; i++){
    y = x_y;
    x = x_x;
    move(y, x);
    attrset(COLOR_PAIR(4));
    addch(' ');
    cache[x][y] = 4;
//  }
    refresh();
    return;
}
void room::new_exit(int x, int y){
  int n = num_of_exits;
  exits_y[n] = y;
  exits_x[n] = x;
  set_start(COLS/2, LINES-4);
  num_of_exits++;
  return;
}
void room::set_start(int x, int y){
  start_x = x;
  start_y = y;
  return;
}
room::room(){
  initscr();  
  keypad(stdscr,TRUE);
  start_color(); 
  num_of_exits = 1;
  start_x = COLS/2;
  start_y = LINES-3;
  exits_x.push_back(COLS/2);
  exits_y.push_back(2);
  x_x = COLS/2;
  x_y = 2;  
  make_floor();
  refresh_exits();
  refresh();
  return;
}
int room::get_block(int x, int y){
  return cache[x][y];
}
frog::frog(int x, int y){
  x_position = x;
  y_position = y;
  return;
}
int frog::get_lives(){
  return lives;
}
int frog::get_scores(){
  return score;
}
int frog::get_x(){
  return x_position;
}
int frog::get_y(){
  return y_position;
}
void frog::add_life(){
  lives++;
  return;
}
void frog::remove_life(){
  lives--;
  return;
}
void frog::add_score(int time, int level, int column){
  score = score + (level * 100) - (time / 2) + column;
  return;
}

void frog::set_x(int x){
  x_position = x;
  return;
}

void frog::set_y(int y){
  y_position = y;
  return;
}

void frog::update_history(){
  //history.insert(make_pair(get_x(), get_y()));
  return;
}

void frog::refresh(){
  attrset(COLOR_PAIR(6));
  move(y_position, x_position);
  addch('+');
  refresh();
  return;
}

car::car(){
  x_position = 0;
  y_position = 0;
  left_to_right = TRUE;
  color = 3;
  length = 2;
  speed = 3; 
  return;
}

car::car(int x, int y, bool L_to_R){
  x_position = x;
  y_position = y;
  left_to_right = L_to_R;
  color = 3;
  length = 2;
  speed = 3; 
  return;
}

int moving_object::get_color(){
  return color;
}
void moving_object::advance(std::vector< std::vector < int > >& cache, frog& frogger){
  int direction = 0;
  if (left_to_right){
    direction = 1;
    x_position++;
    if (x_position > 79)
      x_position = 0;
  }
  else{ 
    direction = -1;
    x_position--;
    if (x_position < 0)
      x_position = 79;
  }
  for ( int i = 0; i < length; i++){
    attrset(COLOR_PAIR(color));
    int next_block = x_position-(i*direction);
    if (next_block < 0 )
      next_block += 80;
    else if (next_block > 79)
      next_block -= 80;
    mvaddch(y_position, next_block, ' ');
    cache[next_block][y_position] = color;
  }
    attrset(COLOR_PAIR(6));
    int clean_up = x_position-length;
    if (direction == 1){
      if (clean_up < 0)
        clean_up = clean_up + 80;
      mvaddch(y_position, clean_up, ' ');
      cache[clean_up][y_position] = 6;
    }
    else{
      clean_up = x_position+length;
      if (clean_up > 79)
        clean_up = clean_up - 80;
      mvaddch(y_position, clean_up, ' ');
      cache[clean_up][y_position] = 6;
    }
  return;
}

int moving_object::get_x(){
  return x_position;
}
int moving_object::get_y(){
  return y_position;
}

train::train(int x, int y, bool L_to_R){
  x_position = x;
  y_position = y;
  left_to_right = L_to_R;
  length = 6;
  color = 5;
  speed = 2; 
  return;
}

log::log(int x, int y, bool L_to_R){
  x_position = x;
  y_position = y;
  left_to_right = L_to_R;
  length = 8;
  color = 6;
  speed = 1; 
  return;
}

void log::advance(std::vector< std::vector < int > >& cache, frog& frogger){
  int direction = 0;
  bool on_log = FALSE;
  int frog_x = frogger.get_x();
  int frog_y = frogger.get_y();
  if (left_to_right){
    direction = 1;
    if (frog_y == y_position){
      for (int k = 0; k < length; k++){
        if (frog_x == x_position-k){
          if (frog_x++ > 79)
            frog_x = 0;
          frogger.set_x(frog_x);
          on_log = TRUE;
        }
      }
    }
    x_position++;
    if (x_position > 79)
      x_position = 0;
  }
  else{ 
    direction = -1;
    if (frog_y == y_position){
      for (int k = 0; k < length; k++){
        if (frog_x == x_position+k){
          if (frog_x-- < 0)
            frog_x = 79;
          frogger.set_x(frog_x);
          on_log = TRUE;
        }
      }
    }
    x_position--;
    if (x_position < 0)
      x_position = 79;
  }
  int x = frogger.get_x();
  for ( int i = 0; i < length; i++){
    attrset(COLOR_PAIR(color));
    int next_block = x_position-(i*direction);
    if (next_block < 0 )
      next_block += 80;
    else if (next_block > 79)
      next_block -= 80;
    if (x == next_block && on_log)
      mvaddch(y_position, next_block, '+');
    else
      mvaddch(y_position, next_block, ' ');
    cache[next_block][y_position] = color;
  }
    attrset(COLOR_PAIR(2));
    int clean_up = x_position-length;
    if (direction == 1){
      if (clean_up < 0)
        clean_up = clean_up + 80;
      mvaddch(y_position, clean_up, ' ');
      cache[clean_up][y_position] = 2;
    }
    else{
      clean_up = x_position+length;
      if (clean_up > 79)
        clean_up = clean_up - 80;
      mvaddch(y_position, clean_up, ' ');
      cache[clean_up][y_position] = 2;
    }
  return;
}

level_1::level_1(){
  objects.push_back(car(0, 10, TRUE)); 
  objects.push_back(car(20, 11, FALSE));
  objects.push_back(car(40, 19, TRUE));
  objects.push_back(car(30, 10, TRUE));
  objects.push_back(train(60, 8, FALSE));
  objects.push_back(log(70, 16, TRUE));
  objects.push_back(log(25, 16, TRUE));
  objects.push_back(log(30, 5, FALSE));
  objects.push_back(log(70, 5, FALSE));
  return;  
}

void level_1::advance(int cycle, frog& frogger, room& the_room){
  for(int i = 0; i < objects.size(); i++){
    if(objects[i].speed == 3){
      objects[i].advance(the_room.cache, frogger);}
    else if(objects[i].speed == 2){
      if (cycle % 2 == 1 || cycle % 3 == 2)
        objects[i].advance(the_room.cache, frogger);
    }
    else if(objects[i].speed == 1){
      if (cycle % 2 == 1)
        objects[i].advance(the_room.cache, frogger);
    }
  }
}
