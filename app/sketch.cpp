#include "sketch.hpp"
#include <iostream>

Sketch::Sketch()
    : title(""), windowSize(800, 600), width(800), height(600),
      fillColor(sf::Color::Black) {

  window.create(sf::VideoMode(windowSize), title,
                sf::Style::Titlebar | sf::Style::Close);
  background(255);
}

void Sketch::run() {
  sf::Clock frameTimer;
  bool firstFrame = true;

  setup();
  
  while (window.isOpen()) {
    // Process events
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *keyPressed =
                 event->getIf<sf::Event::KeyPressed>()) {
        this->keyPressed(keyPressed->code);
      } else if (const auto *keyReleased =
                 event->getIf<sf::Event::KeyReleased>()) {
        this->keyReleased(keyReleased->code);
      } else if (event->is<sf::Event::MouseButtonPressed>()) {
        mousePressed();
      } else if (event->is<sf::Event::MouseButtonReleased>()) {
        mouseReleased();
      }
    }

    // Update mouse position
    mouseX = sf::Mouse::getPosition(window).x;
    mouseY = sf::Mouse::getPosition(window).y;

    // Basic FPS timing - draws immediately on first frame
    if (firstFrame || (!noLoopFlag && frameTimer.getElapsedTime() >= timePerFrame)) {
      frameTimer.restart();
      firstFrame = false;
      
      window.clear();
      draw();
      window.display();
      
      // shows frametime!
      // std::cout << "Frame drawn at time: " << frameTimer.getElapsedTime().asMilliseconds() << "ms" << std::endl;
    }
    
    // Small yield to prevent CPU hogging
    sf::sleep(sf::milliseconds(1));
  }
}

void Sketch::background(int gray) { window.clear(sf::Color(gray, gray, gray)); }
void Sketch::background(int r, int g, int b) {
  window.clear(sf::Color(r, g, b));
}
void Sketch::background(int r, int g, int b, int a) {
  window.clear(sf::Color(r, g, b, a));
}
void Sketch::fill(int gray) { this->fillColor = sf::Color(gray, gray, gray); }
void Sketch::fill(int r, int g, int b) { this->fillColor = sf::Color(r, g, b); }
void Sketch::fill(int r, int g, int b, int a) {
  this->fillColor = sf::Color(r, g, b, a);
}

void Sketch::windowTitle(std::string title) {
  this->title = title;
  window.setTitle(title);
}
void Sketch::size(int width, int height) {
  this->windowSize = sf::Vector2u(width, height);
  this->width = width;
  this->height = height;
  window.setSize(windowSize);
  sf::View view(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(width, height)));
  window.setView(view);
}
void Sketch::smooth(int level) {
  if (level < 0) {
    level = 0;
  } else if (level > 8) {
    level = 8;
  }
  sf::ContextSettings settings;
  settings.antiAliasingLevel = level;

  window.create(sf::VideoMode(windowSize), title,
                sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed,
                settings);
}
void Sketch::noSmooth() {
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 0;

  window.create(sf::VideoMode(windowSize), title,
                sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
}

// GEOMETRY/SHAPES
void Sketch::rect(float x, float y, float width, float height) {
  sf::RectangleShape rect;
  rect.setFillColor(fillColor);
  rect.setPosition(sf::Vector2f(x, y));
  rect.setSize(sf::Vector2f(width, height));
  window.draw(rect);
}
void Sketch::circle(float x, float y, float extent) {
  sf::CircleShape circle;
  circle.setFillColor(fillColor);
  circle.setRadius(extent);
  circle.setPosition(sf::Vector2f(x, y));
  window.draw(circle);
}
void Sketch::line(float x1, float y1, float x2, float y2) {
  sf::Vertex line[2] = {{.position = sf::Vector2f(x1, y1), .color = fillColor},
                        {.position = sf::Vector2f(x2, y2), .color = fillColor}};
  window.draw(line, 2, sf::PrimitiveType::Lines);
}

void Sketch::setup() {}
void Sketch::draw() {}
void Sketch::keyPressed(sf::Keyboard::Key key) {}
void Sketch::keyReleased(sf::Keyboard::Key key) {}
void Sketch::mousePressed() {}
void Sketch::mouseReleased() {}

void Sketch::noLoop() { this->noLoopFlag = true; }
void Sketch::frameRate(float fps) { timePerFrame = sf::seconds(1.0f / fps); }

void Sketch::exit() { window.close(); }