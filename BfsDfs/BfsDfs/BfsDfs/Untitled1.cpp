#include<iostream>
#include<vector>
#include<queue>
#include<conio.h>

using namespace std;

class Graph{
    int vertices;
    vector<vector <int> > edges;
public:
    Graph(int v) {
        this->vertices = v;
        edges = vector<vector <int> >(v);
    }

    void add_edge(int x, int y) {
        edges[x].push_back(y);
        edges[y].push_back(x);
    }

    vector<int> bfs(int src) {
        queue<int> q;
        vector<int> result;
        if(src>=vertices) return result;
        vector<bool> unvisited(vertices, true);
        q.push(src);
        while(!q.empty()) {
            int size;
            size = q.size();
            for (int i = 0; i < size; i++) {
                int vertex;
                vertex = q.front();
                q.pop();
                if (unvisited[vertex]) {
                    unvisited[vertex] = false;
                    result.push_back(vertex);
                }
                for(int i=0;i<edges.size();i++) {
                    vector<int> :: iterator it;
                    for (it = edges[i].begin(); it != edges[i].end(); ++it) {
                        if(unvisited[*it]) {
                            q.push(*it);
                        }
                    }
                    
                }
                
            }
            
        }
        if (result.size() != vertices) return vector<int>(0);
        return result;

    }

    bool dfs_helper(int src, vector<bool> &visited, vector<int> &result) {
        if(result.size() == vertices) return true;
        if(visited[src] == true) return false;
        visited[src] = true;
        result.push_back(src);
        for(int i=0;i<edges.size();i++) {
            vector<int> :: iterator it;
            for (it = edges[i].begin(); it != edges[i].end(); ++it) {
                if(dfs_helper(*it, visited, result)) return true;
            }
        }
        return false;
    }


    vector<int> dfs(int src) {
        vector<bool> visited(vertices, false);
        vector<int> result;
        dfs_helper(src, visited, result);
        return result;
    }
    
};


int main() {
    int v, src, a, b, choice;
    Graph g(v);
    cout<<"******Welcome to graph implementation******\n\n";
    cout<<"Enter the number of vertices of the graph: ";
    cin>>v;
    cout<<"\nEnter the edges of the graph: ";
    for(int i=0;i<v;i++) {
        cin>>a>>b;
        g.add_edge(a,b);
    }
    cout<<"\nEnter the starting vertex or the source vertex: ";
    cin>>src;
    cout<<"Select a procedure:\n";
    cout<<"1. BFS - Breadth First Search\n";
    cout<<"2. DFS - Depth First Search\n";
    cin>>choice;
    vector<int> result;
    switch (choice)
    {
    case 1:
        result = g.bfs(src);
        break;
    
    case 2:
        result = g.dfs(src);
        break;

    default:
        break;
    }
    cout<<"The Traversal Path is: ";
    for(int i: result){
    	cout<<i<<" ";
	}
    return 0;
}
