#include "CSketch.hpp"

// Written in Crocessing.
// github.com/akh5l/Crocessing


float t;
float amplitude;

void CSketch::setup() {
	t = 0;
	amplitude = 80;
	smooth(4);
}
void CSketch::draw() {
	background(30);
	fill(235); // needs to be changed to stroke later
	beginShape("LineStrip");
		for (int i = 0; i < width; i += 2) {
			vertex(i, height / 2 + noise(t + i * 0.01) * amplitude);
		}
	endShape();
	t += 0.03;
}

