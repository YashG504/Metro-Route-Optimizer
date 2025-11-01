#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>
#include <cstring>
#include<bits/stdc++.h>
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
        string res[100];
        int count = 0;
        char *temp = strtok((char *)str.c_str(), "  ");
        while (temp != NULL)
        {
            res[count++] = temp;
            temp = strtok(NULL, "  ");
        }
        arr.push_back(res[0]);

        int c=0;
        for (int i = 1; i < count - 1; i++)
        {
            int index = res[i].find('~');
            string s = res[i].substr(index + 1);

            if (s.length() == 2) // Handles interchange stations like Civil_Court~AP
            {
                string prev = res[i - 1].substr(res[i - 1].find('~') + 1);
                string next = res[i + 1].substr(res[i + 1].find('~') + 1);

                // If previous line character is present in the interchange string AND
                // next line character is also present, check for a real line change.
                if (s.find(prev) != string::npos && s.find(next) != string::npos)
                {
                    if (prev == next) {
                        arr.push_back(res[i]); // Continue on the same line through an interchange station
                    } else {
                        arr.push_back(res[i] + " ==> CHANGE LINE"); // Actual line change
                        c++;
                    }
                } else {
                    arr.push_back(res[i]);
                }
            }
            else
            {
                arr.push_back(res[i]);
            }
        }
        arr.push_back(res[count - 1]);
        arr.push_back(to_string(c) );
        return arr;
    }

    vector<int> Get_Minimum_Fare(string src,string dst,int x)
    {
        int stations=0;
        int fare=0;

        if(x==7)
        {
            stations=get_Interchanges(Get_Minimum_Distance(src, dst)).size()-2; // Adjust for path and interchange count
        }
        else
        {
            stations=get_Interchanges(Get_Minimum_Time(src, dst)).size()-2; // Adjust for path and interchange count
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
        fare=50;

        return {stations,fare};
    }

    static void Create_Metro_Map(Graph_M &g)
    {
        // Add Pune Metro stations
        // Purple Line Stations (~P)
        g.addVertex("Vanaz~P");
        g.addVertex("Anand_Nagar~P");
        g.addVertex("Ideal_Colony~P");
        g.addVertex("Nal_Stop~P");
        g.addVertex("Garware_College~P");
        g.addVertex("Deccan_Gymkhana~P");
        g.addVertex("Chhatrapati_Sambhaji_Udyan~P");
        g.addVertex("PMC~P");
        g.addVertex("Mangalwar_Peth~P");
        g.addVertex("Pune_Railway_Station~P");
        g.addVertex("Ruby_Hall_Clinic~P");
        g.addVertex("Bund_Garden~P");
        g.addVertex("Yerawada~P");
        g.addVertex("Kalyani_Nagar~P");
        g.addVertex("Ramwadi~P");

        // Aqua Line Stations (~A)
        g.addVertex("PCMC~A");
        g.addVertex("Sant_Tukaram_Nagar~A");
        g.addVertex("Bhosari~A");
        g.addVertex("Kasarwadi~A");
        g.addVertex("Phugewadi~A");
        g.addVertex("Dapodi~A");
        g.addVertex("Bopodi~A");
        g.addVertex("Khadki~A");
        g.addVertex("Range_Hill~A");
        g.addVertex("Shivaji_Nagar~A");

        // Interchange Station (~AP)
        g.addVertex("Civil_Court~AP");

        // Add Edges with approximate distances (in km)
        // Purple Line Connections
        g.addEdge("Vanaz~P", "Anand_Nagar~P", 1);
        g.addEdge("Anand_Nagar~P", "Ideal_Colony~P", 1);
        g.addEdge("Ideal_Colony~P", "Nal_Stop~P", 1);
        g.addEdge("Nal_Stop~P", "Garware_College~P", 1);
        g.addEdge("Garware_College~P", "Deccan_Gymkhana~P", 1);
        g.addEdge("Deccan_Gymkhana~P", "Chhatrapati_Sambhaji_Udyan~P", 1);
        g.addEdge("Chhatrapati_Sambhaji_Udyan~P", "PMC~P", 1);
        g.addEdge("PMC~P", "Civil_Court~AP", 1);
        g.addEdge("Civil_Court~AP", "Mangalwar_Peth~P", 1);
        g.addEdge("Mangalwar_Peth~P", "Pune_Railway_Station~P", 2);
        g.addEdge("Pune_Railway_Station~P", "Ruby_Hall_Clinic~P", 1);
        g.addEdge("Ruby_Hall_Clinic~P", "Bund_Garden~P", 1);
        g.addEdge("Bund_Garden~P", "Yerawada~P", 2);
        g.addEdge("Yerawada~P", "Kalyani_Nagar~P", 1);
        g.addEdge("Kalyani_Nagar~P", "Ramwadi~P", 1);

        // Aqua Line Connections
        g.addEdge("PCMC~A", "Sant_Tukaram_Nagar~A", 1);
        g.addEdge("Sant_Tukaram_Nagar~A", "Bhosari~A", 2);
        g.addEdge("Bhosari~A", "Kasarwadi~A", 1);
        g.addEdge("Kasarwadi~A", "Phugewadi~A", 2);
        g.addEdge("Phugewadi~A", "Dapodi~A", 1);
        g.addEdge("Dapodi~A", "Bopodi~A", 1);
        g.addEdge("Bopodi~A", "Khadki~A", 1);
        g.addEdge("Khadki~A", "Range_Hill~A", 2);
        g.addEdge("Range_Hill~A", "Shivaji_Nagar~A", 1);
        g.addEdge("Shivaji_Nagar~A", "Civil_Court~AP", 1);
    }
};

