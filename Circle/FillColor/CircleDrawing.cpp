#include "CircleDrawing.h"

CircleDrawing::CircleDrawing() {
	X = 0;
	Y = 0;
	radius = 0;
	pattern = 15;
	gap = 1;
}

void CircleDrawing::createCircle(int x, int y) {
	circle.clear();
	fillColor.clear();
	radius = sqrt((x - X) * (x - X) + (y - Y) * (y - Y));
	int prevX = X, prevY = Y;
	translate(0, 0);
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
	/*dx = X;
	dy = Y;
	X = 0;
	Y = 0;
	seedX -= dx;
	seedY -= dy;*/
	translate(prevX, prevY);
};

void CircleDrawing::translate(int x, int y) {
	int dx = x - X, dy = y - Y;
	X = x;
	Y = y;
	for (int i = 0; i < circle.size(); i++) {
		circle[i][0] += dx;
		circle[i][1] += dy;
	}
	seedX += dx;
	seedY += dy;
	for (int i = 0; i < fillColor.size(); i++) {
		fillColor[i][0] += dx;
		fillColor[i][1] += dy;
	}
}



