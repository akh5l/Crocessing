#ifndef SKETCH_HPP
#define SKETCH_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>

class Sketch {
public:
  sf::Vector2u windowSize;
  int width;
  int height;
  int mouseX;
  int mouseY;
  std::string title = "Sketch";

  Sketch();
  void run();
  void exit();

  void windowTitle(std::string title);
  void size(int width, int height);
  void frameRate(float fps);

  virtual void setup();
  virtual void draw();
  virtual void keyPressed(sf::Keyboard::Key key);
  virtual void keyReleased(sf::Keyboard::Key key);
  virtual void mousePressed();
  virtual void mouseReleased();
  // void mouseDragged();
  // void mouseMoved();

  void background(int gray);
  void background(int r, int g, int b);
  void background(int r, int g, int b, int a);

  void smooth(int level);
  void noSmooth();

  void fill(int gray);
  void fill(int r, int g, int b);
  void fill(int r, int g, int b, int a);
  void circle(float x, float y, float extent);
  void line(float x1, float y1, float x2, float y2);
  

private:
  sf::RenderWindow window;
  sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
  sf::Color fillColor;
};

#endif