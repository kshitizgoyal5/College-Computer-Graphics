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
	double x_equal;
	int radius;
	vector<vector<int>> circle;
	vector<vector<double>> spokes;
	vector<vector<int>> fillColor;
	CircleDrawing(int x, int y);
	void createCircle(int x, int y);
	void translate(int x, int y);
	void createSpokes();

private:
	
};

