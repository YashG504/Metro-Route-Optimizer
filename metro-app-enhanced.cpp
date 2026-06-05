#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>
#include <cstring>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// DIJKSTRA'S ALGORITHM - ENHANCED DSA IMPLEMENTATION
// ============================================================================
// 
// ALGORITHM OVERVIEW:
// Dijkstra's Algorithm finds the shortest path in a weighted graph
// TIME COMPLEXITY: O((V + E) log V) with priority queue
// SPACE COMPLEXITY: O(V)
//
// KEY FEATURES:
// - Greedy approach: Always selects the unvisited vertex with minimum distance
// - Uses priority queue (min-heap) for efficient vertex selection
// - Works only with non-negative weights
// - Path reconstruction enabled for showing actual routes
//
// IMPLEMENTATION STEPS:
// 1. Initialize all distances to INF except source (0)
// 2. Add all vertices to priority queue
// 3. While queue not empty:
//    a. Extract vertex with minimum distance
//    b. For each neighbor, update distance if shorter path found
//    c. Maintain parent tracking for path reconstruction
// ============================================================================

class Graph_M
{
public:
    class Vertex
    {
    public:
        unordered_map<string, int> neighbours;
    };

    static unordered_map<string, Vertex> vtces;

    // ========== JOURNEY HISTORY ==========
    struct JourneyLog
    {
        string source;
        string destination;
        int distance;
        int time;
        string timestamp;
    };
    static vector<JourneyLog> journeyHistory;

    Graph_M()
    {
        vtces.clear();
    }

    int numVetex()
    {
        return vtces.size();
    }

    bool containsVertex(string vname)
    {
        return vtces.count(vname) > 0;
    }

    void addVertex(string vname)
    {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(string vname)
    {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (auto it = vtx.neighbours.begin(); it != vtx.neighbours.end(); it++)
        {
            keys.push_back(it->first);
        }

        for (string key : keys)
        {
            Vertex nbrVtx = vtces[key];
            nbrVtx.neighbours.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges()
    {
        int count = 0;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            Vertex vtx = it->second;
            count += vtx.neighbours.size();
        }

        return count / 2;
    }

    bool containsEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].neighbours.count(vname2) == 0)
        {
            return false;
        }

