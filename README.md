# 🚇 Metro Route Optimizer (Pune Metro)

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![JavaScript](https://img.shields.io/badge/JavaScript-ES6+-yellow.svg)
![Data Structures](https://img.shields.io/badge/Data_Structures-Graphs-brightgreen.svg)
![Algorithm](https://img.shields.io/badge/Algorithm-Dijkstra-orange.svg)

**A high-performance, full-stack Metro Navigation System built to demonstrate advanced Data Structures & Algorithms (DSA) and modern web UI/UX engineering.**

## 📖 Project Overview

The **Metro Route Optimizer** is a complete, end-to-end software engineering project. It began as a robust **C++ Command-Line Application** utilizing Graph theory to calculate optimal transit paths. It has since evolved into a **Premium Interactive Web Visualizer**, allowing users to dynamically interact with the Pune Metro transit map in real-time.

By modeling the metro network as a weighted graph, this project calculates the **Shortest Distance**, **Shortest Time**, and **Minimum Fare** between any two stations using a highly optimized implementation of **Dijkstra's Algorithm**.

---

## ✨ Key Features

### 🧠 Core Algorithm (The Brain)
* **Custom Min-Heap Priority Queue**: Implemented from scratch (bypassing standard libraries) to guarantee optimal `O((V+E)logV)` execution time for pathfinding.
* **Dual-Weight Pathfinding**: Dynamically calculates the best route based on either physical distance (KM) or travel time (Minutes).
* **Fare Slab Logic**: Calculates trip costs based on non-linear station-count slabs (e.g., 1-3 stations = ₹10, 4-5 stations = ₹20).
* **Interchange Optimization**: Detects line transfers (e.g., Purple Line ↔ Aqua Line at Civil Court) and automatically accounts for interchange delays.

### 💻 Interactive Web UI (The Face)
* **Glassmorphic Design System**: A premium, dark-themed UI featuring translucent panels and modern typography.
* **Live SVG Map Rendering**: A custom-built SVG map of the Pune Metro that highlights active routes and pulsates destination stations.
* **Animated Train Tracking**: Simulates the transit journey visually with moving indicator dots along the SVG paths.
* **Smart Autocomplete Search**: Instantly filters and suggests station names as the user types, eliminating input errors.

---

## 🛠️ Technology Stack

| Domain | Technology / Concept |
| :--- | :--- |
| **Core Algorithms** | C++17, Vanilla JavaScript (ES6+), Graph Theory |
| **Data Structures** | Adjacency Lists, Hash Maps (`unordered_map` / `Map`), Min-Heaps |
| **Frontend UI** | HTML5, CSS3 (Flexbox/Grid), Glassmorphism |
| **Visualization** | Native SVG rendering and CSS Keyframe Animations |

---

## 📈 Algorithmic Performance

The backbone of this application is **Dijkstra’s Shortest Path Algorithm**.

* **Graph Representation:** Adjacency List (`Map<String, Array<Edge>>`)
* **Time Complexity:** `O((V + E) log V)`
    * `V` = Number of Vertices (Stations)
    * `E` = Number of Edges (Connecting routes)
* **Space Complexity:** `O(V)` for the Priority Queue and Distance maps.

By avoiding an `O(V^2)` array-based approach and instead utilizing an optimized Min-Heap for vertex extraction, the path calculation happens near-instantly, even if scaled to hundreds of stations.

---

## 🚀 Getting Started

This project includes two distinct versions: The hardcore C++ CLI and the interactive Web UI.

### 1. Run the Interactive Web Visualizer (Recommended)
This requires Node.js installed on your machine.
```bash
# 1. Clone the repository
# 2. Open a terminal in the project directory
# 3. Start a local development server
npx serve . -l 3000
```
Then, open your browser and navigate to `http://localhost:3000`.

### 2. Run the C++ CLI (Windows Only)
This requires `g++` (MinGW) to be installed and added to your system PATH.
```bash
# Simply double-click the batch file to compile and run:
.\run.bat
```
*Note: The C++ CLI is configured to force UTF-8 console output for beautiful ASCII formatting.*

---

## 📸 Screenshots

*(Add screenshots of your premium Web UI here!)*
* `<img src="path/to/screenshot1.png" width="800">` - The glassmorphic dashboard.
* `<img src="path/to/screenshot2.png" width="800">` - The animated SVG map calculating a cross-line route.

---

## 👨‍💻 Developed By
Designed and Engineered as a portfolio project for SDE and Full-Stack Engineering roles.
