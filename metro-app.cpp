#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>
#include <sstream>
#include <climits>

using namespace std;

class Graph_M
{
public:
    class Vertex
    {
    public:
        unordered_map<string, int> neighbours;
    };

    static unordered_map<string, Vertex> vtces;

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

    void display_Map()
    {
        cout << "\t Pune Metro Map" << endl;
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
                str += to_string(it2->second) + "\n";
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
        cout << "********************************************************************************" << endl;
        cout << "STATION LIST (Purple Line: ~P, Aqua Line: ~A, Interchange: ~AP)" << endl;
        cout << "********************************************************************************" << endl;
        int i = 1;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            cout << i << ". " << key << endl;
            i++;
        }
        cout << "********************************************************************************" << endl;
        cout << endl;
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

    int dijkstra(string src, string des, bool nan)
    {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> pq;

        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            DijkstraPair np;
            np.vname = key;
            np.cost = INT_MAX;

            if (key == src)
            {
                np.cost = 0;
                np.psf = key;
            }

            pq.push(np);
            map[key] = np;
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

            map.erase(rp.vname);

            ans.push_back(rp.vname);

            Vertex v = vtces[rp.vname];
            for (auto it = v.neighbours.begin(); it != v.neighbours.end(); it++)
            {
                string nbr = it->first;
                if (map.count(nbr))
                {
                    int oc = map[nbr].cost;
                    Vertex k = vtces[rp.vname];
                    int nc;
                    if (nan)
                        nc = rp.cost + 120 + 40 * k.neighbours[nbr];
                    else
                        nc = rp.cost + k.neighbours[nbr];

                    if (nc < oc)
                    {
                        DijkstraPair gp = map[nbr];
                        gp.psf = rp.psf + nbr;
                        gp.cost = nc;

                        pq.push(gp);
                        map[nbr] = gp;
                    }
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
        vector<string> res;
        
        stringstream ss(str);
        string segment;
        
        while (getline(ss, segment, ' ')) {
            if (!segment.empty()) {
                res.push_back(segment);
            }
        }
        
        if (res.empty()) return arr;

        arr.push_back(res[0]);
        int c = 0;

        for (int i = 1; i < res.size() - 1; i++)
        {
            int index = res[i].find('~');
            string s = res[i].substr(index + 1);

            if (s.length() == 2)
            {
                string prev = res[i - 1].substr(res[i - 1].find('~') + 1);
                string next = res[i + 1].substr(res[i + 1].find('~') + 1);

                if (prev == next)
                {
                    arr.push_back(res[i]);
                }
                else
                {
                    arr.push_back(res[i] + " ==> CHANGE LINE");
                    c++;
                }
            }
            else
            {
                arr.push_back(res[i]);
            }
        }
        arr.push_back(res[res.size() - 1]);
        arr.push_back(to_string(c));
        return arr;
    }

    vector<int> Get_Minimum_Fare(string src, string dst, int x)
    {
        int stations = 0;
        int fare = 0;
        
        vector<string> path_info;

        if (x == 7)
        {
            path_info = get_Interchanges(Get_Minimum_Distance(src, dst));
        }
        else
        {
             path_info = get_Interchanges(Get_Minimum_Time(src, dst));
        }
        
        stations = path_info.size() - 1; 

        if (stations > 0 && stations <= 3)
            fare = 10;
        else if (stations > 3 && stations <= 7)
            fare = 20;
        else if (stations > 7 && stations <= 11)
            fare = 30;
        else if (stations > 11 && stations <= 20)
            fare = 40;
        else
            fare = 50;

        return {stations, fare};
    }

    static void Create_Metro_Map(Graph_M &g)
    {
        // --- AQUA LINE (Vanaz to Ramwadi) [Suffix: ~A] ---
        g.addVertex("Vanaz~A");
        g.addVertex("Anand_Nagar~A");
        g.addVertex("Ideal_Colony~A");
        g.addVertex("Nal_Stop~A");
        g.addVertex("Garware_College~A");
        g.addVertex("Deccan_Gymkhana~A");
        g.addVertex("Sambhaji_Udyan~A");
        g.addVertex("PMC~A");
        g.addVertex("Civil_Court~AP"); 
        g.addVertex("Mangalwar_Peth~A");
        g.addVertex("Pune_Railway_Station~A");
        g.addVertex("Ruby_Hall_Clinic~A");
        g.addVertex("Bund_Garden~A");
        g.addVertex("Yerawada~A");
        g.addVertex("Kalyani_Nagar~A");
        g.addVertex("Ramwadi~A");

        // --- PURPLE LINE (PCMC to Swargate) [Suffix: ~P] ---
        g.addVertex("PCMC~P");
        g.addVertex("Sant_Tukaram_Nagar~P");
        g.addVertex("Bhosari~P");
        g.addVertex("Kasarwadi~P");
        g.addVertex("Phugewadi~P");
        g.addVertex("Dapodi~P");
        g.addVertex("Bopodi~P");
        g.addVertex("Khadki~P");
        g.addVertex("Range_Hill~P");
        g.addVertex("Shivajinagar~P");
        g.addVertex("Budhwar_Peth~P");
        g.addVertex("Mandai~P");
        g.addVertex("Swargate~P");

        // --- EDGES ---
        g.addEdge("Vanaz~A", "Anand_Nagar~A", 1);
        g.addEdge("Anand_Nagar~A", "Ideal_Colony~A", 1);
        g.addEdge("Ideal_Colony~A", "Nal_Stop~A", 1);
        g.addEdge("Nal_Stop~A", "Garware_College~A", 1);
        g.addEdge("Garware_College~A", "Deccan_Gymkhana~A", 1);
        g.addEdge("Deccan_Gymkhana~A", "Sambhaji_Udyan~A", 1);
        g.addEdge("Sambhaji_Udyan~A", "PMC~A", 1);
        g.addEdge("PMC~A", "Civil_Court~AP", 1); 
        g.addEdge("Civil_Court~AP", "Mangalwar_Peth~A", 1); 
        g.addEdge("Mangalwar_Peth~A", "Pune_Railway_Station~A", 1);
        g.addEdge("Pune_Railway_Station~A", "Ruby_Hall_Clinic~A", 1);
        g.addEdge("Ruby_Hall_Clinic~A", "Bund_Garden~A", 1);
        g.addEdge("Bund_Garden~A", "Yerawada~A", 2);
        g.addEdge("Yerawada~A", "Kalyani_Nagar~A", 1);
        g.addEdge("Kalyani_Nagar~A", "Ramwadi~A", 1);

        g.addEdge("PCMC~P", "Sant_Tukaram_Nagar~P", 1);
        g.addEdge("Sant_Tukaram_Nagar~P", "Bhosari~P", 1);
        g.addEdge("Bhosari~P", "Kasarwadi~P", 1);
        g.addEdge("Kasarwadi~P", "Phugewadi~P", 1);
        g.addEdge("Phugewadi~P", "Dapodi~P", 1);
        g.addEdge("Dapodi~P", "Bopodi~P", 1);
        g.addEdge("Bopodi~P", "Khadki~P", 1);
        g.addEdge("Khadki~P", "Range_Hill~P", 2);
        g.addEdge("Range_Hill~P", "Shivajinagar~P", 2);
        g.addEdge("Shivajinagar~P", "Civil_Court~AP", 1); 
        g.addEdge("Civil_Court~AP", "Budhwar_Peth~P", 1); 
        g.addEdge("Budhwar_Peth~P", "Mandai~P", 1);
        g.addEdge("Mandai~P", "Swargate~P", 1);
    }
};

unordered_map<string, Graph_M::Vertex> Graph_M::vtces;

// --- HELPER FUNCTION FOR INPUT CONVERSION ---
string get_station_from_input(string input) {
    if (!input.empty() && isdigit(input[0])) {
        int index = stoi(input);
        int current = 1;
        for (auto it = Graph_M::vtces.begin(); it != Graph_M::vtces.end(); it++) {
            if (current == index) return it->first;
            current++;
        }
        return ""; 
    }
    return input; 
}

int main()
{
    Graph_M g;
    Graph_M::Create_Metro_Map(g);

    cout << "\n\n\n\n\t\t\t\t\t\t\t\tWELCOME TO THE PUNE METRO APP" << endl;

    while (true)
    {
        cout << "\n\n\t*********************************************************************************************************************************\n";
        cout << "\t*\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t*\n";
        cout << "\t*\t\t\t\t\t\t~LIST OF ACTIONS\t\t\t\t\t\t\t*\n";
        cout << "\t*\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t*\n";
        cout << "\t* 1. LIST ALL THE STATIONS IN THE MAP                                                                                           *\n";
        cout << "\t* 2. SHOW THE METRO MAP                                                                                                         *\n";
        cout << "\t* 3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                                     *\n";
        cout << "\t* 4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                                *\n";
        cout << "\t* 5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                *\n";
        cout << "\t* 6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                    *\n";
        cout << "\t* 7. GET FARE FOR SHORTEST PATH (DISTANCE WISE)                                                                                 *\n";
        cout << "\t* 8. GET FARE FOR SHORTEST PATH (TIME WISE)                                                                                     *\n";
        cout << "\t* 9. EXIT THE MENU                                                                                                              *\n";
        cout << "\t*\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t*\n";
        cout << "\t*********************************************************************************************************************************\n";

        cout << "\n\n";
        int choice = -1;
        cout << "ENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 9) :";
        cin >> choice;

        if (choice == 9)
        {
            break;
        }

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
            g.display_Stations();
            string in1, in2, s1, s2;
            cout << "\nENTER THE SOURCE STATION (Name or ID): ";
            cin.ignore();
            getline(cin, in1);
            s1 = get_station_from_input(in1);
            
            cout << "ENTER THE DESTINATION STATION (Name or ID): ";
            getline(cin, in2);
            s2 = get_station_from_input(in2);

            unordered_map<string, bool> processed;
            if (s1 == "" || s2 == "" || !g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed))
                cout << "\nTHE INPUTS ARE INVALID" << endl;
            else
            {
                cout << "\n-------------------------------------------------------------" << endl;
                cout << "SOURCE: " << s1 << "  |  DESTINATION: " << s2 << endl; 
                cout << "SHORTEST DISTANCE: " << g.dijkstra(s1, s2, false) << " KM" << endl;
                cout << "-------------------------------------------------------------" << endl;
            }
            break;
        }

        case 4:
        {
            g.display_Stations();
            string in1, in2, s1, s2;
            cout << "\nENTER THE SOURCE STATION (Name or ID): ";
            cin.ignore();
            getline(cin, in1);
            s1 = get_station_from_input(in1);
            
            cout << "ENTER THE DESTINATION STATION (Name or ID): ";
            getline(cin, in2);
            s2 = get_station_from_input(in2);

            unordered_map<string, bool> processed1;
            if (s1 == "" || s2 == "" || !g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed1))
                cout << "\nTHE INPUTS ARE INVALID" << endl;
            else
            {
                cout << "\n-------------------------------------------------------------" << endl;
                cout << "SOURCE: " << s1 << "  |  DESTINATION: " << s2 << endl; 
                cout << "SHORTEST TIME: " << g.dijkstra(s1, s2, true) / 60 << " MINUTES" << endl;
                cout << "-------------------------------------------------------------" << endl;
            }
            break;
        }

