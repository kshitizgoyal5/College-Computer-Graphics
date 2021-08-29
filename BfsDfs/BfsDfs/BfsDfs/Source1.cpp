#include "header.h"
int exceptionHandling() {
	cin.clear();
	cin.get();
	cout << "\n Wrong Input\n Press Enter and Choose Again"<<flush;
	char c = _getch();
	system("CLS");
	return 0;
}

int main() {
	bool running = true;
	while (running) {
		int n, t, x, y, d, start, algo;
		string welcome =
			"\t\t\t\t...........BFS / DFS...........\n"
			" 1 > Enter Number Of Nodes\n"
			" 2 > Enter Edges (Directed:1 Undirected:0)\n"
			" 3 > Enter Starting Node\n"
			" 4 > Choose BFS or DFS\n\n\n";
		cout << welcome;
		string s1 = "\n Nodes : ";
		cout << s1;
		welcome += s1;
		while (! (cin >> n)) {
			exceptionHandling();
			cout << welcome;
			continue;
		}
		welcome += char(n + '0');
		graph g(n);
		s1 = "\n Number Of Edges : ";
		cout << s1;
		welcome += s1;

		while (! (cin >> t)) {
			exceptionHandling();
			cout << welcome;
			continue;
		}
		welcome += char(t + '0');
		for (int i = 1; i <= t; i++) {
			cout << "\n Enter Edge " << i << " (x y directed) : ";
			welcome += "\n Enter Edge " + to_string(i) + " (x y directed) : ";
			while (!(cin >> x >> y >> d)) {
				exceptionHandling();
				cout << welcome;
				continue;
			}
			welcome += to_string(x) + " " + to_string(y) + " " + to_string(d);
			g.addEdge(x, y);
			if (!d) g.addEdge(y, x);
		}
		cout << "\n Starting Node : ";
		welcome += "\n Starting Node : ";
		while (!(cin >> start) && start >= n) {
			exceptionHandling();
			cout << welcome;
			continue;
		}
		welcome += to_string(start);

		cout << "\n Press 1 for BFS or Press 2 for Dfs : ";
		welcome += "\n Press 1 for BFS or Press 2 for Dfs : ";
		while (!(cin >> algo)) {
			exceptionHandling();
			cout << welcome;
			continue;
		}
		welcome += to_string(algo);
		vector<int> order = algo == 1? g.BFS(start) : g.DFS(start);
		if (order.size() == 0) cout << "\n No Possible Path Possible\n";
		else {
			cout << "\n Order : ";
			for (int node : order) {
				cout << node << " ";
			}
			cout << endl;
		}
		cout << "\n TO EXIT PRESS c \t\t\t TO CONTINUE PRESS ANY KEY";
		char c;
		c = _getch();
		if (c == 'c') running = false;
		system("CLS");

	}
	return 0;
}