        return true;
    }

    void addEdge(string vname1, string vname2, int value)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].neighbours.count(vname2) > 0)
        {
            return;
        }

        vtces[vname1].neighbours[vname2] = value;
        vtces[vname2].neighbours[vname1] = value;
    }

    void removeEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].neighbours.count(vname2) == 0)
        {
            return;
        }

        vtces[vname1].neighbours.erase(vname2);
        vtces[vname2].neighbours.erase(vname1);
    }

    void display_Map()
    {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << endl;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            string str = key + " =>" + "\n";
            Vertex vtx = it->second;
            for (auto it2 = vtx.neighbours.begin(); it2 != vtx.neighbours.end(); it2++)
            {
                string nbr = it2->first;
                str += "\t" + nbr + "\t";
                if (nbr.length() < 16)
                    str += "\t";
                if (nbr.length() < 8)
                    str += "\t";
                str += to_string(it2->second) + " km\n";
            }
            cout << str << endl;
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << endl;
    }

    void display_Stations()
    {
        cout << endl;
        cout << "╔════════════════════════════════════════════════════╗" << endl;
        cout << "║           DELHI METRO STATIONS                    ║" << endl;
        cout << "╚════════════════════════════════════════════════════╝" << endl;
        int i = 1;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            cout << i << ". " << key << endl;
            i++;
        }
        cout << endl;
    }

    // ========== STATION SEARCH WITH AUTOCOMPLETE ==========
    vector<string> searchStations(string query)
    {
        vector<string> results;
        transform(query.begin(), query.end(), query.begin(), ::tolower);
        
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string station = it->first;
            string stationLower = station;
            transform(stationLower.begin(), stationLower.end(), stationLower.begin(), ::tolower);
            
            if (stationLower.find(query) != string::npos)
            {
                results.push_back(station);
            }
        }
        return results;
    }

    bool hasPath(string vname1, string vname2, unordered_map<string, bool> &processed)
    {
        if (containsEdge(vname1, vname2))
        {
            return true;
        }

        processed[vname1] = true;

        Vertex vtx = vtces[vname1];
        for (auto it = vtx.neighbours.begin(); it != vtx.neighbours.end(); it++)
        {
            string nbr = it->first;
            if (!processed.count(nbr))
            {
                if (hasPath(nbr, vname2, processed))
                {
                    return true;
                }
            }
        }

        return false;
    }

    // ========== DIJKSTRA WITH PATH RECONSTRUCTION ==========
    class DijkstraResult
    {
    public:
        int cost;
        vector<string> path;
        bool found;
    };

    class DijkstraPair
    {
    public:
        string vname;
        int cost;

        bool operator<(const DijkstraPair &other) const
        {
            return cost > other.cost;
        }
    };

    DijkstraResult dijkstraWithPath(string src, string des, bool timeMode)
    {
        DijkstraResult result;
        result.found = false;
        result.cost = INT_MAX;

        // STEP 1: Initialize distances and parents
        unordered_map<string, int> dist;
        unordered_map<string, string> parent;
        priority_queue<DijkstraPair> pq;

        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            dist[it->first] = INT_MAX;
            parent[it->first] = "";
        }

        dist[src] = 0;
        pq.push({src, 0});

        // STEP 2: Dijkstra main loop
        while (!pq.empty())
        {
            DijkstraPair curr = pq.top();
            pq.pop();

            // Skip if already processed with shorter distance
            if (curr.cost > dist[curr.vname])
                continue;

            // Check if destination reached
            if (curr.vname == des)
            {
                result.found = true;
                result.cost = curr.cost;
                break;
            }

            // STEP 3: Relax edges (update distances)
            Vertex v = vtces[curr.vname];
            for (auto it = v.neighbours.begin(); it != v.neighbours.end(); it++)
            {
                string nbr = it->first;
                int edgeWeight = it->second;
                
                // Calculate new cost based on mode
                int newCost;
                if (timeMode)
                    newCost = curr.cost + 120 + 40 * edgeWeight; // 120s base + 40s per km
                else
                    newCost = curr.cost + edgeWeight;

                // Update if shorter path found
                if (newCost < dist[nbr])
                {
                    dist[nbr] = newCost;
                    parent[nbr] = curr.vname;
                    pq.push({nbr, newCost});
                }
            }
        }

        // STEP 4: Reconstruct path
        if (result.found)
        {
            string node = des;
            while (!node.empty())
            {
                result.path.push_back(node);
                node = parent[node];
            }
            reverse(result.path.begin(), result.path.end());
        }

        return result;
    }

    int dijkstra(string src, string des, bool nan)
    {
        int val = 0;
        vector<string> ans;
        unordered_map<string, int> map;

        priority_queue<DijkstraPair> pq;

        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            map[key] = INT_MAX;

            if (key == src)
            {
                map[key] = 0;
            }

            pq.push({key, map[key]});
        }

        while (!pq.empty())
        {
            DijkstraPair rp = pq.top();
            pq.pop();

            if (rp.vname == des)
            {
                val = rp.cost;
                break;
            }

            Vertex v = vtces[rp.vname];
            for (auto it = v.neighbours.begin(); it != v.neighbours.end(); it++)
            {
                string nbr = it->first;
                int oc = map[nbr];
                Vertex k = vtces[rp.vname];
                int nc;
                if (nan)
                    nc = rp.cost + 120 + 40 * k.neighbours[nbr];
                else
                    nc = rp.cost + k.neighbours[nbr];

                if (nc < oc)
                {
                    map[nbr] = nc;
                    pq.push({nbr, nc});
                }
            }
        }
        return val;
    }

    class Pair
    {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string Get_Minimum_Distance(string src, string dst)
    {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty())
        {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname))
            {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst)
            {
                int temp = rp.min_dis;
                if (temp < min)
                {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto it = rpvtx.neighbours.begin(); it != rpvtx.neighbours.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.neighbours[nbr];
                    stack.push_front(np);
                }
            }
        }
        ans = ans + to_string(min);
        return ans;
    }

    string Get_Minimum_Time(string src, string dst)
    {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty())
        {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname))
            {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst)
            {
                int temp = rp.min_time;
                if (temp < min)
                {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto it = rpvtx.neighbours.begin(); it != rpvtx.neighbours.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_time = rp.min_time + 120 + 40 * rpvtx.neighbours[nbr];
                    stack.push_front(np);
                }
            }
        }
        double minutes = ceil((double)min / 60);
        ans = ans + to_string(minutes);
        return ans;
    }

    vector<string> get_Interchanges(string str)
    {
        vector<string> arr;
        vector<string> stations;
        
        // Split the path string by double spaces
        size_t start = 0;
        size_t end = str.find("  ");
        while (end != string::npos)
        {
            string station = str.substr(start, end - start);
            if (!station.empty())
                stations.push_back(station);
            start = end + 2;
            end = str.find("  ", start);
        }
        // Add last station
        if (start < str.length())
        {
            string station = str.substr(start);
            if (!station.empty())
                stations.push_back(station);
        }
        
        // Extract interchange count from last element if it's a number
        int interchanges = 0;
        if (!stations.empty())
        {
            string lastElem = stations.back();
            bool isNum = !lastElem.empty() && all_of(lastElem.begin(), lastElem.end(), ::isdigit);
            if (isNum)
            {
                interchanges = stoi(lastElem);
                stations.pop_back();
            }
        }
        
        // Build result array with stations and interchange count
        for (const string& station : stations)
            arr.push_back(station);
        
        arr.push_back(to_string(interchanges));
        return arr;
    }

    vector<int> Get_Minimum_Fare(string src,string dst,int x)
    {
        int stations=0;
        int fare=0;

        if(x==7)
        {
            stations=get_Interchanges(Get_Minimum_Distance(src, dst)).size()-1;
        }

        else
        {
            stations=get_Interchanges(Get_Minimum_Time(src, dst)).size()-1;
        }

        if(stations>0 && stations<=3)
        fare=10;

        else if(stations>3 && stations<=7)
        fare=20;

        else if(stations>7 && stations<=11)
        fare=30;

        else if(stations>11 && stations<=20)
        fare=40;

        else
        fare=40+ (stations-20)*10;

        return {stations,fare};
    }

    // ========== LOAD FROM CONFIGURATION FILE ==========
    void loadFromFile(string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Configuration file not found: " << filename << endl;
            return;
        }

        string line;
        string mode = "";

        while (getline(file, line))
        {
            if (line.empty() || line[0] == '#') continue;

            if (line == "[VERTICES]")
            {
                mode = "VERTICES";
                continue;
            }
            if (line == "[EDGES]")
            {
                mode = "EDGES";
                continue;
            }

            if (mode == "VERTICES")
            {
                addVertex(line);
            }
            else if (mode == "EDGES")
            {
                size_t comma1 = line.find(',');
                size_t comma2 = line.find(',', comma1 + 1);
                string v1 = line.substr(0, comma1);
                string v2 = line.substr(comma1 + 1, comma2 - comma1 - 1);
                int weight = stoi(line.substr(comma2 + 1));
                addEdge(v1, v2, weight);
            }
        }

        file.close();
        cout << "✓ Loaded " << numVetex() << " stations from " << filename << endl;
    }

    // ========== SAVE JOURNEY HISTORY ==========
    void saveJourneyHistory()
    {
        ofstream file("journey_history.txt");
        for (const auto& journey : journeyHistory)
        {
            file << journey.source << " -> " << journey.destination 
                 << " | Distance: " << journey.distance << " km"
                 << " | Time: " << journey.time << " sec\n";
        }
        file.close();
    }

    void logJourney(string src, string dst, int dist, int time)
    {
        JourneyLog log;
        log.source = src;
        log.destination = dst;
        log.distance = dist;
        log.time = time;
        journeyHistory.push_back(log);
    }

    void displayJourneyHistory()
    {
        if (journeyHistory.empty())
        {
            cout << "No journey history yet." << endl;
            return;
        }

        cout << "\n╔════════════════════════════════════════════╗" << endl;
        cout << "║          JOURNEY HISTORY                 ║" << endl;
        cout << "╚════════════════════════════════════════════╝" << endl;

        for (int i = 0; i < journeyHistory.size(); i++)
        {
            cout << i + 1 << ". " << journeyHistory[i].source << " → " 
                 << journeyHistory[i].destination 
                 << " | " << journeyHistory[i].distance << " km" << endl;
        }
    }

    static void Create_Metro_Map(Graph_M &g)
    {
        g.addVertex("Noida_Sector_62~B");
        g.addVertex("Botanical_Garden~B");
        g.addVertex("Yamuna_Bank~B");
        g.addVertex("Rajiv_Chowk~BY");
        g.addVertex("Vaishali~B");
        g.addVertex("Moti_Nagar~B");
        g.addVertex("Janak_Puri_West~BO");
        g.addVertex("Dwarka_Sector_21~B");
        g.addVertex("Huda_City_Center~Y");
        g.addVertex("Saket~Y");
        g.addVertex("Vishwavidyalaya~Y");
        g.addVertex("Chandni_Chowk~Y");
        g.addVertex("New_Delhi~YO");
        g.addVertex("AIIMS~Y");
        g.addVertex("Shivaji_Stadium~O");
        g.addVertex("DDS_Campus~O");
        g.addVertex("IGI_Airport~O");
        g.addVertex("Rajouri_Garden~BP");
        g.addVertex("Netaji_Subhash_Place~PR");
        g.addVertex("Punjabi_Bagh_West~P");

        g.addEdge("Noida_Sector_62~B", "Botanical_Garden~B", 8);
        g.addEdge("Botanical_Garden~B", "Yamuna_Bank~B", 10);
        g.addEdge("Yamuna_Bank~B", "Vaishali~B", 8);
        g.addEdge("Yamuna_Bank~B", "Rajiv_Chowk~BY", 6);
        g.addEdge("Rajiv_Chowk~BY", "Moti_Nagar~B", 9);
        g.addEdge("Moti_Nagar~B", "Janak_Puri_West~BO", 7);
        g.addEdge("Janak_Puri_West~BO", "Dwarka_Sector_21~B", 6);
        g.addEdge("Huda_City_Center~Y", "Saket~Y", 15);
        g.addEdge("Saket~Y", "AIIMS~Y", 6);
        g.addEdge("AIIMS~Y", "Rajiv_Chowk~BY", 7);
        g.addEdge("Rajiv_Chowk~BY", "New_Delhi~YO", 1);
        g.addEdge("New_Delhi~YO", "Chandni_Chowk~Y", 2);
        g.addEdge("Chandni_Chowk~Y", "Vishwavidyalaya~Y", 5);
        g.addEdge("New_Delhi~YO", "Shivaji_Stadium~O", 2);
        g.addEdge("Shivaji_Stadium~O", "DDS_Campus~O", 7);
        g.addEdge("DDS_Campus~O", "IGI_Airport~O", 8);
        g.addEdge("Moti_Nagar~B", "Rajouri_Garden~BP", 2);
        g.addEdge("Punjabi_Bagh_West~P", "Rajouri_Garden~BP", 2);
        g.addEdge("Punjabi_Bagh_West~P", "Netaji_Subhash_Place~PR", 3);
    }
};

