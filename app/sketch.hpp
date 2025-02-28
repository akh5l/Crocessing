#ifndef SKETCH_HPP
#define SKETCH_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>

class Sketch {
public:
  int width;
  int height;
  int mouseX;
  int mouseY;

  Sketch();
  void run();
  void exit();
  
  // default methods
  virtual void setup();
  virtual void draw();
  virtual void keyPressed(sf::Keyboard::Key key);
  virtual void keyReleased(sf::Keyboard::Key key);
  virtual void mousePressed();
  virtual void mouseReleased();
  // void mouseDragged(); to do
  // void mouseMoved();

  // sketch settings
  void windowTitle(std::string title);
  void size(int width, int height);
  void frameRate(float fps);
  void noLoop();
  void background(int gray);
  void background(int r, int g, int b);
  void background(int r, int g, int b, int a);
  void fill(int gray);
  void fill(int r, int g, int b);
  void fill(int r, int g, int b, int a);
  void smooth(int level);
  void noSmooth();

  // geometry
  void rect(float x, float y, float width, float height);
  void circle(float x, float y, float extent);
  void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
  void beginShape(std::string shape);
  void endShape();
  void line(float x1, float y1, float x2, float y2);
  void point(float x, float y);
  void vertex(float x, float y);

  // other
  float noise(float x);
  float noise(float x, float y);
  float lerp(float a, float b, float t);

private:
  sf::RenderWindow window;  
  sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
  std::string title = "Sketch";
  sf::Vector2u windowSize;
  bool noLoopFlag = false;
  
  sf::Color fillColor;
  sf::VertexArray vertexArray;

  const int TABLE_SIZE = 256;
  std::vector<int> permTable;
  void initPermTable();
  float fade(float t);
  float perlinGradient1D(int hash);
  sf::Vector2f perlinGradient2D(int hash);
};

#endif