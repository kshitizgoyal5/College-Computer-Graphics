#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<set>
#include<string>
#include <conio.h> 
using namespace std;

class graph {
private:
	int nodes;
	vector<set<int> > edges;
public:
	graph(int n);

	void addEdge(int x, int y);

	vector<int> BFS(int start);

	bool DFS_Temp(int start, vector<bool>& visited, vector<int>& order);

	vector<int> DFS(int start);
};