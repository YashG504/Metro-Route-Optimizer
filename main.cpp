#include "graph.h"
#include "ui.h"
#include <iostream>
#include <limits>

using namespace std;

int main()
{
    Graph_M g;
    Graph_M::Create_Metro_Map(g);

    UI::printHeader();

    while (true)
    {
        UI::displayMenu();
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            UI::printError("Invalid input. Please enter a number.");
            continue;
        }

        UI::handleChoice(g, choice);
        
        if (choice != 9) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    return 0;
}
