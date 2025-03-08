#include "CSketch.hpp"

// Written in Crocessing.
// github.com/akh5l/Crocessing

#include <cmath>

float myTime;
float step;
float radius;
float centerY;
float iDivisor;
int lookahead;
float y;

void CSketch::setup() {
	size(1920, 720);
	smooth(8);
	fill(225, 120, 80);
	frameRate(60);
	myTime = 0;
	step = 0.03;
	radius = 2.;
	centerY = height / 2 - radius;
	lookahead = 1; // has a maximum line length that never extends past amplitude
	
	//iDivisor = 1.0;
	//iDivisor = 1.342940394;
	iDivisor = 2.342425;
	//iDivisor = 23.577123;
}
void CSketch::draw() {
	background(30);
	for (int i = 0; i < width; i+= radius * 2) {
		y = centerY + std::sin(myTime + i / iDivisor) * height / 4;
		fill((y / height * 255.0), 120, y / height * 100);
		circle(i, y, radius);
		line(i + radius, y, i + radius, height / 2 - 5 + std::sin(myTime + step * lookahead + (i + lookahead) / iDivisor) * height / 4);
	}
	myTime += step;
	iDivisor += step / 50.;
}

