#include "ui.h"
#include <iostream>
#include <iomanip>

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void UI::printHeader() {
    cout << "\n\n" << BOLD << CYAN;
    cout << "\t\t**********************************************" << endl;
    cout << "\t\t*           PUNE METRO ROUTE OPTIMIZER       *" << endl;
    cout << "\t\t**********************************************" << RESET << endl;
}

void UI::printFooter() {
    cout << "\n\t" << BOLD << "Thank you for using Pune Metro App!" << RESET << endl;
}

void UI::printSuccess(string msg) {
    cout << GREEN << "[SUCCESS] " << msg << RESET << endl;
}

void UI::printError(string msg) {
    cout << RED << "[ERROR] " << msg << RESET << endl;
}

void UI::printInfo(string msg) {
    cout << CYAN << "[INFO] " << msg << RESET << endl;
}

void UI::displayMenu() {
    cout << "\n" << YELLOW << "\tPlease choose an action:" << RESET << endl;
    cout << "\t" << "1. List all stations" << endl;
    cout << "\t" << "2. Show Metro Map" << endl;
    cout << "\t" << "3. Shortest Distance (Source -> Dest)" << endl;
    cout << "\t" << "4. Shortest Time (Source -> Dest)" << endl;
    cout << "\t" << "5. Path (Distance-wise)" << endl;
    cout << "\t" << "6. Path (Time-wise)" << endl;
    cout << "\t" << "7. Fare (Distance-wise)" << endl;
    cout << "\t" << "8. Fare (Time-wise)" << endl;
    cout << "\t" << "9. Exit" << endl;
    cout << "\n" << BOLD << "Enter your choice (1-9): " << RESET;
}

string UI::get_station_from_input(Graph_M &g, string input) {
    if (!input.empty() && isdigit(input[0])) {
        int index = stoi(input);
        int current = 1;
        for (auto it = g.vtces.begin(); it != g.vtces.end(); it++) {
            if (current == index) return it->first;
            current++;
        }
        return ""; 
    }
    
    // Case insensitive search
    string lower_input = input;
    transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);

    for (auto it = g.vtces.begin(); it != g.vtces.end(); it++) {
        string key = it->first;
        string lower_key = key;
        transform(lower_key.begin(), lower_key.end(), lower_key.begin(), ::tolower);
        
        if (lower_key.find(lower_input) != string::npos) {
            return key; // Return the exact key from the map
        }
    }

    return input; // Fallback
}

void UI::handleChoice(Graph_M &g, int choice) {
    switch (choice) {
        case 1:
            g.display_Stations();
            break;

        case 2:
            g.display_Map();
            break;

        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8: {
            string in1, in2, s1, s2;
            
            cout << "\n" << MAGENTA << "ENTER SOURCE STATION (Name or ID): " << RESET;
            if (cin.peek() == '\n') cin.ignore(); // handle leftover newline
            getline(cin, in1);
            s1 = get_station_from_input(g, in1);
            
            cout << MAGENTA << "ENTER DESTINATION STATION (Name or ID): " << RESET;
            getline(cin, in2);
            s2 = get_station_from_input(g, in2);

            unordered_map<string, bool> processed;
            if (s1 == "" || s2 == "" || !g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed)) {
                printError("Invalid Source or Destination Station. Please try again.");
                return;
            }

            cout << "\n" << BOLD << "-------------------------------------------------------------" << RESET << endl;
            cout << "SOURCE: " << GREEN << s1 << RESET << "  |  DESTINATION: " << GREEN << s2 << RESET << endl;

            if (choice == 3) {
                cout << "SHORTEST DISTANCE: " << BOLD << g.dijkstra(s1, s2, false) << " KM" << RESET << endl;
            } else if (choice == 4) {
                cout << "SHORTEST TIME: " << BOLD << g.dijkstra(s1, s2, true) / 60 << " MINUTES" << RESET << endl;
            } else if (choice == 5) {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                int len = str.size();
                cout << "DISTANCE : " << BOLD << str[len - 2] << " KM" << RESET << endl;
                cout << "INTERCHANGES : " << BOLD << str[len - 1] << RESET << endl;
                cout << "\nPATH:\n";
                for (int i = 0; i < len - 2; i++) cout << (i==0 ? "" : " -> ") << str[i];
                cout << endl;
            } else if (choice == 6) {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(s1, s2));
                int len = str.size();
                cout << "TIME : " << BOLD << str[len - 2] << " MINUTES" << RESET << endl;
                cout << "INTERCHANGES : " << BOLD << str[len - 1] << RESET << endl;
                cout << "\nPATH:\n";
                for (int i = 0; i < len - 2; i++) cout << (i==0 ? "" : " -> ") << str[i];
                cout << endl;
            } else if (choice == 7) {
                vector<int> result = g.Get_Minimum_Fare(s1, s2, 7);
                cout << "STATIONS COUNT: " << result[0] << endl;
                cout << "FARE: " << BOLD << "Rs " << result[1] << RESET << endl;
            } else if (choice == 8) {
                vector<int> result = g.Get_Minimum_Fare(s1, s2, 8);
                cout << "STATIONS COUNT: " << result[0] << endl;
                cout << "FARE: " << BOLD << "Rs " << result[1] << RESET << endl;
            }
            cout << BOLD << "-------------------------------------------------------------" << RESET << endl;
            break;
        }

        case 9:
            printFooter();
            exit(0);

        default:
            printError("Invalid Choice! Please enter a number between 1 and 9.");
    }
}
