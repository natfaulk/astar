#include <iostream>
#include <SFML/Graphics.hpp>

#include "point.hpp"
#include "astar.hpp"
#include "constants.hpp"
#include "map.hpp"
#include "utils.hpp"
#include "checkbox.hpp"

int frameCount = 0;
const int FRAME_DIVIDER = 60 / 10;
bool done = false;

int main(int argc, char const *argv[]) {
  /* code */
  UTIL_RandSeed();
  // std::cout << "Hello world!\r\n";

  sf::Font font;
  if (!font.loadFromFile("res/Roboto-Regular.ttf")) {
    std::cout << "Failed to load font. Exiting..." << std::endl;
    return 1;
  }

  Checkbox testChk(5, 5, false, "Select for timelapse");

  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_TITLE);
  window.setFramerateLimit(60);

  // ASTAR_Test();
  MAP_Clear();
  MAP_GenRandom();

  int xpos = UTIL_RandBetween(1, MAP_WIDTH - 2);
  int ypos = UTIL_RandBetween(1, MAP_HEIGHT - 2);
  Point src(xpos, ypos);

  xpos = UTIL_RandBetween(1, MAP_WIDTH - 2);
  ypos = UTIL_RandBetween(1, MAP_HEIGHT - 2);
  Point dest(xpos, ypos);

  // MAP_Gen();

  // astar(&src, &dest);
  // ASTAR_PrintOpen();
  // MAP_Print();

  ASTAR_Begin(&src, &dest);
  MAP_SetData(&src, SRC);
  MAP_SetData(&dest, DEST);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::R) {
            done = false;
            MAP_Clear();
            MAP_GenRandom();

            xpos = UTIL_RandBetween(1, MAP_WIDTH - 2);
            ypos = UTIL_RandBetween(1, MAP_HEIGHT - 2);
            src = Point(xpos, ypos);

            xpos = UTIL_RandBetween(1, MAP_WIDTH - 2);
            ypos = UTIL_RandBetween(1, MAP_HEIGHT - 2);
            dest = Point(xpos, ypos);

            ASTAR_Begin(&src, &dest);
            MAP_SetData(&src, SRC);
            MAP_SetData(&dest, DEST);
          }

          if (event.key.code == sf::Keyboard::P) {
            ASTAR_Test();
            while(1);
          }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
          sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
          if (testChk.isMouseInside(mousePosition.x, mousePosition.y)) testChk.onClick();
        }
    }

    if (!testChk.value() && !done)
    {
      while (!ASTAR_Step())
      {
        MAP_SetData(ASTAR_LatestSearched(), SEARCHED);
      }

      Point nextPoint = ASTAR_PathNextPoint();
      while(nextPoint.x >= 0)
      {
        MAP_SetData(&nextPoint, BEST);
        nextPoint = ASTAR_PathNextPoint();
      }
      MAP_SetData(&src, SRC);
      MAP_SetData(&dest, DEST);
      done = true;
    }
    else if (frameCount++ >= FRAME_DIVIDER)
    {
      frameCount = 0;
      if (!ASTAR_Step())
      {
        MAP_SetData(ASTAR_LatestSearched(), SEARCHED);
      }
      else if(!done)
      {
        Point nextPoint = ASTAR_PathNextPoint();
        while(nextPoint.x >= 0)
        {
          MAP_SetData(&nextPoint, BEST);
          nextPoint = ASTAR_PathNextPoint();
        }
        MAP_SetData(&src, SRC);
        MAP_SetData(&dest, DEST);
        done = true;
      }

      MAP_Draw(window);
      testChk.draw(window, font);
      window.display();
    }


  }

  return 0;
}