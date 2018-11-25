// code by Jakub Smolaga

#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <sstream>
#include <fstream>
using namespace sf;

const int W=20, H=20, B=25;
const float DELAY=0.1;
std::vector<int> tail_x;
std::vector<int> tail_y;

int snake_x=10, snake_y=10, last_dir=0, next_dir=0;
int fruit_x=3, fruit_y=3;
int best_score=0;

// restart the game
void restart(){
  snake_x=10, snake_y=10, last_dir=0, next_dir=0;
  fruit_x=rand()%W;
  fruit_y=rand()%H;
  tail_x.clear();
  tail_y.clear();
}

int main(){
  RenderWindow window(VideoMode(W*B, H*B), "Snake by Jakub Smolaga");
  std::ifstream infile;
  infile.open("resources/best_score.dat");
  infile >> best_score;
  infile.close();
  Font font;
  font.loadFromFile("resources/pixel_font.ttf");
  Text text("score 0        best score 0", font);
  text.setPosition(0,-8);
  text.setCharacterSize(30);
  std::stringstream ss;
  Texture texture_blue, texture_orange, texture_green;
  texture_blue.  loadFromFile ("resources/blue.png");
  texture_orange.loadFromFile ("resources/orange.png");
  texture_green. loadFromFile ("resources/green.png");
  Sprite sprite_blue   (texture_blue);
  Sprite sprite_orange (texture_orange);
  Sprite sprite_green  (texture_green);

  srand(time(NULL));
  Clock clock;
  float timer=0;

  while(window.isOpen()){
    timer+= clock.getElapsedTime().asSeconds();
    clock.restart();
    Event event;
    while(window.pollEvent(event)){
      if(event.type == Event::Closed){
        // save best score
        std::ofstream outfile;
        outfile.open("resources/best_score.dat");
        outfile << best_score;
        outfile.close();
        window.close();
      }
    }
    // CONTROLS
    if(Keyboard::isKeyPressed(Keyboard::Up)    && last_dir!=2) next_dir=0;
    if(Keyboard::isKeyPressed(Keyboard::Right) && last_dir!=3) next_dir=1;
    if(Keyboard::isKeyPressed(Keyboard::Down)  && last_dir!=0) next_dir=2;
    if(Keyboard::isKeyPressed(Keyboard::Left)  && last_dir!=1) next_dir=3;

    // UPDATE
    if(timer>DELAY){
      // update tail positions
      for(int i=0;tail_x.size()&&i<tail_x.size()-1;i++){
       tail_x[i] = tail_x[i+1];
       tail_y[i] = tail_y[i+1];
      }
      if(tail_x.size()) tail_x[tail_x.size()-1] = snake_x;
      if(tail_y.size()) tail_y[tail_y.size()-1] = snake_y;
      // update head position
      if      (next_dir==0) snake_y--;
      else if (next_dir==1) snake_x++;
      else if (next_dir==2) snake_y++;
      else if (next_dir==3) snake_x--;
      if(snake_x>=W) snake_x=0; if(snake_x<0) snake_x=W-1;
      if(snake_y>=H) snake_y=0; if(snake_y<0) snake_y=H-1;
      // if snake eats food spawn new food and increase tail
      if(snake_x==fruit_x && snake_y==fruit_y) {
        // make sure food doesn't spawn on snake
        bool ok=false;
        while(!ok){
          ok=true;
          fruit_x=rand()%W;
          fruit_y=rand()%H;
          for(int i=0;i<tail_x.size();i++)
            if(tail_x[i]==fruit_x&&tail_y[i]==fruit_y)
              ok=false;
          if(snake_x==fruit_x&&snake_y==fruit_y)
            ok=false;
        }
        tail_x.push_back(-1);
        tail_y.push_back(-1);
        if(tail_x.size() > best_score)
          best_score = tail_x.size();
      }
      // if snake eats its tail the game restarts
      for(int i=0;i<tail_x.size();i++)
        if(tail_x[i]==snake_x && tail_y[i]==snake_y)
          restart();

      timer=0;
      last_dir=next_dir;

      // DRAW
      window.clear();
      for(int i=0;i<H;i++)
        for(int j=0;j<W;j++){
          sprite_blue.setPosition(i*B, j*B);
          window.draw(sprite_blue);
        }
      for(int i=0;i<tail_x.size();i++){
        sprite_orange.setPosition(tail_x[i]*B, tail_y[i]*B);
        window.draw(sprite_orange);
      }
      sprite_orange.setPosition(snake_x*B, snake_y*B);
      window.draw(sprite_orange);
      sprite_green.setPosition(fruit_x*B, fruit_y*B);
      window.draw(sprite_green);
      ss.str(std::string());
      ss<<"score "<<tail_x.size()<<"        best score "<<best_score;
      text.setString(ss.str());
      window.draw(text);
      window.display();
    }
  }
  return 0;
}