unordered_map<string, Graph_M::Vertex> Graph_M::vtces;

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

int main()
{
    Graph_M g;
    Graph_M::Create_Metro_Map(g);

    cout << "\n\n\n\n\t\t\t\t\t\t\t\tWELCOME TO THE PUNE METRO APP" << endl;

    while (true)
    {
        cout<<"\n\n\t*********************************************************************************************************************************\n";
        cout<<"\t*\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t*\n";
        cout<<"\t*\t\t\t\t\t\t~LIST OF ACTIONS\t\t\t\t\t\t\t*\n";
        cout<<"\t*\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t*\n";
        cout<<"\t* 1. LIST ALL THE STATIONS IN THE MAP                                                                                        *\n";
        cout<<"\t* 2. SHOW THE METRO MAP                                                                                                      *\n";
        cout<<"\t* 3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                                  *\n";
        cout<<"\t* 4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                             *\n";
        cout<<"\t* 5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                             *\n";
        cout<<"\t* 6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                 *\n";
        cout<<"\t* 7. GET FARE FOR SHORTEST PATH (DISTANCE WISE)                                                                              *\n";
        cout<<"\t* 8. GET FARE FOR SHORTEST PATH (TIME WISE)                                                                                  *\n";
        cout<<"\t* 9. EXIT THE MENU                                                                                                           *\n";
        cout<<"\t*\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t*\n";
        cout<<"\t*********************************************************************************************************************************\n";

        cout << "\n\n";
        int choice = -1;
        cout<<"ENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 9) :";
        cin >> choice;

        if (choice == 9)
        {break;}

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
                // string *keys = printCodelist(); // This line prints the list but the return value is unused.
                g.display_Stations();
                string st1, st2;
                cout << "\nENTER THE SOURCE STATION (e.g., Vanaz~P): ";
                cin.ignore();
                getline(cin, st1);
                cout << "ENTER THE DESTINATION STATION (e.g., PCMC~A): ";
                getline(cin, st2);

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed))
                    cout << "\nTHE INPUTS ARE INVALID" << endl;
                else
                    cout << "\nSHORTEST DISTANCE: "<< g.dijkstra(st1, st2, false) << "KM" << endl;
                break;
            }

            case 4:
            {
                // string *keys = printCodelist();
                g.display_Stations();
                string sat1, sat2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                 if (!g.containsVertex(sat1) || !g.containsVertex(sat2) || !g.hasPath(sat1, sat2, processed1))
                    cout << "\nTHE INPUTS ARE INVALID" << endl;
                 else
                    cout << "\nSHORTEST TIME: " << g.dijkstra(sat1, sat2, true) / 60 << " MINUTES" << endl;
                break;
            }

            case 5:
            {
                // string *keys = printCodelist();
                g.display_Stations();
                string s1, s2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, s1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                    cout << "THE INPUTS ARE INVALID" << endl;
                else
                {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    int len = str.size();
                    cout<<endl<<endl;
                    cout << "SOURCE STATION : " << s1 << endl;
                    cout << "DESTINATION STATION : " << s2 << endl;
                    cout << "DISTANCE : " << str[len-2]<< " km" << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len-1] << endl;
                    cout << endl<< endl<<"THE PATH IS AS FOLLOWS:\n\n";
                    for (int i = 0; i <len-2; i++)
                    {cout << str[i] << endl;}
                }
                break;
            }

            case 6:
            {
                // string *keys = printCodelist();
                g.display_Stations();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                    cout << "THE INPUTS ARE INVALID" << endl;
                else
                {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                    int len = str.size();
                    cout<<endl<<endl;
                    cout << "SOURCE STATION : " << ss1 << endl;
                    cout << "DESTINATION STATION : " << ss2 << endl;
                    cout << "TIME : " << str[len-2] << " MINUTES" << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len-1] << endl;
                    cout << endl<< endl<<"THE PATH IS AS FOLLOWS:\n";
                    for (int i = 0; i < len-2; i++)
                    {cout << str[i] << endl;}
                    cout << endl;
                }
                break;
            }

            case 7:
            {
                // string *keys = printCodelist();
                g.display_Stations();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                cout << "THE INPUTS ARE INVALID" << endl;

                else
                {
                    cout<<"\nNUMBER OF STATIONS IN SHORTEST DISTANCE :"<<g.Get_Minimum_Fare(ss1,ss2,7)[0];
                    cout<<"\nCALCULATED FARE : Rs "<<g.Get_Minimum_Fare(ss1,ss2,7)[1];
                }

                break;
            }

            case 8:
            {
                // string *keys = printCodelist();
                g.display_Stations();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                cout << "THE INPUTS ARE INVALID" << endl;

                else
                {
                    cout<<"\nNUMBER OF STATIONS IN SHORTEST TIME :"<<g.Get_Minimum_Fare(ss1,ss2,8)[0];
                    cout<<"\nCALCULATED FARE : Rs "<<g.Get_Minimum_Fare(ss1,ss2,8)[1];
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
