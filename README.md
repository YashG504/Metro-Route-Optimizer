# Pune Metro Route Optimizer

A C++ console-based application to find the shortest path, minimum time, and fare between Pune Metro stations.

## Features

- **Shortest Distance:** Calculates the shortest path between two stations based on distance.
- **Shortest Time:** Calculates the fastest route between stations.
- **Fare Calculation:** Estimates the ticket fare based on distance and time.
- **Interchange Detection:** Identifies interchange stations (e.g., Civil Court).
- **Interactive UI:** Clean, menu-driven interface with color-coded output.
- **Station Search:** Supports station selection by ID or partial name search.

## Algorithms Used

- **Dijkstra's Algorithm:** Used for finding the shortest path (both distance and time weighted).
- **Graph (Adjacency List):** Represents the metro network with stations as vertices and tracks as edges.
- **Breadth-First Search (BFS) / Dijkstra:** Used in path finding logic.

## Project Structure

- `main.cpp`: Entry point of the application.
- `graph.h` / `graph.cpp`: Graph data structure and core algorithms (Dijkstra, BFS).
- `ui.h` / `ui.cpp`: User Interface logic, menu handling, and input validation.
- `Makefile`: Build script.

## How to Run

1. **Compile the code:**
   ```bash
   make
   ```

2. **Run the application:**
   ```bash
   ./metro_app
   ```
   Or simply:
   ```bash
   make run
   ```

## Sample Output

```
		**********************************************
		*           PUNE METRO ROUTE OPTIMIZER       *
		**********************************************

	Please choose an action:
	1. List all stations
	2. Show Metro Map
	3. Shortest Distance (Source -> Dest)
    ...
```