        case 5:
        {
            g.display_Stations();
            string in1, in2, s1, s2;
            cout << "\nENTER THE SOURCE STATION (Name or ID): ";
            cin.ignore();
            getline(cin, in1);
            s1 = get_station_from_input(in1);
            
            cout << "ENTER THE DESTINATION STATION (Name or ID): ";
            getline(cin, in2);
            s2 = get_station_from_input(in2);

            unordered_map<string, bool> processed2;
            if (s1 == "" || s2 == "" || !g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
            {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                int len = str.size();
                cout << endl << endl;
                cout << "SOURCE STATION : " << s1 << endl;
                cout << "DESTINATION STATION : " << s2 << endl;
                cout << "DISTANCE : " << str[len - 2] << " KM" << endl;
                cout << "NUMBER OF INTERCHANGES : " << str[len - 1] << endl;
                cout << endl << endl << "THE PATH IS AS FOLLOWS:\n\n";
                for (int i = 0; i < len - 2; i++)
                {
                    cout << str[i] << endl;
                }
            }
            break;
        }

        case 6:
        {
            g.display_Stations();
            string in1, in2, s1, s2;
            cout << "\nENTER THE SOURCE STATION (Name or ID): ";
            cin.ignore();
            getline(cin, in1);
            s1 = get_station_from_input(in1);
            
            cout << "ENTER THE DESTINATION STATION (Name or ID): ";
            getline(cin, in2);
            s2 = get_station_from_input(in2);

            unordered_map<string, bool> processed3;
            if (s1 == "" || s2 == "" || !g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed3))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
            {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(s1, s2));
                int len = str.size();
                cout << endl << endl;
                cout << "SOURCE STATION : " << s1 << endl;
                cout << "DESTINATION STATION : " << s2 << endl;
                cout << "TIME : " << str[len - 2] << " MINUTES" << endl;
                cout << "NUMBER OF INTERCHANGES : " << str[len - 1] << endl;
                cout << endl << endl << "THE PATH IS AS FOLLOWS:\n";
                for (int i = 0; i < len - 2; i++)
                {
                    cout << str[i] << endl;
                }
                cout << endl;
            }
            break;
        }

        case 7:
        {
            g.display_Stations();
            string in1, in2, s1, s2;
            cout << "\nENTER THE SOURCE STATION (Name or ID): ";
            cin.ignore();
            getline(cin, in1);
            s1 = get_station_from_input(in1);
            
            cout << "ENTER THE DESTINATION STATION (Name or ID): ";
            getline(cin, in2);
            s2 = get_station_from_input(in2);

            unordered_map<string, bool> processed3;
            if (s1 == "" || s2 == "" || !g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed3))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
            {
                vector<int> result = g.Get_Minimum_Fare(s1, s2, 7);
                cout << "\nNUMBER OF STATIONS IN SHORTEST DISTANCE :" << result[0];
                cout << "\nCALCULATED FARE : Rs " << result[1];
            }
            break;
        }

        case 8:
        {
            g.display_Stations();
            string in1, in2, s1, s2;
            cout << "\nENTER THE SOURCE STATION (Name or ID): ";
            cin.ignore();
            getline(cin, in1);
            s1 = get_station_from_input(in1);
            
            cout << "ENTER THE DESTINATION STATION (Name or ID): ";
            getline(cin, in2);
            s2 = get_station_from_input(in2);

            unordered_map<string, bool> processed3;
            if (s1 == "" || s2 == "" || !g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed3))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
            {
                 vector<int> result = g.Get_Minimum_Fare(s1, s2, 8);
                cout << "\nNUMBER OF STATIONS IN SHORTEST TIME :" << result[0];
                cout << "\nCALCULATED FARE : Rs " << result[1];
            }
            break;
        }

        default:
        {
            cout << "Invalid choice" << endl;
            break;
        }
        }
    }

    return 0;
}
