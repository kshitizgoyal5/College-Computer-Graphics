#pragma once
#include<iostream>
using namespace std;
#include<vector>

vector<vector<double> > simpleDDA(double x, double y, double X, double Y);

vector<vector<double> > symmerticalDDA(double x, double y, double X, double Y);

vector<vector<int> > bresenhamLDA(double x, double y, double X, double Y);

vector<vector<int> > midPointLDA(double x, double y, double X, double Y);