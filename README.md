DRRAS — Disaster Response Routing & Allocation System

A console‑based backend simulation of a real‑time emergency operations control system. D‑RRAS models how authorities route rescue teams, prioritize victims, allocate scarce resources, coordinate volunteers, and broadcast alerts during disasters — implemented entirely with core Data Structures & Algorithms in modern C++ (no external frameworks).


Overview

In real disasters, decisions must be made under uncertainty and limited resources. Static systems fail because conditions change continuously: roads get blocked, new requests appear, supplies run out, and evacuation routes must adapt.

D‑RRAS simulates an intelligent command center that continuously:

* Prioritizes incoming rescue requests
* Computes safe navigation paths avoiding roadblocks and traffic
* Allocates food/medical supplies optimally
* Coordinates volunteers by skill and location
* Sends real‑time alerts and evacuation suggestions

This project emphasizes **system design + algorithmic reasoning** rather than UI development.


Key Features

* Dynamic road network with traffic and blockages
* AI‑like multi‑factor priority scoring for rescue requests
* Optimal resource allocation using dynamic programming
* Volunteer discovery by location & skill matching
* Predictive evacuation route suggestions
* Real‑time alert broadcasting (publish‑subscribe model)
* Multilingual console interface
* Feedback ranking and persistence


Architecture

    User / Admin Interface
              ↓
    RescueManagementSystem (Core Controller)
              ↓
     ┌─────────────┬─────────────┬─────────────┬─────────────┐
     Routing      Resource      Volunteer      Alert
     Engine       Optimizer     Manager        System
     (Graphs)     (DP/Greedy)   (Hash Index)   (Queue Pub/Sub)

     

Algorithms & Data Structures

| Category                           | Usage                               |
| ---------------------------------- | ----------------------------------- |
| Graph (Adjacency List)             | City navigation network             |
| Dijkstra                           | Shortest rescue route               |
| Priority Queue                     | Emergency request scheduling        |
| Dynamic Programming (0/1 Knapsack) | Resource allocation                 |
| Greedy Heuristics                  | Dispatch optimization               |
| Hash Maps                          | Fast lookup for volunteers & cities |
| Linked List                        | Supply inventory storage            |
| Queue                              | Alert event processing              |
| Sorting                            | Feedback ranking                    |
| Heuristic Scoring                  | Intelligent prioritization          |


Modules

-Routing Engine

Calculates optimal paths considering traffic and blocked roads. Automatically recomputes routes when conditions change.

-Priority Engine

Scores requests based on severity, population affected, location risk, and time sensitivity to simulate real control‑room decision making.

-Resource Optimizer

Uses a knapsack‑style algorithm to maximize impact when resources are limited.

-Volunteer Coordinator

Matches available volunteers by skill and proximity for efficient deployment.

-Alert System

Processes disaster events and notifies subscribed entities (publish‑subscribe pattern).

-Evacuation Planner

Suggests safe routes depending on disaster type (e.g., avoid coastal zones during tsunami risk).


## Project Structure

    DRRAS/
     ├── drras_more_enhanced.cpp   # Main system implementation
     ├── README.md                 # Documentation
     ├── Design.png                # Architecture diagram
     ├── Project_structure.txt     # Additional design notes
     ├── Screenshots/              # Demo images
     └── .gitignore

> Runtime files (generated during execution) are intentionally excluded from the repository.


Build & Run

-Requirements

* Windows OS
* MinGW‑w64 or compatible g++ compiler
* C++17 support

-Compile

bash

    g++ drras_more_enhanced.cpp -std=c++17 -O2 -o drras

-Run

bash

    drras


Usage Flow

1. User reports disaster
2. System calculates urgency score
3. Request enters priority queue
4. Admin dispatches teams
5. Optimal route computed
6. Volunteers assigned
7. Resources allocated
8. Alerts broadcast
9. Request resolved


Screenshots

See the `Screenshots/` directory for example runs and module demonstrations.


Learning Outcomes

This project demonstrates practical application of:

* Translating DSA theory into large systems
* Resource‑constrained optimization
* Event‑driven architecture
* Modular C++ design
* Real‑time decision simulation


Future Improvements

* GUI dashboard
* Multi‑threaded simulation
* Real map API integration
* Machine learning priority prediction
* Persistent database storage


Contributing

Contributions, bug reports, and feature suggestions are welcome. Please open an issue before submitting major changes.


License

MIT License

Author
Purnendu Raghav Srivastava
JIIT
