#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include <cmath>

vector<vector<double> > simpleDDA(double x, double y, double X, double Y) {
	double delX = double(X - x), delY = double(Y - y);
	double step = max(abs(delX), abs(delY));
	double xInc = delX / step, yInc = delY / step;
	vector<vector<double> > line(step+1, {0, 0});
	line[0][0] = x;
	line[0][1] = y;
	for (int i = 0; i < step; i++) {
		line[i + 1][0] = (line[i][0] + xInc);
		line[i + 1][1] = (line[i][1] + yInc);
	}
	for (int i = 0; i < step + 1; i++) {
		line[i][0] = round(line[i][0]);
		line[i][1] = round(line[i][1]);
	}
	return line;
}

vector<vector<double> > symmerticalDDA(double x, double y, double X, double Y) {
	double delX = double(X - x), delY = double(Y - y);
	double step = max(abs(delX), abs(delY));
	int n = ceil(log2(step));
	int N = (1<<n);
	double xInc = delX / (N), yInc = delY / (N);
	vector<vector<double> > line(1 << n + 1, { 0, 0 });
	line[0][0] = x;
	line[0][1] = y;
	for (int i = 0; i < 1 << n; i++) {
		line[i + 1][0] = (line[i][0] + xInc);
		line[i + 1][1] = (line[i][1] + yInc);
	}
	for (int i = 0; i < step + 1; i++) {
		line[i][0] = round(line[i][0]);
		line[i][1] = round(line[i][1]);
	}
	return line;
}

vector<vector<double> > bresenhamLDA(double x, double y, double X, double Y) {
	double delX = double(X - x), delY = double(Y - y);
	if (abs(delX) > abs(delY)) {
		double d = 2 * abs(delY) - abs(delX);
		vector<vector<double> > line(abs(delX) + 1, { 0, 0 });
		line[0][0] = x;
		line[0][1] = y;
		for (int i = 0; i < abs(delX); i++) {
			if (x < X) x++;
			else x--;
			if (d < 0) {
				d += 2 * abs(delY);
			}
			else {
				if (y > Y) y--;
				else y++;
				d += 2 * (abs(delY) - abs(delX));
			}
			line[i + 1][0] = x;
			line[i + 1][1] = y;
		}
		return line;
	}
	else {
		double d = 2 * abs(delX) - abs(delY);
		vector<vector<double> > line(abs(delY) + 1, { 0, 0 });
		line[0][0] = x;
		line[0][1] = y;
		for (int i = 0; i < abs(delY); i++) {
			if (y < Y) y++;
			else y--;
			if (d < 0) {
				d += 2 * abs(delX);
			}
			else {
				if (x < X) x++;
				else x--;
				d += 2 * (abs(delX) - abs(delY));
			}
			line[i + 1][0] = x;
			line[i + 1][1] = y;
		}
		return line;
	}
}

vector<vector<double> > midPointLDA(double x, double y, double X, double Y) {
	double delX = abs(X - x), delY = abs(Y - y);
	if ((delX) > (delY)) {
		if (x > X) {
			swap(x, X);
			swap(y, Y);
		}
		double d = delY - delX / 2;
		vector<vector<double> > line(delX + 1, { 0, 0 });
		line[0][0] = x;
		line[0][1] = y;
		for (int i = 0; i < delX; i++) {
			x++;
			d += delY;
			if (d >= 0) {
				d -= delX;
				if (Y > y) y++;
				else y--;
			}
			line[i + 1][0] = x;
			line[i + 1][1] = y;
		}
		return line;
	}
	else {
		if (y > Y) {
			swap(y, Y);
			swap(x, X);
		}
		double d = delX - delY / 2;
		vector<vector<double> > line(delY + 1, { 0, 0 });
		line[0][0] = x;
		line[0][1] = y;
		for (int i = 0; i < delY; i++) {
			y++;
			d += delX;
			if (d >= 0) {
				d -= delY;
				if (X > x) x++;
				else x--;
			}
			line[i + 1][0] = x;
			line[i + 1][1] = y;
		}
		return line;
	}
}

