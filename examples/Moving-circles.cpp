#include "CSketch.hpp"
#include <cmath>

float myTime;
float step;
float radius;
float centerY;
float iDivisor;
int lookahead;

void CSketch::setup() {
	smooth(8);
	fill(225, 120, 80);
	frameRate(60);
	myTime = 0;
	step = 0.03;
	radius = 10;
	centerY = height / 2 - radius;
	lookahead = 1; // has a maximum line length that never extends past amplitude
	
	//iDivisor = 1.342940394;
	iDivisor = 2.342425;
	//iDivisor = 23.577123;
}
void CSketch::draw() {
	background(30);
	
	for (int i = 0; i < width; i+= radius * 2) {
		circle(i, centerY + std::sin(myTime + i / iDivisor) * height / 4, radius);
		line(i + radius, centerY + std::sin(myTime + i / iDivisor) * height / 4, i + radius, height / 2 - 5 + std::sin(myTime + step * lookahead + (i + lookahead) / iDivisor) * height / 4);
	}
	myTime += step;
}

