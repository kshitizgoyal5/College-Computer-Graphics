#pragma once

#include <iostream>
#include <utility> 
#include<vector>
#include<algorithm>
#include "lineDrawingalgos.h"
using namespace std;

class CircleDrawing
{
private:

public:
	int X, Y;
	int radius;
	int gap, pattern;
	int seedX, seedY;
	vector<vector<int>> circle;
	vector<vector<double>> fillColor;
	CircleDrawing();
	void createCircle(int x, int y);
	void translate(int x, int y);

private:
	
};

