
#include <cmath>
#include <vector>
#include <iostream>

struct Branch {
    float x1, y1, x2, y2;  // Start and end points of a branch
    float angle;            // Angle of the branch
    float length;           // Length of the branch
    int depth;              // Depth of the branch
};

std::vector<Branch> branches;  // To store the branches
void CSketch::setup() {
	size(1024, 768);
	smooth(8);

    float x1 = width / 2.0f;
    float y1 = height;
    float angle = -M_PI / 2.0f;
    float length = 240.0f;
    int depth = 16;


    drawBranch(x1, y1, angle, length, depth);
}

void CSketch::draw() {
	background(255);

    for (const auto& branch : branches) {
        line(branch.x1, branch.y1, branch.x2, branch.y2);
    }
}

void CSketch::drawBranch(float x1, float y1, float angle, float length, int depth) {
    if (depth == 0) return;  // Stop recursion at a certain depth

    float x2 = x1 + cos(angle) * length;
    float y2 = y1 + sin(angle) * length;

    Branch branch = {x1, y1, x2, y2, angle, length, depth};
    branches.push_back(branch);

    drawBranch(x2, y2, angle - M_PI / 6.0f, length * 0.7f, depth - 1);  // Left branch
    drawBranch(x2, y2, angle + M_PI / 6.0f, length * 0.7f, depth - 1);  // Right branch
}
