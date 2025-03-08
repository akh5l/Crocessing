#include "CSketch.hpp"

// Written in Crocessing.
// github.com/akh5l/Crocessing

// Perlin Noise Line Visualization
#include <cmath>
#include <iostream>

float xOffset = 0.0;  // Starting position in noise space
float yNoiseScale = 0.01;  // Scale factor for y-noise (adjust as needed)
float xNoiseScale = 0.005;  // How quickly we move through the noise

void CSketch::setup() {
  size(1200, 800);
}

void CSketch::draw() {

  //background(255);
  beginShape("LineStrip");
  for (int x = 0; x < width; x += 5) {  // Step by 5 pixels for efficiency
    // Calculate noise value - replace with your noise function if needed
    float y = ((noise((x * yNoiseScale), xOffset) + 1) * 0.5) * height;
     //fill(40, 25, 25, std::fmod(y + height, 255));
     fill(90, 65, 45, 20);
    // Draw vertex at this position
    vertex(x, y);
  }
  endShape();
  
  // Move through the noise
  xOffset += 0.008;
}
