# 🚨 D-RRAS: Disaster Relief & Resource Allocation System

A C++-based simulation platform for managing real-time disaster relief operations using advanced data structures and algorithms. This project models backend logic for dynamic rescue routing, roadblock-aware navigation, resource optimization, and multilingual alerting — built entirely around core DSA principles.

---

## 📌 Key Features

### ⚙️ Algorithmic Core
- ✅ **Priority Queue (Heap)** for managing rescue requests by severity and timestamp.
- 📍 **Dijkstra's Algorithm** for calculating shortest, safest evacuation routes.
- 🎯 **Dynamic Programming (0/1 Knapsack)** for resource allocation under capacity constraints.
- ⚡ **Greedy Allocation** for volunteer-task matching based on skill and proximity.
- ⛔ **Road Block Handling** with real-time graph edge updates and re-routing.

### 🛠️ Functional Modules
- 🚨 **Rescue Dispatch System** – Prioritizes and dispatches aid dynamically.
- 🧑‍🚒 **Volunteer Management** – Matches individuals based on location and skills.
- 🌍 **Multilingual Alerts** – Supports English, French, and Spanish notifications.
- 🧭 **Predictive Evacuation** – Suggests optimal escape paths during disasters.
- 💬 **Feedback Collector** – Gathers, stores, and analyzes ratings/comments.
- 💾 **File Persistence Layer** – Simulated lightweight DB using file I/O.

---

## 🧠 Tech Stack & Concepts

- **Language**: C++ (Standard Template Library, OOP, File Handling)
- **Data Structures**: Graphs, Heaps, Maps, Vectors, Queues, Sets
- **Algorithms**: Dijkstra, Knapsack DP, Greedy Matching
- **Design Principles**: Modularization, Encapsulation, Simulation-oriented logic
- **Environment**: Command-Line Interface (CLI)

---

🖼️ Screenshots
The project comes with a complete set of screenshots to visually demonstrate key functionalities, available inside the /Screenshots folder.

---

## 🔍 Why This Project Stands Out

> Built to demonstrate practical application of core DSA knowledge, D-RRAS focuses on real-world backend logic over UI — simulating the kind of intelligent systems used in emergency response centers, logistics management, and large-scale routing platforms.
Rather than relying on frameworks, this project reflects **algorithmic problem solving**, **systemic thinking**, and **resource prioritization** — the exact qualities valued in backend engineering, SDE roles, and AMTS positions at top tech companies.

---

## 🧾 How to Run

```bash
g++ -o drras drras_enhanced.cpp
./drras

Author
Aditya Chaturvedi
Jaypee Institute of Information Technology
