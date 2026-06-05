#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <deque>
#include <cmath>
#include <sstream>
#include <climits>
#include <string>

using namespace std;

class Graph_M
{
public:
    class Vertex
    {
    public:
        unordered_map<string, int> neighbours;
    };

    unordered_map<string, Vertex> vtces;

    Graph_M();
    int numVertex();
    bool containsVertex(string vname);
    void addVertex(string vname);
    void removeVertex(string vname);
    int numEdges();
    bool containsEdge(string vname1, string vname2);
    void addEdge(string vname1, string vname2, int value);
    void display_Map();
    void display_Stations();
    bool hasPath(string vname1, string vname2, unordered_map<string, bool> &processed);

    // Dijkstra Helpers
    class DijkstraPair
    {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair &other) const
        {
            return cost > other.cost;
        }
    };

    int dijkstra(string src, string des, bool nan);

    // Path finding helpers
    class Pair
    {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string Get_Minimum_Distance(string src, string dst);
    string Get_Minimum_Time(string src, string dst);
    vector<string> get_Interchanges(string str);
    vector<int> Get_Minimum_Fare(string src, string dst, int x);
    
    // Static helper to populate the map
    static void Create_Metro_Map(Graph_M &g);
};

#endif