unordered_map<string, Graph_M::Vertex> Graph_M::vtces;
vector<Graph_M::JourneyLog> Graph_M::journeyHistory;

string *printCodelist()
{
    cout << "\nList of station along with their codes:" << endl;
    unordered_map<string, Graph_M::Vertex>::iterator it;
    int i = 1, j = 0, m = 1;
    string temp = "";
    string *codes = new string[Graph_M::vtces.size()];
    char c;
    for (it = Graph_M::vtces.begin(); it != Graph_M::vtces.end(); it++)
    {
        string key = it->first;
        codes[i - 1] = "";
        j = 0;
        while (j < key.length())
        {
            temp = key[j];
            c = temp[0];
            while (c > 47 && c < 58)
            {
                codes[i - 1] += c;
                j++;
                c = key[j];
            }
            if ((c < 48 || c > 57) && c < 123)
                codes[i - 1] += c;
            j++;
        }
        if (codes[i - 1].length() < 2)
            codes[i - 1] += toupper(key[1]);

        cout << i << ". " << key << "\t";
        if (key.length() < (22 - m))
            cout << "\t";
        if (key.length() < (14 - m))
            cout << "\t";
        if (key.length() < (6 - m))
            cout << "\t";
        cout << codes[i - 1] << endl;
        i++;
        if (i == pow(10, m))
            m++;
    }
    return codes;
}

