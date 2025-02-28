#include "sketch.hpp"

class CSketch : public Sketch {
public:
  void setup() override;
  void draw() override;
  void keyPressed(sf::Keyboard::Key key) override {}
  void keyReleased(sf::Keyboard::Key key) override {}
  void mousePressed() override {}
  void mouseReleased() override {}
};