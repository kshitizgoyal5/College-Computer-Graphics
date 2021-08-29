#include"header.h"

graph :: graph(int n)
	:nodes(n){
	edges = vector<set<int> >(n);
}

void graph ::addEdge(int x, int y) {
	if (x < nodes && y < nodes) {
		edges[x].insert(y);
	}
}

vector<int> graph :: BFS(int start) {
	queue<int> Q;
	vector<int> order;
	if (start >= nodes) return order;
	vector<bool> unvisited(nodes, true);
	Q.push(start);
	while (!Q.empty()) {
		int size = Q.size();
		for (int i = 0; i < size; i++) {
			int node = Q.front();
			Q.pop();
			if (unvisited[node]) {
				unvisited[node] = false;
				order.push_back(node);
				for (auto itr = edges[node].begin(); itr != edges[node].end(); ++itr) {
					if(unvisited[*itr])
						Q.push(*itr);
				}
			}
		}
	}
	if (order.size() != nodes) return vector<int>(0);
	return order;
}

bool graph::DFS_Temp(int start, vector<bool> &visited, vector<int> &order) {
	if (order.size() == nodes) return true;
	if (visited[start] == true) return false;
	visited[start] = true;
	order.push_back(start);
	for (auto itr = edges[start].begin(); itr != edges[start].end(); ++itr) {
		if (DFS_Temp(*itr, visited, order)) return true;
	}
	return false;
}

vector<int> graph::DFS(int start) {
	vector<bool> visited(nodes, false);
	vector<int> order;
	DFS_Temp(start, visited, order);
	return order;
}