// ========== STATION SEARCH HELPER ==========
string getStationWithSearch(Graph_M &g)
{
    cout << "\nEnter station name (or partial name for search): ";
    cin.ignore();
    string query;
    getline(cin, query);

    vector<string> results = g.searchStations(query);

    if (results.empty())
    {
        cout << "No stations found matching: " << query << endl;
        return "";
    }

    if (results.size() == 1)
    {
        cout << "✓ Selected: " << results[0] << endl;
        return results[0];
    }

    cout << "\nFound " << results.size() << " matching stations:" << endl;
    for (int i = 0; i < results.size() && i < 5; i++)
    {
        cout << i + 1 << ". " << results[i] << endl;
    }

    cout << "Enter choice (1-" << min(5, (int)results.size()) << "): ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= min(5, (int)results.size()))
    {
        return results[choice - 1];
    }

    return "";
}

int main()
{
    Graph_M g;
    Graph_M::Create_Metro_Map(g);

    cout << "\n\n";
    cout << "╔════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║     WELCOME TO DELHI METRO ROUTE OPTIMIZER (v2.0)            ║" << endl;
    cout << "║     Enhanced with Dijkstra's Algorithm & Smart Features      ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════════╝" << endl;

    while (true)
    {
        cout << "\n╔════════════════════════════════════════════════════╗" << endl;
        cout << "║          MAIN MENU                                 ║" << endl;
        cout << "╠════════════════════════════════════════════════════╣" << endl;
        cout << "║  1. List All Stations                             ║" << endl;
        cout << "║  2. Show Metro Map                                ║" << endl;
        cout << "║  3. Shortest Distance (Dijkstra)                  ║" << endl;
        cout << "║  4. Shortest Time (Dijkstra with Time Cost)       ║" << endl;
        cout << "║  5. Complete Route Path (Distance)                ║" << endl;
        cout << "║  6. Complete Route Path (Time)                    ║" << endl;
        cout << "║  7. Fare Calculation (Distance-based)             ║" << endl;
        cout << "║  8. Fare Calculation (Time-based)                 ║" << endl;
        cout << "║  9. Journey History                               ║" << endl;
        cout << "║ 10. Search Stations                               ║" << endl;
        cout << "║ 11. Exit                                          ║" << endl;
        cout << "╚════════════════════════════════════════════════════╝" << endl;

        int choice = -1;
        cout << "\nEnter your choice (1-11): ";
        cin >> choice;

        if (choice == 11)
            break;

        switch (choice)
        {
            case 1:
                g.display_Stations();
                break;

            case 2:
                g.display_Map();
                break;

            case 3:
            {
                printCodelist();
                string st1, st2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, st1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, st2);

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed))
                    cout << "\n✗ THE INPUTS ARE INVALID" << endl;
                else
                {
                    int dist = g.dijkstra(st1, st2, false);
                    cout << "\n✓ SHORTEST DISTANCE: " << dist << " KM" << endl;
                    g.logJourney(st1, st2, dist, 0);
                }
                break;
            }

            case 4:
            {
                printCodelist();
                string sat1, sat2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                if (!g.containsVertex(sat1) || !g.containsVertex(sat2) || !g.hasPath(sat1, sat2, processed1))
                    cout << "\n✗ THE INPUTS ARE INVALID" << endl;
                else
                {
                    int time = g.dijkstra(sat1, sat2, true);
                    cout << "\n✓ SHORTEST TIME: " << time / 60 << " MINUTES" << endl;
                    g.logJourney(sat1, sat2, 0, time);
                }
                break;
            }

            case 5:
            {
                printCodelist();
                string s1, s2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, s1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                    cout << "\n✗ THE INPUTS ARE INVALID" << endl;
                else
                {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    int len = str.size();
                    cout << "\n╔═══════════════════════════════════════╗" << endl;
                    cout << "║     ROUTE DETAILS (DISTANCE-WISE)    ║" << endl;
                    cout << "╠═══════════════════════════════════════╣" << endl;
                    cout << "║ SOURCE: " << s1 << endl;
                    cout << "║ DESTINATION: " << s2 << endl;
                    cout << "║ DISTANCE: " << str[len-2] << " KM" << endl;
                    cout << "║ INTERCHANGES: " << str[len-1] << endl;
                    cout << "╠═══════════════════════════════════════╣" << endl;
                    cout << "║ PATH:" << endl;
                    for (int i = 0; i < len-2; i++)
                    {
                        if (i == 0)
                            cout << "║ ↓" << endl;
                        cout << "║ " << str[i] << endl;
                        if (i < len-3)
                            cout << "║ ↓" << endl;
                    }
                    cout << "╚═══════════════════════════════════════╝" << endl;
                }
                break;
            }

            case 6:
            {
                printCodelist();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                    cout << "\n✗ THE INPUTS ARE INVALID" << endl;
                else
                {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                    int len = str.size();
                    cout << "\n╔═══════════════════════════════════════╗" << endl;
                    cout << "║     ROUTE DETAILS (TIME-WISE)        ║" << endl;
                    cout << "╠═══════════════════════════════════════╣" << endl;
                    cout << "║ SOURCE: " << ss1 << endl;
                    cout << "║ DESTINATION: " << ss2 << endl;
                    cout << "║ TIME: " << str[len-2] << " MINUTES" << endl;
                    cout << "║ INTERCHANGES: " << str[len-1] << endl;
                    cout << "╠═══════════════════════════════════════╣" << endl;
                    cout << "║ PATH:" << endl;
                    for (int i = 0; i < len-2; i++)
                    {
                        if (i == 0)
                            cout << "║ ↓" << endl;
                        cout << "║ " << str[i] << endl;
                        if (i < len-3)
                            cout << "║ ↓" << endl;
                    }
                    cout << "╚═══════════════════════════════════════╝" << endl;
                }
                break;
            }

            case 7:
            {
                printCodelist();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                    cout << "\n✗ THE INPUTS ARE INVALID" << endl;
                else
                {
                    cout << "\n╔═══════════════════════════════════════╗" << endl;
                    cout << "║     FARE CALCULATION (DISTANCE)      ║" << endl;
                    cout << "╠═══════════════════════════════════════╣" << endl;
                    cout << "║ STATIONS: " << g.Get_Minimum_Fare(ss1,ss2,7)[0] << endl;
                    cout << "║ FARE: ₹" << g.Get_Minimum_Fare(ss1,ss2,7)[1] << endl;
                    cout << "╚═══════════════════════════════════════╝" << endl;
                }
                break;
            }

            case 8:
            {
                printCodelist();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                    cout << "\n✗ THE INPUTS ARE INVALID" << endl;
                else
                {
                    cout << "\n╔═══════════════════════════════════════╗" << endl;
                    cout << "║      FARE CALCULATION (TIME)         ║" << endl;
                    cout << "╠═══════════════════════════════════════╣" << endl;
                    cout << "║ STATIONS: " << g.Get_Minimum_Fare(ss1,ss2,8)[0] << endl;
                    cout << "║ FARE: ₹" << g.Get_Minimum_Fare(ss1,ss2,8)[1] << endl;
                    cout << "╚═══════════════════════════════════════╝" << endl;
                }
                break;
            }

            case 9:
                g.displayJourneyHistory();
                break;

            case 10:
            {
                cout << "\n" << string(50, '=') << endl;
                cout << "STATION SEARCH (Autocomplete)" << endl;
                cout << string(50, '=') << endl;
                
                string found = getStationWithSearch(g);
                if (!found.empty())
                {
                    cout << "✓ Station: " << found << endl;
                }
                break;
            }

            default:
                cout << "\n✗ Invalid choice. Please try again." << endl;
        }
    }

    cout << "\n╔════════════════════════════════════════════════════╗" << endl;
    cout << "║      Thank you for using Metro Optimizer!         ║" << endl;
    cout << "║              Goodbye! 👋                           ║" << endl;
    cout << "╚════════════════════════════════════════════════════╝" << endl;

    return 0;
}
