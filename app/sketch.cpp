#include "sketch.hpp"
#include <iostream>
#include <cstdlib> // for rand()
#include <ctime> // for rand seed
#include <cmath>

Sketch::Sketch()
    : title("Sketch"), windowSize(800, 600), width(800), height(600),
      fillColor(sf::Color::Black), permTable(TABLE_SIZE) {

  window.create(sf::VideoMode(windowSize), title,
                sf::Style::Titlebar | sf::Style::Close);
  background(255);
}

// SKETCH SETTINGS

void Sketch::noLoop() { this->noLoopFlag = true; } // not very functional
void Sketch::frameRate(float fps) { timePerFrame = sf::seconds(1.0f / fps); }

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
  sf::CircleShape circle(extent);
  circle.setFillColor(fillColor);
  circle.setPosition(sf::Vector2f(x, y));
  window.draw(circle);
}
void Sketch::triangle(float x1, float y1, float x2, float y2, float x3,
                      float y3) {
  sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);
  triangle[0].position = sf::Vector2f(x1, y1);
  triangle[1].position = sf::Vector2f(x2, y2);
  triangle[2].position = sf::Vector2f(x3, y3);
  triangle[0].color = fillColor;
  triangle[1].color = fillColor;
  triangle[2].color = fillColor;
  window.draw(triangle);
}
void Sketch::line(float x1, float y1, float x2, float y2) {
  sf::Vertex line[2] = {{.position = sf::Vector2f(x1, y1), .color = fillColor},
                        {.position = sf::Vector2f(x2, y2), .color = fillColor}};
  window.draw(line, 2, sf::PrimitiveType::Lines);
}
void Sketch::point(float x, float y) {
  sf::Vertex point[1] = {{.position = sf::Vector2f(x, y), .color = fillColor}};
  window.draw(point, 1, sf::PrimitiveType::Points);
}
void Sketch::beginShape(std::string shape) {
  if (shape == "LineStrip") {
    sf::VertexArray lines(sf::PrimitiveType::LineStrip);
    this->vertexArray = lines;
  } else if (shape == "TriangleStrip") {
    sf::VertexArray tris(sf::PrimitiveType::TriangleStrip);
    this->vertexArray = tris;
  }
}
void Sketch::vertex(float x, float y) {
  vertexArray.append(sf::Vertex{{x, y}, fillColor});
}
void Sketch::endShape() {
  window.draw(vertexArray);
  this->vertexArray.clear();
}

// other functions
float Sketch::lerp(float a, float b, float t) {
  return a + t * (b - a);
}
float Sketch::fade(float t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

float Sketch::noise(float x) {
  if (permTable[0] == 0 && permTable[1] == 0 && permTable[255] == 0) {
    initPermTable();
  }

  // split x into int and decimal parts
  int xi = static_cast<int>(floor(x));
  float fx = x - floor(x);

  int g0 = permTable[xi % TABLE_SIZE];
  int g1 = permTable[(xi + 1) % TABLE_SIZE];
  
  float grad0 = perlinGradient1D(g0);
  float grad1 = perlinGradient1D(g1);

  float n0 = grad0 * fx; // "dot products" but just multiplication in 1d
  float n1 = grad1 * (fx - 1);

  float u = fade(fx);

  return lerp(n0, n1, u);
}
float Sketch::noise(float x, float y) {
  if (permTable[0] == 0 && permTable[1] == 0 && permTable[255] == 0) {
    initPermTable();
  }

  // split x into int and decimal parts
  int xi = static_cast<int>(floor(x));
  int yi = static_cast<int>(floor(y));
  float fx = x - floor(x);
  float fy = y - floor(y);

  int aa = permTable[(xi + permTable[yi % TABLE_SIZE]) % TABLE_SIZE];
  int ab = permTable[(xi + permTable[(yi + 1) % TABLE_SIZE]) % TABLE_SIZE];
  int ba = permTable[((xi + 1) + permTable[yi % TABLE_SIZE]) % TABLE_SIZE];
  int bb = permTable[((xi + 1) + permTable[(yi + 1) % TABLE_SIZE]) % TABLE_SIZE];

  sf::Vector2f gAA = perlinGradient2D(aa);
  sf::Vector2f gBA = perlinGradient2D(ba);
  sf::Vector2f gAB = perlinGradient2D(ab);
  sf::Vector2f gBB = perlinGradient2D(bb);

  float dotAA = gAA.x * fx + gAA.y * fy;
  float dotBA = gBA.x * (fx - 1) + gBA.y * fy;
  float dotAB = gAB.x * fx + gAB.y * (fy - 1);
  float dotBB = gBB.x * (fx - 1) + gBB.y * (fy - 1);


}
void Sketch::initPermTable() {
  srand(time(nullptr));
  for (int i = 0; i < TABLE_SIZE; i++) {
    permTable[i] = rand() % 256;
  }
}
float Sketch::perlinGradient1D(int hash) {
  return (2.0 * (hash / 255.0) - 1.0);
}
sf::Vector2f Sketch::perlinGradient2D(int hash) {
  float angle = (hash / 255.0f) * 2 * M_PI;  // angles between 0 and 2pi
  return { cos(angle), sin(angle) };  // unit vector pointing in a random direction
}

// PROCESSING default methods

void Sketch::setup() {}
void Sketch::draw() {}
void Sketch::keyPressed(sf::Keyboard::Key key) {}
void Sketch::keyReleased(sf::Keyboard::Key key) {}
void Sketch::mousePressed() {}
void Sketch::mouseReleased() {}
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
    if (firstFrame ||
        (!noLoopFlag && frameTimer.getElapsedTime() >= timePerFrame)) {
      frameTimer.restart();
      firstFrame = false;

      draw();
      window.display();

      // shows frametime! remember to include iostream
      // std::cout << "Frame drawn at time: " <<
      // frameTimer.getElapsedTime().asMilliseconds() << "ms" << std::endl;
    } else if (noLoopFlag) {
      frameTimer.restart();
      window.display();
    }

    // Small yield to prevent CPU hogging
    sf::sleep(sf::milliseconds(1));
  }
}
void Sketch::exit() { window.close(); }