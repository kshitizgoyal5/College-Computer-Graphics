#include "CircleDrawing.h"
int temp = 0;
CircleDrawing::CircleDrawing(int x, int y) {
	X = x;
	Y = y;
	x_equal = 0;
	radius = 0;
	createCircle(x + 60, y);
}

void CircleDrawing::createCircle(int x, int y) {
	circle.clear();
	radius = sqrt((x - X) * (x - X) + (y - Y) * (y - Y));
	int dx = 0, dy = radius;
	int p = 1 - radius;
	circle.push_back({ dx, dy});
	while (dx < dy) {
		if (p <= 0) {
			p = p + 2 * dx + 1;
		}
		else {
			p = p + 2 * dx - 2 * dy + 1;
			dy--;
		}
		dx++;
		circle.push_back({ dx, dy});
	}
	x_equal = circle[circle.size() - 1][0];
	int s = circle.size();
	for (int i = s-1; i >=0 ; i--) {
		circle.push_back({ circle[i][1], circle[i][0] });
	}
	s = circle.size();
	for (int i = 0; i < s; i++) {
		circle.push_back({ circle[i][0], -circle[i][1] });
	}
	s = circle.size();
	for (int i = s-1; i >=0 ; i--) {
		circle.push_back({ -circle[i][0], circle[i][1] });
	}
	s = circle.size();
	for (int i = s - 1; i >= 0; i--) {
		circle[i][0] += X;
		circle[i][1] += Y;
	}
	createSpokes();
};

void CircleDrawing::translate(int x, int y) {
	int dx = x - X, dy = y - Y;
	x_equal += x;
	X = x;
	Y = y;
	for (int i = 0; i < circle.size(); i++) {
		circle[i][0] += dx;
		circle[i][1] += dy;
	}
	for (int i = 0; i < spokes.size(); i++) {
		spokes[i][0] += dx;
		spokes[i][1] += dy;
	}
}

void CircleDrawing::createSpokes(){
	spokes.clear();
	vector<vector<double>> temp;
	cout << x_equal << endl;
	temp.push_back({ x_equal       + X, x_equal      + Y});
	temp.push_back({ -x_equal      + X, x_equal      + Y});
	temp.push_back({ x_equal       + X, -x_equal     + Y});
	temp.push_back({ -x_equal      + X, -x_equal     + Y});
	temp.push_back({ 1.0 * radius  + X, 1.0 * 0      + Y});
	temp.push_back({-1.0 * radius  + X, 1.0 * 0      + Y});
	temp.push_back({ 1.0 * 0       + X, 1.0 * radius + Y});
	temp.push_back({ 1.0 * 0       + X,-1.0 * radius + Y});
	for (int i = 0; i < 8; i++) {
		vector<vector<double>> temp1 = bresenhamLDA(X, Y, temp[i][0], temp[i][1]);
		for (int j = 0; j < temp1.size();j++) {
			spokes.push_back({temp1[j][0], temp1[j][1]});
		}
	}
}


