#ifndef UI_H
#define UI_H

#include "graph.h"
#include <iostream>
#include <string>

using namespace std;

class UI {
public:
    static void displayMenu();
    static void handleChoice(Graph_M &g, int choice);
    static string get_station_from_input(Graph_M &g, string input);
    static void printHeader();
    static void printFooter();
    static void printSuccess(string msg);
    static void printError(string msg);
    static void printInfo(string msg);
};

#endif
