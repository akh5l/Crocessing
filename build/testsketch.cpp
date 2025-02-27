#include "testsketch.hpp"
#include <iostream>
#include <random>

void TestSketch::setup() {
  size(1200, 800);
  frameRate(60);
  smooth(8);
  windowTitle("Test Sketch");
}

void TestSketch::draw() {
  background(255);
  fill(255, 0, 0);
  for (int i = -5; i < static_cast<int>(windowSize.x); i += 10) {
    for (int j = -5; j < static_cast<int>(windowSize.y); j += 10) {
      line(i, j, mouseX, mouseY);
    }
  }
}