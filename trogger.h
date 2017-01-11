#include <vector>
using namespace std;
class room {
  public:
    room();
    vector< vector< int > > cache;
    vector< int > exits_x;
    vector< int > exits_y;
    int get_block(int x, int y);
    void make_floor();
    void refresh_exits();
    void new_exit(int x, int y);
    void set_start(int x, int y);
    int num_of_exits;
    int start_x;
    int start_y;
    int x_x;
    int x_y;
  private:
};
class frog {
  public:
    frog(int x, int y);
    int get_lives();
    int get_scores();
    int get_x();
    int get_y();
    void add_life();
    void remove_life();
    void add_score(int time, int level, int column);
    void update_history();
    void set_x(int x);
    void set_y(int y);
    void refresh();
  private:
    int lives;
    int score;
    int x_position;
    int y_position;
    vector< pair <int, int> > history;
};
class moving_object { 
  public:
    void advance(std::vector< std::vector< int > >& cache, frog& frogger);
    int get_color();
    int get_x();
    int get_y();
    int speed;
    
  protected:
    int length;
    int color;
    int x_position;
    int y_position;
    bool left_to_right;
};

class car: public moving_object { 
  public:
    car();
    car(int x, int y, bool L_to_R);
    int speed;
};

class train: public moving_object {
  public:
    train(int x, int y, bool L_to_R);
    int speed;
};

class log: public moving_object {
  public:
    log(int x, int y, bool L_to_R);
    int speed;
    void advance(std::vector< std::vector< int > >& cache, frog& frogger);
};

class level_1 { 
  public:
    vector< moving_object > objects;
    level_1();
    void advance(int cycle, frog& frogger, room& the_room);

  private:

};

#include "trogger.hpp"
