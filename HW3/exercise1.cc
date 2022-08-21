#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <limits>


using namespace std;

double MAX_DOUBLE = 1.79e+308;

// Data structure to store a graph edge
struct Edge {
    int src, dest;
    float weight;
};

typedef pair<float, float> Pair;

// A class to represent a graph object
class Graph
{
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges, float n)
    {
        // resize the vector to hold `n` elements of type vector<Edge>
        adjList.resize(n);

        // add edges to the directed graph
        for (auto &edge: edges)
        {
            int src = edge.src;
            int dest = edge.dest;
            float weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));

            // uncomment the following code for undirected graph
            // adjList[dest].push_back(make_pair(src, weight));
        }
    }
};

// Function to print adjacency list representation of a graph
void printGraph(Graph const &graph, int n)
{
    for (int i = 0; i < n; i++)
    {
        // Function to print all neighboring vertices of a given vertex
        for (Pair v: graph.adjList[i]) {
            cout << "(" << i << ", " << v.first << ", " << v.second << ") ";
        }
        cout << endl;
    }
}

void printPath(int parent[], int j)
{
    // Base Case : If j is source
    if (parent[j] == -1)
        return;
    printPath(parent, parent[j]);
    cout << j << " ";
}


//-----------------------------------------------------------------------------------------------------------------

int minDistance(double dist[], bool sptSet[])
{
    // Initialize min value
    double minValue = MAX_DOUBLE;
    int min_index;
    for (int i = 0; i < 11; i++)
        if (sptSet[i] == false && dist[i] <= minValue)
            minValue = dist[i], min_index = i;
    return min_index;
}
void dijkstra(double graph[11][11], int src)
{
    int parent[11];
    parent[src] = -1;

    double dist[11];
    bool sptSet[11];


    for (int i = 0; i < 11; i++) {
        dist[i] = MAX_DOUBLE;
        sptSet[i] = false;
    }
    // Distance of source vertex from itself is always 0
    dist[src] = graph[src][src];

    // Find shortest path for all vertices
    for (int count = 0; count < 11 - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in first iteration.
        int u = minDistance(dist, sptSet);
        // Mark the picked vertex as processed
        sptSet[u] = true;
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < 11; v++)
            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v]
                && dist[u] + graph[u][v] <= dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }


    }

    // Post process:
    vector<int> shooters;
    bool srcIsShooter= graph[src][src] !=0;
    for(int i=0; i< 11; i++){

        if(graph[i][i] != 0){
            shooters.push_back(i);
            if(!srcIsShooter) {
                dist[i] = dist[i] * graph[i][i] * -1;
            }
        }
    }



    int minShooter = shooters[0];
    for(int shooter: shooters){
        if(dist[shooter] < dist[minShooter]){
            minShooter = shooter;
        }
    }

   cout << src << " " ;
   printPath(parent,minShooter);
   cout << endl;
}




int main(int argc, char** argv) {


        string path = argv[1];
        int s = atoi(argv[2]);
        

        vector<vector<float>> content;
        vector<float> row;
        string line, word;
        vector<Edge> edges;
        vector<Edge> edges_prepro;

//    int dist[11];
//
        double pre_matrix[11][11];

        ifstream file;
        file.open(path, ios::in);
        if (file.is_open()) {
            while (getline(file, line)) {
                row.clear();

                stringstream str(line);

                while (getline(str, word, ','))
                    row.push_back(std::stof(word));
                content.push_back(row);
            }
        } else
            cout << "Could not open the file\n";

        for (int i = 0; i < content.size(); i++) {
            for (int j = 0; j < content[i].size(); j++) {
                if(j!=10)
                    cout << content[i][j] << ",";
                else
                    cout << content[i][j];
            };
            cout << "\n";
        }


        //construct weighted graph
        for (int i = 0; i < content.size(); i++) {
            for (int j = 0; j < content[i].size(); j++) {
                struct Edge edge;
                edge.src = i;
                edge.dest = j;
                edge.weight = content[i][j];
                edges.push_back(edge);

            }
        }

        int n = 11;

        // construct graph
        //Graph graph(edges, n);
        //printGraph(graph,n);



        for (int i = 0; i < content.size(); i++) {
            for (int j = 0; j < content[i].size(); j++) {
                struct Edge edge;
                edge.src = i;
                edge.dest = j;
                if (content[i][j] != 0) {
                    if (i == j) {
                        edge.weight = content[i][j];
                    } else {
                        edge.weight = log(content[i][j]) / log(10) * (-1);
                    }
                } else {
                    edge.weight = 0;
                }
                edges_prepro.push_back(edge);
            }
        }


        for (int i = 0; i < content.size(); i++) {
            for (int j = 0; j < content[i].size(); j++) {
                if (i == j) {
                    pre_matrix[i][j] = content[i][j];
                } else {
                    pre_matrix[i][j] = log(content[i][j]) * (-1);
                }
            }
        }



        // construct graph
//    Graph graph_prepro(edges_prepro, n);
//    printGraph(graph_prepro,n);


        dijkstra(pre_matrix, s);
        file.close();

        return 0;
}
