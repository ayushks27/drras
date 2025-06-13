#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <windows.h>
#include <queue>
#include <fstream>
#include <functional>
#include <cctype>
#include <sstream>
#include <set>
#include <stack>
#include <bits/stdc++.h>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <ctime>

#define CLEAR_SCREEN_CMD "cls"
#define INF INT_MAX

using namespace std;

// Color codes for terminal
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";
const string UNDERLINE = "\033[4m";
const string BLINK = "\033[5m";
const string REVERSE = "\033[7m";

// Function to print a decorative border
void printBorder(int length = 100, char symbol = '=') {
    cout << MAGENTA;
    for (int i = 0; i < length; i++) {
        cout << symbol;
    }
    cout << RESET << endl;
}

// Function to print a centered title
void printCenteredTitle(const string& title, int width = 100) {
    int padding = (width - title.length()) / 2;
    cout << BLUE << BOLD;
    for (int i = 0; i < padding; i++) cout << " ";
    cout << title << RESET << endl;
}

// Function to clear screen with a nice effect
void clearScreen() {
    system(CLEAR_SCREEN_CMD);
    cout << GREEN;
    printBorder(100, '*');
    printCenteredTitle("DISASTER RELIEF & RESOURCE ALLOCATION SYSTEM");
    printBorder(100, '*');
    cout << RESET;
}

// Multi-language support
unordered_map<string, unordered_map<string, string>> translations = {
    {"en", {
        {"welcome", "Welcome to Disaster Relief System"},
        {"login", "Login"},
        {"signup", "Sign Up"},
        {"admin", "Admin"},
        {"home", "Home"},
        {"precautions", "Precautions"},
        {"helpline", "Helpline Services"},
        {"request", "Rescue Request"},
        {"feedback", "Feedback"},
        {"exit", "Exit"},
        {"select_option", "Select an option: "},
        {"invalid_choice", "Invalid choice. Please try again."}
    }},
    {"es", {
        {"welcome", "Bienvenido al Sistema de Ayuda en Desastres"},
        {"login", "Iniciar sesión"},
        {"signup", "Registrarse"},
        {"admin", "Administrador"},
        {"home", "Inicio"},
        {"precautions", "Precauciones"},
        {"helpline", "Servicios de Ayuda"},
        {"request", "Solicitud de Rescate"},
        {"feedback", "Comentarios"},
        {"exit", "Salir"},
        {"select_option", "Seleccione una opción: "},
        {"invalid_choice", "Opción no válida. Por favor intente de nuevo."}
    }},
    {"fr", {
        {"welcome", "Bienvenue dans le système d'aide aux sinistrés"},
        {"login", "Connexion"},
        {"signup", "S'inscrire"},
        {"admin", "Administrateur"},
        {"home", "Accueil"},
        {"precautions", "Précautions"},
        {"helpline", "Services d'assistance"},
        {"request", "Demande de sauvetage"},
        {"feedback", "Commentaires"},
        {"exit", "Sortie"},
        {"select_option", "Sélectionnez une option: "},
        {"invalid_choice", "Choix invalide. Veuillez réessayer."}
    }}
};

string currentLanguage = "en";

string translate(const string& key) {
    if (translations[currentLanguage].find(key) != translations[currentLanguage].end()) {
        return translations[currentLanguage][key];
    }
    return key; // Return key if translation not found
}

// Graph class for Dijkstra's algorithm with enhanced routing
class Graph {
private:
    int V;
    vector<pair<int, int>>* adj;
    unordered_map<int, string> cityNames;

public:
    Graph(int vertices) : V(vertices) {
        adj = new vector<pair<int, int>>[V];
    }

    // Struct for hashing pairs (needed for unordered_map)
    struct pair_hash {
        template <class T1, class T2>
        size_t operator() (const pair<T1, T2> &pair) const {
            return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
        }
    };

    unordered_map<pair<int, int>, bool, pair_hash> blockedEdges;
    unordered_map<pair<int, int>, int, pair_hash> trafficConditions; // 1-10 scale

    void addEdge(int u, int v, int w) {
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    void setCityName(int index, const string& name) {
        cityNames[index] = name;
    }

    string getCityName(int index) const {
        if (cityNames.find(index) != cityNames.end()) {
            return cityNames.at(index);
        }
        return "City " + to_string(index);
    }

    // Block an edge between u and v
    void blockEdge(int u, int v) {
        blockedEdges[make_pair(min(u, v), max(u, v))] = true;
    }

    // Unblock an edge between u and v
    void unblockEdge(int u, int v) {
        blockedEdges.erase(make_pair(min(u, v), max(u, v)));
    }

    // Check if an edge is blocked
    bool isEdgeBlocked(int u, int v) const {
        auto key = make_pair(min(u, v), max(u, v));
        return blockedEdges.find(key) != blockedEdges.end();
    }

    // Set traffic condition for an edge (1-10 scale)
    void setTrafficCondition(int u, int v, int condition) {
        if (condition < 1) condition = 1;
        if (condition > 10) condition = 10;
        trafficConditions[make_pair(min(u, v), max(u, v))] = condition;
    }

    // Get traffic condition for an edge
    int getTrafficCondition(int u, int v) const {
        auto key = make_pair(min(u, v), max(u, v));
        if (trafficConditions.find(key) != trafficConditions.end()) {
            return trafficConditions.at(key);
        }
        return 1; // Default to light traffic
    }

    // Get adjusted weight based on traffic conditions
    int getAdjustedWeight(int u, int v, int baseWeight) const {
        if (isEdgeBlocked(u, v)) {
            return INF;
        }
        int traffic = getTrafficCondition(u, v);
        return baseWeight * (1 + traffic / 5); // Increase weight based on traffic
    }

    vector<int> dijkstra(int src) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> dist(V, INF);

        pq.emplace(0, src);
        dist[src] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int baseWeight = neighbor.second;
                int adjustedWeight = getAdjustedWeight(u, v, baseWeight);

                if (dist[v] > dist[u] + adjustedWeight) {
                    dist[v] = dist[u] + adjustedWeight;
                    pq.emplace(dist[v], v);
                }
            }
        }

        return dist;
    }

    vector<int> getShortestPath(int src, int dest) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> dist(V, INF);
        vector<int> parent(V, -1);

        pq.emplace(0, src);
        dist[src] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == dest) break;

            for (auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int baseWeight = neighbor.second;
                int adjustedWeight = getAdjustedWeight(u, v, baseWeight);

                if (dist[v] > dist[u] + adjustedWeight) {
                    dist[v] = dist[u] + adjustedWeight;
                    parent[v] = u;
                    pq.emplace(dist[v], v);
                }
            }
        }

        vector<int> path;
        if (dist[dest] == INF) return path;

        for (int v = dest; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());

        return path;
    }

    vector<pair<int, int>> getBlockedEdges() const {
        vector<pair<int, int>> blocked;
        for (const auto& entry : blockedEdges) {
            blocked.push_back(entry.first);
        }
        return blocked;
    }

    const vector<pair<int, int>>* getAdj() const {
        return adj;
    }
};

// Feedback System
struct FeedbackEntry {
    string userName;
    int rating;
    string suggestion;

    FeedbackEntry(const string& name, int rate, const string& sugg)
        : userName(name), rating(rate), suggestion(sugg) {}
};

class FeedbackSystem {
private:
    vector<FeedbackEntry> feedbackQueue;

    static bool compareFeedback(const FeedbackEntry& a, const FeedbackEntry& b) {
        return a.rating > b.rating;
    }

    void saveFeedbackToFile() {
        ofstream file("feedback.txt");
        if (file.is_open()) {
            for (const auto& feedback : feedbackQueue) {
                file << feedback.userName << "," << feedback.rating << "," << feedback.suggestion << endl;
            }
            file.close();
        }
        else {
            cout << RED << "Error: Unable to open file for saving feedback.\n" << RESET;
        }
    }

    void loadFeedbackFromFile() {
        ifstream file("feedback.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.rfind(',');

                if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
                    string name = line.substr(0, pos1);
                    try {
                        int rating = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                        string suggestion = line.substr(pos2 + 1);
                        feedbackQueue.emplace_back(name, rating, suggestion);
                    }
                    catch (const invalid_argument&) {
                        cout << RED << "Error reading feedback data.\n" << RESET;
                    }
                }
            }
            file.close();
            sort(feedbackQueue.begin(), feedbackQueue.end(), compareFeedback);
        }
    }

public:
    FeedbackSystem() {
        loadFeedbackFromFile();
    }

    ~FeedbackSystem() {
        saveFeedbackToFile();
    }

    void addFeedback(const string& name, int rating, const string& suggestion) {
        if (rating < 1 || rating > 10) {
            cout << RED << "Invalid rating. Please provide a rating between 1 and 10.\n" << RESET;
            return;
        }

        feedbackQueue.emplace_back(name, rating, suggestion);
        sort(feedbackQueue.begin(), feedbackQueue.end(), compareFeedback);
        cout << GREEN << "Thank you, " << name << ", for your feedback!\n" << RESET;
    }

    void displayFeedback() {
        if (feedbackQueue.empty()) {
            cout << YELLOW << "No feedback available.\n" << RESET;
            return;
        }

        cout << CYAN << "--- Feedback Priority List ---\n" << RESET;
        for (const auto& feedback : feedbackQueue) {
            cout << BOLD << "Name: " << RESET << feedback.userName << "\n"
                 << BOLD << "Rating: " << RESET << feedback.rating << "/10\n"
                 << BOLD << "Suggestion: " << RESET << feedback.suggestion << "\n";
            printBorder(50, '-');
        }
    }

    bool collectFeedback() {
        string userName, suggestion;
        int rating;
        char choice = ' ';

        do {
            clearScreen();
            printCenteredTitle("FEEDBACK FORM");
            printBorder();
            
            cout << BOLD << "\nEnter your name: " << RESET;
            cin.ignore();
            getline(cin, userName);

            cout << BOLD << "\nRate the app on a scale of 1-10: " << RESET;
            cin >> rating;

            while (cin.fail() || rating < 1 || rating > 10) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "Invalid rating. Enter a number between 1 and 10: " << RESET;
                cin >> rating;
            }

            cout << BOLD << "\nAny suggestions to improve? " << RESET;
            cin.ignore();
            getline(cin, suggestion);

            addFeedback(userName, rating, suggestion);

            while (choice != 'B') {
                cout << YELLOW << "\nPress 'B' to go back: " << RESET;
                cin >> choice;
                choice = toupper(choice);
            }

            if (choice == 'B') {
                clearScreen();
                return true;
            }

        } while (choice == 'Y');
        return false;
    }
};

// Request class with enhanced features
class Request {
public:
    string name, phone, disasterType, severity, city, province, location, comments;
    int priority;
    int foodNeeded;
    int waterNeeded;
    int medNeeded;
    int populationAffected;
    time_t timestamp;
    bool isActive;

    Request(const string& n = "", const string& p = "", const string& d = "", const string& s = "",
        const string& c = "", const string& pr = "", const string& l = "", const string& com = "", 
        int pri = 0, int food = 0, int water = 0, int med = 0, int pop = 0)
        : name(n), phone(p), disasterType(d), severity(s), city(c), province(pr), 
          location(l), comments(com), priority(pri), foodNeeded(food), 
          waterNeeded(water), medNeeded(med), populationAffected(pop), 
          timestamp(time(nullptr)), isActive(true) {}

    bool operator<(const Request& other) const {
        return priority < other.priority;
    }

    string getFormattedTime() const {
    char buffer[80];
    struct tm* timeinfo = localtime(&timestamp);  // Fix: use pointer return
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

};

// Disaster Zone class for dynamic programming
class DisasterZone {
public:
    string name;
    int severity;
    int resourcesNeeded;
    int distance;

    DisasterZone(string n, int s, int r, int d) 
        : name(n), severity(s), resourcesNeeded(r), distance(d) {}
};

// Volunteer class
class Volunteer {
public:
    string name;
    string phone;
    string email;
    string skills;
    string location;
    bool isAvailable;

    Volunteer(const string& n, const string& p, const string& e, const string& s, const string& l)
        : name(n), phone(p), email(e), skills(s), location(l), isAvailable(true) {}
};

// Volunteer Management System
class VolunteerSystem {
private:
    vector<Volunteer> volunteers;
    unordered_map<string, vector<Volunteer*>> volunteersByLocation;

    void loadVolunteersFromFile() {
        ifstream file("volunteers.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                vector<string> tokens;
                string token;
                istringstream tokenStream(line);
                while (getline(tokenStream, token, '|')) {
                    tokens.push_back(token);
                }
                if (tokens.size() >= 5) {
                    volunteers.emplace_back(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
                    volunteers.back().isAvailable = (tokens.size() > 5) ? (tokens[5] == "1") : true;
                }
            }
            file.close();
        }
        updateLocationIndex();
    }

    void saveVolunteersToFile() {
        ofstream file("volunteers.txt");
        if (file.is_open()) {
            for (const auto& volunteer : volunteers) {
                file << volunteer.name << "|" << volunteer.phone << "|" << volunteer.email << "|"
                     << volunteer.skills << "|" << volunteer.location << "|" 
                     << (volunteer.isAvailable ? "1" : "0") << "\n";
            }
            file.close();
        }
    }

    void updateLocationIndex() {
        volunteersByLocation.clear();
        for (auto& volunteer : volunteers) {
            volunteersByLocation[volunteer.location].push_back(&volunteer);
        }
    }

public:
    VolunteerSystem() {
        loadVolunteersFromFile();
    }

    ~VolunteerSystem() {
        saveVolunteersToFile();
    }

    void addVolunteer(const Volunteer& volunteer) {
        volunteers.push_back(volunteer);
        volunteersByLocation[volunteer.location].push_back(&volunteers.back());
        saveVolunteersToFile();
        cout << GREEN << "Volunteer added successfully!\n" << RESET;
    }

    vector<Volunteer*> findAvailableVolunteers(const string& location, const string& skill = "") {
        vector<Volunteer*> result;
        if (volunteersByLocation.find(location) != volunteersByLocation.end()) {
            for (auto volunteer : volunteersByLocation[location]) {
                if (volunteer->isAvailable && 
                    (skill.empty() || volunteer->skills.find(skill) != string::npos)) {
                    result.push_back(volunteer);
                }
            }
        }
        return result;
    }

    void displayVolunteers() const {
        if (volunteers.empty()) {
            cout << YELLOW << "No volunteers registered.\n" << RESET;
            return;
        }

        cout << CYAN << "--- Registered Volunteers ---\n" << RESET;
        for (const auto& volunteer : volunteers) {
            cout << BOLD << "Name: " << RESET << volunteer.name << "\n"
                 << BOLD << "Phone: " << RESET << volunteer.phone << "\n"
                 << BOLD << "Email: " << RESET << volunteer.email << "\n"
                 << BOLD << "Skills: " << RESET << volunteer.skills << "\n"
                 << BOLD << "Location: " << RESET << volunteer.location << "\n"
                 << BOLD << "Status: " << RESET 
                 << (volunteer.isAvailable ? GREEN + string("Available") : RED + string("Unavailable")) << RESET << "\n";
            printBorder(50, '-');
        }
    }

    void updateVolunteerAvailability(const string& email, bool availability) {
        for (auto& volunteer : volunteers) {
            if (volunteer.email == email) {
                volunteer.isAvailable = availability;
                saveVolunteersToFile();
                cout << GREEN << "Volunteer availability updated.\n" << RESET;
                return;
            }
        }
        cout << RED << "Volunteer not found.\n" << RESET;
    }
};

// Dynamic Programming for resource allocation
class ResourceAllocator {
private:
    vector<DisasterZone> zones;

public:
    void addZone(const DisasterZone& zone) {
        zones.push_back(zone);
    }

    // Knapsack-like approach for resource allocation
    vector<string> allocateResources(int totalResources) {
        int n = zones.size();
        vector<vector<int>> dp(n + 1, vector<int>(totalResources + 1, 0));
        vector<vector<bool>> keep(n + 1, vector<bool>(totalResources + 1, false));

        for (int i = 1; i <= n; i++) {
            for (int w = 0; w <= totalResources; w++) {
                if (zones[i-1].resourcesNeeded <= w && 
                    dp[i-1][w - zones[i-1].resourcesNeeded] + zones[i-1].severity > dp[i-1][w]) {
                    dp[i][w] = dp[i-1][w - zones[i-1].resourcesNeeded] + zones[i-1].severity;
                    keep[i][w] = true;
                } else {
                    dp[i][w] = dp[i-1][w];
                }
            }
        }

        vector<string> result;
        int w = totalResources;
        for (int i = n; i > 0; i--) {
            if (keep[i][w]) {
                result.push_back(zones[i-1].name);
                w -= zones[i-1].resourcesNeeded;
            }
        }

        return result;
    }
};

// Real-Time Disaster Alert System
class DisasterAlertSystem {
private:
    queue<pair<string, string>> alerts; // <disaster type, location>
    unordered_map<string, vector<string>> subscribers; // <email, disaster types>

public:
    void addAlert(const string& disasterType, const string& location) {
        alerts.push(make_pair(disasterType, location));
    }

    void processAlerts() {
        while (!alerts.empty()) {
            auto alert = alerts.front();
            alerts.pop();
            
            cout << RED << BOLD << "ALERT: " << RESET << alert.first << " in " << alert.second << "\n";
            
            // Notify subscribers interested in this type of disaster
            for (const auto& subscriber : subscribers) {
                if (find(subscriber.second.begin(), subscriber.second.end(), alert.first) != subscriber.second.end()) {
                    // In a real system, we would send an email or push notification
                    cout << CYAN << "Notifying " << subscriber.first << " about " 
                         << alert.first << " in " << alert.second << "\n" << RESET;
                }
            }
        }
    }

    void subscribe(const string& email, const vector<string>& disasterTypes) {
        subscribers[email] = disasterTypes;
        cout << GREEN << email << " subscribed to alerts for specified disaster types.\n" << RESET;
    }

    void unsubscribe(const string& email) {
        subscribers.erase(email);
        cout << YELLOW << email << " unsubscribed from all alerts.\n" << RESET;
    }

    void simulateRandomAlert() {
        vector<string> disasterTypes = {"Earthquake", "Flood", "Fire", "Hurricane", "Tornado"};
        vector<string> locations = {"Delhi", "Mumbai", "Bangalore", "Kolkata", "Chennai"};
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> disType(0, disasterTypes.size() - 1);
        uniform_int_distribution<> disLoc(0, locations.size() - 1);
        
        string type = disasterTypes[disType(gen)];
        string loc = locations[disLoc(gen)];
        
        addAlert(type, loc);
        cout << YELLOW << "Simulated alert: " << type << " in " << loc << "\n" << RESET;
    }
};

// Main Rescue Management System
class RescueManagementSystem {
private:
    priority_queue<Request> rescueRequests;
    const string adminEmail = "admin@dras.com";
    const string adminPassword = "admin123";
    DisasterAlertSystem alertSystem;
    VolunteerSystem volunteerSystem;

    struct SupplyNode {
        string category;
        string itemName;
        int quantity;
        SupplyNode* next;

        SupplyNode(string cat, string name, int qty) 
            : category(cat), itemName(name), quantity(qty), next(nullptr) {}
    };

    SupplyNode* head;
    Graph* cityNetwork;
    unordered_map<string, int> cityIndices;
    int nextCityIndex;

    // AI Priority Scoring parameters
    const map<string, int> disasterSeverityWeights = {
        {"earthquake", 10}, {"fire", 8}, {"flood", 7}, 
        {"hurricane", 9}, {"tornado", 9}, {"tsunami", 10}
    };

    const map<string, int> locationRiskFactors = {
        {"urban", 8}, {"suburban", 5}, {"rural", 3}, 
        {"coastal", 7}, {"mountain", 6}
    };

public:
    RescueManagementSystem() : head(nullptr), nextCityIndex(0), cityNetwork(nullptr) {
        initializeSupplies();
        initializeCityNetwork();
        
        // Simulate some initial alerts
        alertSystem.addAlert("Earthquake", "Delhi");
        alertSystem.addAlert("Flood", "Mumbai");
    }

    ~RescueManagementSystem() {
        // Clean up supplies linked list
        while (head != nullptr) {
            SupplyNode* temp = head;
            head = head->next;
            delete temp;
        }
        
        if (cityNetwork) {
            delete cityNetwork;
        }
    }

private:
    void initializeSupplies() {
        // Medical Supplies
        head = new SupplyNode("Medical Supplies", "Aspirin", 100);
        head->next = new SupplyNode("Medical Supplies", "Bandages", 200);
        head->next->next = new SupplyNode("Medical Supplies", "First Aid Kits", 50);
        
        // Basic Necessities
        head->next->next->next = new SupplyNode("Basic Necessities", "Water Bottles", 100);
        head->next->next->next->next = new SupplyNode("Basic Necessities", "Blankets", 75);
        
        // Rescue Personnel
        head->next->next->next->next->next = new SupplyNode("Rescue Personnel", "Doctor Squads", 6);
    }

    void initializeCityNetwork() {
        // Initialize with some common cities
        addCityToNetwork("Delhi");
        addCityToNetwork("Mumbai");
        addCityToNetwork("Bangalore");
        addCityToNetwork("Kolkata");
        addCityToNetwork("Chennai");
        addCityToNetwork("Noida");
        
        // Create a network with some connections
        cityNetwork = new Graph(nextCityIndex);
        
        // Add connections with distances (weights)
        cityNetwork->addEdge(cityIndices["Delhi"], cityIndices["Mumbai"], 1400);
        cityNetwork->addEdge(cityIndices["Delhi"], cityIndices["Bangalore"], 2150);
        cityNetwork->addEdge(cityIndices["Mumbai"], cityIndices["Bangalore"], 980);
        cityNetwork->addEdge(cityIndices["Mumbai"], cityIndices["Kolkata"], 1650);
        cityNetwork->addEdge(cityIndices["Bangalore"], cityIndices["Chennai"], 350);
        cityNetwork->addEdge(cityIndices["Chennai"], cityIndices["Kolkata"], 1660);
        cityNetwork->addEdge(cityIndices["Delhi"], cityIndices["Noida"], 50);
        
        // Set city names in the graph
        for (const auto& pair : cityIndices) {
            cityNetwork->setCityName(pair.second, pair.first);
        }
        
        // Simulate some traffic conditions
        cityNetwork->setTrafficCondition(cityIndices["Delhi"], cityIndices["Noida"], 7); // Heavy traffic
        cityNetwork->setTrafficCondition(cityIndices["Mumbai"], cityIndices["Bangalore"], 4); // Moderate traffic
    }

    void addCityToNetwork(const string& city) {
        if (cityIndices.find(city) == cityIndices.end()) {
            cityIndices[city] = nextCityIndex++;
        }
    }

    // AI Priority Scoring function
    int calculateAIPriority(const Request& req) {
        int priority = 0;
        
        // Base priority from user input
        priority += req.priority * 10;
        
        // Disaster type severity
        string lowerDisaster = req.disasterType;
        transform(lowerDisaster.begin(), lowerDisaster.end(), lowerDisaster.begin(), ::tolower);
        if (disasterSeverityWeights.find(lowerDisaster) != disasterSeverityWeights.end()) {
            priority += disasterSeverityWeights.at(lowerDisaster) * 5;
        }
        
        // Population affected
        priority += min(req.populationAffected / 100, 50); // Cap at 50 for 5000+ people
        
        // Location risk factor
        string lowerLocation = req.location;
        transform(lowerLocation.begin(), lowerLocation.end(), lowerLocation.begin(), ::tolower);
        for (const auto& riskFactor : locationRiskFactors) {
            if (lowerLocation.find(riskFactor.first) != string::npos) {
                priority += riskFactor.second * 3;
                break;
            }
        }
        
        // Time decay factor (more recent = higher priority)
        time_t now = time(nullptr);
        double hoursSinceReport = difftime(now, req.timestamp) / 3600.0;
        priority += max(0, 20 - static_cast<int>(hoursSinceReport * 2));
        
        return min(priority, 100); // Cap at 100
    }

    // Generate a heatmap of resource needs
    void generateResourceHeatmap() {
        if (rescueRequests.empty()) {
            cout << YELLOW << "No requests available to generate heatmap.\n" << RESET;
            return;
        }

        unordered_map<string, tuple<int, int, int>> cityResources; // city -> <food, water, med>

        // Create a temporary queue to process all requests
        priority_queue<Request> tempQueue = rescueRequests;
        while (!tempQueue.empty()) {
            Request req = tempQueue.top();
            tempQueue.pop();

            if (!req.isActive) continue;

            auto& resources = cityResources[req.city];
            get<0>(resources) += req.foodNeeded;
            get<1>(resources) += req.waterNeeded;
            get<2>(resources) += req.medNeeded;
        }

        // Display the heatmap
        cout << CYAN << "--- Resource Needs Heatmap ---\n" << RESET;
        cout << BOLD << setw(15) << left << "City" 
             << setw(15) << "Food Needed" 
             << setw(15) << "Water Needed" 
             << setw(15) << "Med Needed" << RESET << "\n";
        
        for (const auto& entry : cityResources) {
            cout << setw(15) << left << entry.first
                 << setw(15) << get<0>(entry.second)
                 << setw(15) << get<1>(entry.second)
                 << setw(15) << get<2>(entry.second) << "\n";
            
            // Visual representation
            int total = get<0>(entry.second) + get<1>(entry.second) + get<2>(entry.second);
            int stars = min(50, total / 10);
            cout << "    " << string(stars, '*') << "\n";
        }
    }

    // Generate predictive evacuation routes
    void generateEvacuationRoutes(const string& city, const string& disasterType) {
        if (cityIndices.find(city) == cityIndices.end()) {
            cout << RED << "City not found in network.\n" << RESET;
            return;
        }

        int source = cityIndices[city];
        vector<int> safeCities;
        
        // Determine safe cities based on disaster type
        if (disasterType == "earthquake" || disasterType == "tsunami") {
            // Avoid coastal cities for these disasters
            for (const auto& pair : cityIndices) {
                if (pair.first != city && 
                    pair.first.find("Coastal") == string::npos &&
                    pair.first.find("Beach") == string::npos) {
                    safeCities.push_back(pair.second);
                }
            }
        }
        else if (disasterType == "flood") {
            // Avoid low-lying cities
            for (const auto& pair : cityIndices) {
                if (pair.first != city && 
                    pair.first.find("Valley") == string::npos &&
                    pair.first.find("Riverside") == string::npos) {
                    safeCities.push_back(pair.second);
                }
            }
        }
        else {
            // For other disasters, all cities are potential safe havens
            for (const auto& pair : cityIndices) {
                if (pair.first != city) {
                    safeCities.push_back(pair.second);
                }
            }
        }

        if (safeCities.empty()) {
            cout << YELLOW << "No safe cities identified for this disaster type.\n" << RESET;
            return;
        }

        cout << CYAN << "--- Predictive Evacuation Routes from " << city << " ---\n" << RESET;
        cout << "Disaster type: " << disasterType << "\n\n";

        // Get shortest paths to all safe cities
        vector<int> distances = cityNetwork->dijkstra(source);
        
        // Sort safe cities by distance
        vector<pair<int, int>> sortedSafeCities; // <distance, cityIndex>
        for (int dest : safeCities) {
            if (distances[dest] != INF) {
                sortedSafeCities.emplace_back(distances[dest], dest);
            }
        }
        sort(sortedSafeCities.begin(), sortedSafeCities.end());

        // Display top 3 evacuation routes
        int count = 0;
        for (const auto& entry : sortedSafeCities) {
            if (count >= 3) break;
            
            int distance = entry.first;
            int dest = entry.second;
            
            vector<int> path = cityNetwork->getShortestPath(source, dest);
            
            cout << BOLD << (count + 1) << ". To " << cityNetwork->getCityName(dest) 
                 << " (" << distance << " km)" << RESET << ":\n  ";
            
            for (size_t i = 0; i < path.size(); ++i) {
                cout << cityNetwork->getCityName(path[i]);
                if (i != path.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << "\n\n";
            count++;
        }
    }

public:
    bool userLogin() {
        clearScreen();
        printCenteredTitle("USER LOGIN PAGE");
        printBorder();
        
        string email, password;
        cout << BOLD << "\n\nEnter your email: " << RESET;
        cin >> email;
        cout << BOLD << "\nEnter your password: " << RESET;
        cin >> password;

        ifstream inFile("userdata.txt");
        if (!inFile.is_open()) {
            cout << RED << "\nError opening user data file!\n" << RESET;
            return false;
        }

        string storedEmail, storedPassword;
        while (inFile >> storedEmail >> storedPassword) {
            if (storedEmail == email && storedPassword == password) {
                cout << GREEN << "\nLogin successful!\n" << RESET;
                return true;
            }
        }
        cout << RED << "Invalid credentials.\n" << RESET;
        system("pause");
        return false;
    }

    void userSignup() {
        clearScreen();
        printCenteredTitle("USER SIGNUP PAGE");
        printBorder();
        
        string email, password;
        cout << BOLD << "\n\nEnter a new email: " << RESET;
        cin >> email;
        cout << BOLD << "\nEnter a new password: " << RESET;
        cin >> password;

        ofstream outFile("userdata.txt", ios::app);
        if (!outFile.is_open()) {
            cout << RED << "\nError saving user data!\n" << RESET;
            return;
        }

        outFile << email << " " << password << endl;
        cout << GREEN << "\nSignup successful!\n" << RESET;
    }

    bool adminLogin() {
        clearScreen();
        printCenteredTitle("ADMIN LOGIN PAGE");
        printBorder();
        
        string email, password;
        bool validCredentials = false;
        int attempts = 0;
        const int maxAttempts = 3;

        while (attempts < maxAttempts && !validCredentials) {
            cout << BOLD << "\n\nEnter admin email: " << RESET;
            cin >> email;
            cout << BOLD << "\nEnter admin password: " << RESET;
            cin >> password;

            if (email != adminEmail) {
                cout << RED << "\nERROR: Invalid admin username!\n" << RESET;
            }
            else if (password != adminPassword) {
                cout << RED << "\nERROR: Invalid admin password!\n" << RESET;
            }
            else {
                validCredentials = true;
                cout << GREEN << "\nAdmin login successful!\n" << RESET;
            }

            if (!validCredentials) {
                attempts++;
                cout << YELLOW << "\nAttempts remaining: " << (maxAttempts - attempts) << "\n" << RESET;
                if (attempts < maxAttempts) {
                    cout << "\nPlease try again...\n";
                    Sleep(2000);
                    clearScreen();
                    printCenteredTitle("ADMIN LOGIN PAGE");
                    printBorder();
                }
            }
        }

        if (!validCredentials) {
            cout << RED << "\nMaximum login attempts reached!\n" << RESET;
            cout << "\nReturning to main menu...\n";
            Sleep(3000);
        }

        return validCredentials;
    }

    void addRescueRequest(const Request& req) {
        Request modifiedReq = req;
        modifiedReq.priority = calculateAIPriority(req); // Apply AI scoring
        rescueRequests.push(modifiedReq);
        cout << GREEN << "Rescue request added with priority " << modifiedReq.priority << ".\n" << RESET;
        
        // Add a real-time alert for this disaster
        alertSystem.addAlert(req.disasterType, req.city);
    }

    void saveRequestToFile(const Request& req) {
        ofstream outFile("dataofrequests.txt", ios::app);
        if (!outFile.is_open()) {
            cout << RED << "Error saving request to file.\n" << RESET;
            return;
        }

        outFile << req.name << "|" << req.phone << "|" << req.disasterType << "|"
            << req.severity << "|" << req.city << "|" << req.province << "|"
            << req.location << "|" << req.comments << "|" << req.priority << "|"
            << req.foodNeeded << "|" << req.waterNeeded << "|" << req.medNeeded << "|"
            << req.populationAffected << "|" << req.timestamp << "|" << req.isActive << "\n";
        cout << GREEN << "Request saved to file.\n" << RESET;
    }

    void displayRequests() {
        if (rescueRequests.empty()) {
            cout << YELLOW << "No rescue requests available.\n" << RESET;
            return;
        }

        auto tempQueue = rescueRequests;
        while (!tempQueue.empty()) {
            Request req = tempQueue.top();
            cout << BOLD << "Name: " << RESET << req.name << "\n"
                 << BOLD << "Disaster: " << RESET << req.disasterType << "\n"
                 << BOLD << "Severity: " << RESET << req.severity << "\n"
                 << BOLD << "City: " << RESET << req.city << "\n"
                 << BOLD << "Priority: " << RESET << req.priority << "\n"
                 << BOLD << "Population Affected: " << RESET << req.populationAffected << "\n"
                 << BOLD << "Time Reported: " << RESET << req.getFormattedTime() << "\n";
            printBorder(50, '-');
            tempQueue.pop();
        }
    }

    void dispatchSupplies() {
        if (rescueRequests.empty()) {
            cout << YELLOW << "No requests available to dispatch.\n" << RESET;
            return;
        }

        string userName;
        cout << BOLD << "Enter the name of the user to dispatch supplies to: " << RESET;
        cin.ignore();
        getline(cin, userName);

        priority_queue<Request> tempQueue;
        bool found = false;

        while (!rescueRequests.empty()) {
            Request req = rescueRequests.top();
            rescueRequests.pop();

            if (req.name == userName) {
                found = true;
                cout << GREEN << "\nDispatching supplies to " << req.name << " at " << req.location << "...\n" << RESET;

                // Find available volunteers in the area
                auto availableVolunteers = volunteerSystem.findAvailableVolunteers(req.city);
                if (!availableVolunteers.empty()) {
                    cout << CYAN << "\nAvailable Volunteers in " << req.city << ":\n" << RESET;
                    for (auto volunteer : availableVolunteers) {
                        cout << "- " << volunteer->name << " (" << volunteer->skills << ")\n";
                    }
                    
                    string assign;
                    cout << BOLD << "\nAssign volunteers? (yes/no): " << RESET;
                    cin >> assign;
                    if (assign == "yes") {
                        for (auto volunteer : availableVolunteers) {
                            volunteer->isAvailable = false;
                            cout << GREEN << "Assigned " << volunteer->name << " to this operation.\n" << RESET;
                        }
                    }
                }

                // Calculate optimal route using Dijkstra's algorithm
                if (cityIndices.find(req.city) != cityIndices.end()) {
                    int destIndex = cityIndices[req.city];
                    vector<int> path = cityNetwork->getShortestPath(0, destIndex); // Assuming 0 is the central hub
                    
                    if (!path.empty()) {
                        cout << CYAN << "Optimal delivery route: " << RESET;
                        for (int i = 0; i < path.size(); i++) {
                            cout << cityNetwork->getCityName(path[i]);
                            if (i != path.size() - 1) cout << " -> ";
                        }
                        cout << "\n" << CYAN << "Estimated distance: " << RESET 
                             << cityNetwork->dijkstra(0)[destIndex] << " km\n";
                    } else {
                        cout << YELLOW << "No available route to " << req.city 
                             << ". Using alternative transport.\n" << RESET;
                    }
                } else {
                    cout << YELLOW << "City not in network. Using default dispatch protocol.\n" << RESET;
                }

                // Dispatch supplies
                SupplyNode* temp = head;
                while (temp != nullptr) {
                    int quantity;
                    cout << BOLD << "Enter quantity for " << temp->itemName 
                         << " (available: " << temp->quantity << "): " << RESET;
                    cin >> quantity;

                    if (quantity > temp->quantity) {
                        cout << YELLOW << "Not enough " << temp->itemName 
                             << " available. Dispatching " << temp->quantity << ".\n" << RESET;
                        quantity = temp->quantity;
                    }

                    temp->quantity -= quantity;
                    temp = temp->next;
                }

                cout << GREEN << "\nSupplies have been successfully dispatched to " 
                     << req.name << ".\n" << RESET;
                updateFilesAfterDispatch(req);
                break;
            } else {
                tempQueue.push(req);
            }
        }

        if (!found) {
            cout << RED << "User not found.\n" << RESET;
        }

        // Restore the remaining requests
        while (!tempQueue.empty()) {
            rescueRequests.push(tempQueue.top());
            tempQueue.pop();
        }
    }

    void optimizeDispatch() {
        if (rescueRequests.empty()) {
            cout << YELLOW << "No requests to optimize.\n" << RESET;
            return;
        }

        // Collect all requests
        vector<Request> allRequests;
        priority_queue<Request> tempQueue = rescueRequests;
        
        while (!tempQueue.empty()) {
            allRequests.push_back(tempQueue.top());
            tempQueue.pop();
        }

        // Calculate total needs
        int totalFood = 0, totalWater = 0, totalMed = 0;
        for (const auto& req : allRequests) {
            totalFood += req.foodNeeded;
            totalWater += req.waterNeeded;
            totalMed += req.medNeeded;
        }

        // Get available supplies
        int availableFood = getSupplyQuantity("Water Bottles");
        int availableWater = getSupplyQuantity("Energy Bars");
        int availableMed = getSupplyQuantity("First Aid Kits");

        cout << CYAN << "\n--- Resource Optimization ---\n" << RESET;
        cout << BOLD << "Total needed: " << RESET << "Food=" << totalFood 
             << ", Water=" << totalWater << ", Med=" << totalMed << "\n";
        cout << BOLD << "Available: " << RESET << "Food=" << availableFood 
             << ", Water=" << availableWater << ", Med=" << availableMed << "\n";

        if (availableFood >= totalFood && availableWater >= totalWater && availableMed >= totalMed) {
            cout << GREEN << "Enough resources available for all requests.\n" << RESET;
            return;
        }

        // Use greedy approach to prioritize based on severity and population
        sort(allRequests.begin(), allRequests.end(), [](const Request& a, const Request& b) {
            return (a.priority * a.populationAffected) > (b.priority * b.populationAffected);
        });

        cout << CYAN << "\nOptimized Dispatch Plan:\n" << RESET;
        for (const auto& req : allRequests) {
            // Calculate percentage to allocate based on priority
            double allocationFactor = (req.priority * req.populationAffected) / 100.0;
            int foodToSend = min(req.foodNeeded, static_cast<int>(availableFood * allocationFactor));
            int waterToSend = min(req.waterNeeded, static_cast<int>(availableWater * allocationFactor));
            int medToSend = min(req.medNeeded, static_cast<int>(availableMed * allocationFactor));

            if (foodToSend > 0 || waterToSend > 0 || medToSend > 0) {
                cout << BOLD << "To " << RESET << req.name << " at " << req.city << ":\n";
                cout << "  Food: " << foodToSend << "/" << req.foodNeeded << "\n";
                cout << "  Water: " << waterToSend << "/" << req.waterNeeded << "\n";
                cout << "  Med: " << medToSend << "/" << req.medNeeded << "\n\n";

                availableFood -= foodToSend;
                availableWater -= waterToSend;
                availableMed -= medToSend;
            }

            if (availableFood <= 0 && availableWater <= 0 && availableMed <= 0) break;
        }
    }

    int getSupplyQuantity(const string& itemName) {
        SupplyNode* temp = head;
        while (temp != nullptr) {
            if (temp->itemName == itemName) {
                return temp->quantity;
            }
            temp = temp->next;
        }
        return 0;
    }

    void updateFilesAfterDispatch(const Request& dispatchedReq) {
        ifstream inFile("dataofrequests.txt");
        ofstream outFile("tempfile.txt");

        if (!inFile.is_open() || !outFile.is_open()) {
            cout << RED << "Error accessing files for update.\n" << RESET;
            return;
        }

        string line;
        while (getline(inFile, line)) {
            if (line.find(dispatchedReq.name) == string::npos) {
                outFile << line << "\n";
            }
        }

        inFile.close();
        outFile.close();

        remove("dataofrequests.txt");
        rename("tempfile.txt", "dataofrequests.txt");

        cout << GREEN << "Dispatched request removed from file.\n" << RESET;
    }

    void addSupply(const string& item, int quantity) {
        SupplyNode* temp = head;
        bool found = false;

        while (temp != nullptr) {
            if (temp->itemName == item) {
                temp->quantity += quantity;
                cout << GREEN << "Added " << quantity << " units of " << item 
                     << ". New quantity: " << temp->quantity << RESET << endl;
                found = true;
                break;
            }
            temp = temp->next;
        }

        if (!found) {
            SupplyNode* newNode = new SupplyNode("Unknown Category", item, quantity);
            newNode->next = head;
            head = newNode;
            cout << GREEN << "Added " << quantity << " units of " << item 
                 << " as a new supply.\n" << RESET;
        }
    }

    void displaySupplies() {
        cout << CYAN << "--- Available Supplies ---\n" << RESET;
        SupplyNode* temp = head;
        while (temp != nullptr) {
            cout << BOLD << temp->itemName << ": " << RESET << temp->quantity 
                 << " units (" << temp->category << ")\n";
            temp = temp->next;
        }
    }

    void addRoadBlock() {
        string city1, city2;
        cout << BOLD << "Enter first city: " << RESET;
        cin >> city1;
        cout << BOLD << "Enter second city: " << RESET;
        cin >> city2;

        if (cityIndices.find(city1) == cityIndices.end() || cityIndices.find(city2) == cityIndices.end()) {
            cout << RED << "One or both cities not found in network.\n" << RESET;
            return;
        }

        int u = cityIndices[city1];
        int v = cityIndices[city2];
        cityNetwork->blockEdge(u, v);
        cout << YELLOW << "Road between " << city1 << " and " << city2 
             << " is now blocked.\n" << RESET;
        
        // Add an alert about the road block
        alertSystem.addAlert("Road Block", city1 + "-" + city2);
    }

    void removeRoadBlock() {
        string city1, city2;
        cout << BOLD << "Enter first city: " << RESET;
        cin >> city1;
        cout << BOLD << "Enter second city: " << RESET;
        cin >> city2;

        if (cityIndices.find(city1) == cityIndices.end() || cityIndices.find(city2) == cityIndices.end()) {
            cout << RED << "One or both cities not found in network.\n" << RESET;
            return;
        }

        int u = cityIndices[city1];
        int v = cityIndices[city2];
        cityNetwork->unblockEdge(u, v);
        cout << GREEN << "Road between " << city1 << " and " << city2 
             << " is now unblocked.\n" << RESET;
    }

    void setTrafficCondition() {
        string city1, city2;
        int condition;
        cout << BOLD << "Enter first city: " << RESET;
        cin >> city1;
        cout << BOLD << "Enter second city: " << RESET;
        cin >> city2;
        cout << BOLD << "Enter traffic condition (1-10 scale): " << RESET;
        cin >> condition;

        if (cityIndices.find(city1) == cityIndices.end() || cityIndices.find(city2) == cityIndices.end()) {
            cout << RED << "One or both cities not found in network.\n" << RESET;
            return;
        }

        int u = cityIndices[city1];
        int v = cityIndices[city2];
        cityNetwork->setTrafficCondition(u, v, condition);
        cout << YELLOW << "Traffic condition between " << city1 << " and " << city2 
             << " set to " << condition << "/10.\n" << RESET;
    }

    void viewRoadBlocks() {
        vector<pair<int, int>> blocked = cityNetwork->getBlockedEdges();
        if (blocked.empty()) {
            cout << YELLOW << "No roads are currently blocked.\n" << RESET;
            return;
        }
        
        cout << CYAN << "--- Currently Blocked Roads ---\n" << RESET;
        for (const auto& edge : blocked) {
            string city1 = cityNetwork->getCityName(edge.first);
            string city2 = cityNetwork->getCityName(edge.second);
            cout << RED << city1 << " <-> " << city2 << " (BLOCKED)\n" << RESET;
            
            // Show original distance
            int originalDistance = 0;
            const auto& adjList = cityNetwork->getAdj();
            for (const auto& neighbor : adjList[edge.first]) {
                if (neighbor.first == edge.second) {
                    originalDistance = neighbor.second;
                    break;
                }
            }
            cout << "  Original distance: " << originalDistance << " km\n";
            cout << "  Current status: " << RED << "BLOCKED" << RESET << " (∞ km)\n\n";
        }
        
        // Show effect on sample routes
        cout << CYAN << "\n--- Route Impact Examples ---\n" << RESET;
        cout << "Note: System will automatically find alternate routes when possible\n";
    }

    void manageVolunteers() {
        int choice;
        do {
            clearScreen();
            printCenteredTitle("VOLUNTEER MANAGEMENT");
            printBorder();
            
            cout << BOLD << "1. Register New Volunteer\n" << RESET;
            cout << BOLD << "2. View All Volunteers\n" << RESET;
            cout << BOLD << "3. Find Volunteers by Location\n" << RESET;
            cout << BOLD << "4. Update Volunteer Availability\n" << RESET;
            cout << BOLD << "5. Back\n" << RESET;
            cout << BOLD << "\nSelect an option: " << RESET;
            cin >> choice;
            clearScreen();
            
            switch (choice) {
                case 1: {
                    string name, phone, email, skills, location;
                    cin.ignore();
                    cout << BOLD << "Enter volunteer name: " << RESET;
                    getline(cin, name);
                    cout << BOLD << "Enter phone number: " << RESET;
                    getline(cin, phone);
                    cout << BOLD << "Enter email: " << RESET;
                    getline(cin, email);
                    cout << BOLD << "Enter skills (comma separated): " << RESET;
                    getline(cin, skills);
                    cout << BOLD << "Enter location: " << RESET;
                    getline(cin, location);
                    
                    volunteerSystem.addVolunteer(Volunteer(name, phone, email, skills, location));
                    break;
                }
                case 2:
                    volunteerSystem.displayVolunteers();
                    break;
                case 3: {
                    string location, skill;
                    cin.ignore();
                    cout << BOLD << "Enter location: " << RESET;
                    getline(cin, location);
                    cout << BOLD << "Enter skill to filter (leave blank for all): " << RESET;
                    getline(cin, skill);
                    
                    auto volunteers = volunteerSystem.findAvailableVolunteers(location, skill);
                    if (volunteers.empty()) {
                        cout << YELLOW << "No available volunteers found.\n" << RESET;
                    } else {
                        cout << CYAN << "\nAvailable Volunteers in " << location << ":\n" << RESET;
                        for (auto volunteer : volunteers) {
                            cout << "- " << volunteer->name << " (" << volunteer->skills << ")\n";
                            cout << "  Phone: " << volunteer->phone << ", Email: " << volunteer->email << "\n\n";
                        }
                    }
                    break;
                }
                case 4: {
                    string email;
                    char availability;
                    cout << BOLD << "Enter volunteer email: " << RESET;
                    cin >> email;
                    cout << BOLD << "Set availability (1 for available, 0 for unavailable): " << RESET;
                    cin >> availability;
                    volunteerSystem.updateVolunteerAvailability(email, availability == '1');
                    break;
                }
                case 5:
                    cout << YELLOW << "Returning to main menu...\n" << RESET;
                    break;
                default:
                    cout << RED << "Invalid choice.\n" << RESET;
            }
            if (choice != 5) {
                system("pause");
            }
        } while (choice != 5);
    }

    void manageAlerts() {
        int choice;
        do {
            clearScreen();
            printCenteredTitle("DISASTER ALERT SYSTEM");
            printBorder();
            
            cout << BOLD << "1. View Pending Alerts\n" << RESET;
            cout << BOLD << "2. Simulate Random Alert\n" << RESET;
            cout << BOLD << "3. Subscribe to Alerts\n" << RESET;
            cout << BOLD << "4. Unsubscribe from Alerts\n" << RESET;
            cout << BOLD << "5. Back\n" << RESET;
            cout << BOLD << "\nSelect an option: " << RESET;
            cin >> choice;
            clearScreen();
            
            switch (choice) {
                case 1:
                    alertSystem.processAlerts();
                    break;
                case 2:
                    alertSystem.simulateRandomAlert();
                    break;
                case 3: {
                    string email;
                    vector<string> disasterTypes;
                    string type;
                    cout << BOLD << "Enter your email: " << RESET;
                    cin >> email;
                    cout << BOLD << "Enter disaster types to subscribe to (comma separated): " << RESET;
                    cin.ignore();
                    getline(cin, type);
                    
                    istringstream iss(type);
                    string token;
                    while (getline(iss, token, ',')) {
                        disasterTypes.push_back(token);
                    }
                    
                    alertSystem.subscribe(email, disasterTypes);
                    break;
                }
                case 4: {
                    string email;
                    cout << BOLD << "Enter your email to unsubscribe: " << RESET;
                    cin >> email;
                    alertSystem.unsubscribe(email);
                    break;
                }
                case 5:
                    cout << YELLOW << "Returning to main menu...\n" << RESET;
                    break;
                default:
                    cout << RED << "Invalid choice.\n" << RESET;
            }
            if (choice != 5) {
                system("pause");
            }
        } while (choice != 5);
    }

    void rescueRequestForm() {
        clearScreen();
        cout << BOLD << BLUE;
        printCenteredTitle("RESCUE REQUEST FORM");
        printBorder();
        cout << RESET;
        
        for (int i = 0; i < 3; ++i) {
            Sleep(500);
            cout << ".";
            cout.flush();
        }
        clearScreen();

        Request req;
        cout << BOLD << "\nWhat's your name?: " << RESET;
        cin >> req.name;
        cout << BOLD << "\nPlease provide your contact number: " << RESET;
        cin >> req.phone;
        if(req.phone.length() != 10 || !all_of(req.phone.begin(), req.phone.end(), ::isdigit)) {
            cout << RED << "Invalid phone number. Please enter a 10-digit number.\n" << RESET;
            Sleep(1000);
            rescueRequestForm();
        }
        
        
        cin.ignore();

        cout << BOLD << "\nWhich city are you reporting from?: " << RESET;
        getline(cin, req.city);
        addCityToNetwork(req.city); // Add city to network if not already present

        cout << BOLD << "\nIn which province are you located?: " << RESET;
        getline(cin, req.province);
        cout << BOLD << "\nWhat type of disaster are you reporting (e.g., Fire, Earthquake, Flood, other)?: " << RESET;
        getline(cin, req.disasterType);
        cout << BOLD << "\nHow severe is the disaster? (low/medium/high): " << RESET;
        getline(cin, req.severity);
        
        // Set priority based on severity
        if (req.severity == "high") req.priority = 3;
        else if (req.severity == "medium") req.priority = 2;
        else req.priority = 1;

        cout << BOLD << "\nPlease specify the exact location of the disaster: " << RESET;
        getline(cin, req.location);
        cout << BOLD << "\nEstimated population affected: " << RESET;
        cin >> req.populationAffected;
        cout << BOLD << "\nEstimated food needed (units): " << RESET;
        cin >> req.foodNeeded;
        cout << BOLD << "\nEstimated water needed (liters): " << RESET;
        cin >> req.waterNeeded;
        cout << BOLD << "\nEstimated medical supplies needed (units): " << RESET;
        cin >> req.medNeeded;
        cin.ignore();
        cout << BOLD << "\nIs there anything else we should know? (Add any additional comments): " << RESET;
        getline(cin, req.comments);

        cout << GREEN << "\n\nYour rescue request has been created successfully!\n\n" << RESET;
        system("pause");
        clearScreen();
        
        cout << BOLD << BLUE;
        printCenteredTitle("RESCUE REQUEST SUMMARY");
        printBorder();
        cout << RESET;
        
        cout << BOLD << "Name: " << RESET << req.name << endl << endl;
        cout << BOLD << "Contact Number: " << RESET << req.phone << endl << endl;
        cout << BOLD << "City: " << RESET << req.city << endl << endl;
        cout << BOLD << "Province: " << RESET << req.province << endl << endl;
        cout << BOLD << "Disaster Type: " << RESET << req.disasterType << endl << endl;
        cout << BOLD << "Severity: " << RESET << req.severity << " (Priority: " << req.priority << ")" << endl << endl;
        cout << BOLD << "Location: " << RESET << req.location << endl << endl;
        cout << BOLD << "Population Affected: " << RESET << req.populationAffected << endl << endl;
        cout << BOLD << "Resources Needed: " << RESET << "Food=" << req.foodNeeded << ", Water=" << req.waterNeeded 
             << ", Medical=" << req.medNeeded << endl << endl;
        cout << BOLD << "Additional Comments: " << RESET << req.comments << endl << endl;

        cout << BOLD << "Do you want to submit or make changes? (submit/change): " << RESET;
        string action;
        cin >> action;
        if (action == "submit") {
            saveRequestToFile(req);
            addRescueRequest(req);
            cout << GREEN << "Request submitted successfully!\n" << RESET;
        }
        else if (action == "change") {
            cout << YELLOW << "Making changes...\n" << RESET;
            rescueRequestForm();
        }
    }

    void adminMainMenu() {
        clearScreen();
        printCenteredTitle("ADMIN MAIN MENU");
        printBorder();
        
        int choice;
        do {
            cout << BOLD << "\n1. Add Supplies\n" << RESET;
            cout << BOLD << "2. View Supplies\n" << RESET;
            cout << BOLD << "3. View Rescue Requests\n" << RESET;
            cout << BOLD << "4. Dispatch Supplies\n" << RESET;
            cout << BOLD << "5. Optimize Dispatch\n" << RESET;
            cout << BOLD << "6. View City Network\n" << RESET;
            cout << BOLD << "7. Add Road Block\n" << RESET;
            cout << BOLD << "8. Remove Road Block\n" << RESET;
            cout << BOLD << "9. View Road Blocks\n" << RESET;
            cout << BOLD << "10. Set Traffic Conditions\n" << RESET;
            cout << BOLD << "11. Manage Volunteers\n" << RESET;
            cout << BOLD << "12. Manage Alerts\n" << RESET;
            cout << BOLD << "13. Generate Resource Heatmap\n" << RESET;
            cout << BOLD << "14. Generate Evacuation Routes\n" << RESET;
            cout << BOLD << "15. Logout\n" << RESET;
            cout << BOLD << "\nSelect an option: " << RESET;
            cin >> choice;
            clearScreen();
            
            switch (choice) {
            case 1: {
                string item;
                int quantity;
                cout << BOLD << "\nEnter supply name: " << RESET;
                cin >> item;
                cout << BOLD << "\nEnter quantity: " << RESET;
                cin >> quantity;
                addSupply(item, quantity);
                system("pause");
                clearScreen();
                break;
            }
            case 2:
                displaySupplies();
                system("pause");
                clearScreen();
                break;
            case 3:
                displayRequests();
                system("pause");
                clearScreen();
                break;
            case 4:
                dispatchSupplies();
                system("pause");
                clearScreen();
                break;
            case 5:
                optimizeDispatch();
                system("pause");
                clearScreen();
                break;
            case 6:
                displayCityNetwork();
                system("pause");
                clearScreen();
                break;
            case 7:
                addRoadBlock();
                system("pause");
                clearScreen();
                break;
            case 8:
                removeRoadBlock();
                system("pause");
                clearScreen();
                break;
            case 9:
                viewRoadBlocks();
                system("pause");
                clearScreen();
                break;
            case 10:
                setTrafficCondition();
                system("pause");
                clearScreen();
                break;
            case 11:
                manageVolunteers();
                clearScreen();
                break;
            case 12:
                manageAlerts();
                clearScreen();
                break;
            case 13:
                generateResourceHeatmap();
                system("pause");
                clearScreen();
                break;
            case 14: {
                string city, disasterType;
                cout << BOLD << "Enter city: " << RESET;
                cin >> city;
                cout << BOLD << "Enter disaster type: " << RESET;
                cin >> disasterType;
                generateEvacuationRoutes(city, disasterType);
                system("pause");
                clearScreen();
                break;
            }
            case 15:
                cout << GREEN << "Logged out.\n" << RESET;
                break;
            default:
                cout << RED << "Invalid choice.\n" << RESET;
            }
        } while (choice != 15);
    }

    void displayCityNetwork() {
        cout << CYAN << "--- City Network Connections ---\n" << RESET;
        cout << BOLD << "Cities in network:\n" << RESET;
        for (const auto& city : cityIndices) {
            cout << "- " << city.first << " (Index: " << city.second << ")\n";
        }
        
        // Display blocked roads
        vector<pair<int, int>> blocked = cityNetwork->getBlockedEdges();
        if (!blocked.empty()) {
            cout << CYAN << "\n--- Blocked Roads ---\n" << RESET;
            for (const auto& edge : blocked) {
                cout << RED << cityNetwork->getCityName(edge.first) << " <-> " 
                     << cityNetwork->getCityName(edge.second) << " (BLOCKED)\n" << RESET;
            }
        }
        
        cout << CYAN << "\nSample shortest paths from central hub:\n" << RESET;
        for (const auto& city : cityIndices) {
            if (city.second == 0) continue; // Skip central hub
            
            vector<int> path = cityNetwork->getShortestPath(0, city.second);
            if (!path.empty()) {
                cout << BOLD << "To " << RESET << city.first << ": ";
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << cityNetwork->getCityName(path[i]);
                    if (i != path.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << " (Distance: " << cityNetwork->dijkstra(0)[city.second] << " km)\n";
            }
        }
    }

    void welcomePage() {
        clearScreen();
        cout << BLINK << BOLD << GREEN;
        printCenteredTitle("DISASTER RELIEF & RESOURCE ALLOCATION SYSTEM");
        printBorder(100, '=');
        cout << RESET;
        
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        cout << BOLD << BLUE;
        printCenteredTitle("HELPING COMMUNITIES IN CRISIS");
        cout << RESET;
        
        Sleep(3000);
        cout << BLINK << BOLD << RED;
        printCenteredTitle("DISCOVER MORE");
        cout << RESET;
        
        system("pause");
        clearScreen();
    }

    void showHome() {
        clearScreen();
        cout << BOLD << BLUE;
        printCenteredTitle("HOME PAGE");
        printBorder();
        cout << RESET;
        
        for (int i = 0; i < 3; ++i) {
            Sleep(500);
            cout << ".";
            cout.flush();
        }
        clearScreen();
        
        cout << BOLD << BLUE;
        printCenteredTitle("DISASTER RELIEF SYSTEM");
        printBorder();
        cout << RESET;
        
        cout << "A disaster relief system is a coordinated response to a disaster that provides immediate assistance and early recovery efforts to those affected:\n\n";
        Sleep(1000);
        
        cout << BOLD << CYAN;
        printCenteredTitle("GOALS");
        printBorder(50, '-');
        cout << RESET;
        
        cout << "The goals of disaster relief are to:\n";
        cout << "1. Save lives\n";
        cout << "2. Reduce health impacts\n";
        cout << "3. Ensure public safety\n";
        cout << "4. Meet basic needs\n\n";
        Sleep(1000);
        
        cout << BOLD << CYAN;
        printCenteredTitle("ACTIVITIES");
        printBorder(50, '-');
        cout << RESET;
        
        cout << "Disaster relief activities include:\n";
        cout << "1. Damage and needs assessments\n";
        cout << "2. Providing food, water, clothes, and shelter\n\n";
        Sleep(1000);
        
        cout << BOLD << CYAN;
        printCenteredTitle("SEARCH AND RESCUE OPERATIONS");
        printBorder(50, '-');
        cout << RESET;
        
        cout << "1. Providing psychosocial support\n";
        cout << "2. Facilitating community development\n";
        cout << "3. Restoring essential services like water and power\n\n";
        Sleep(1000);
        
        cout << BOLD << CYAN;
        printCenteredTitle("COORDINATION");
        printBorder(50, '-');
        cout << RESET;
        
        cout << "\nA coordinated multi-agency response is vital to reduce the impact of a disaster.\n\n";
        Sleep(1000);
        
        cout << BOLD << CYAN;
        printCenteredTitle("ROLES");
        printBorder(50, '-');
        cout << RESET;
        
        cout << "Social workers play a crucial role in disaster relief.\n\n";
        Sleep(1000);
        
        cout << "Some examples of disaster relief systems include:\n";
        Sleep(1000);
        
        cout << BOLD << GREEN << "\nNATIONAL DISASTER MANAGEMENT AUTHORITY\n" << RESET;
        Sleep(1000);
        
        cout << BOLD << GREEN << "\nNATIONAL DISASTER RESPONSE FORCE\n" << RESET;
        Sleep(1000);
        
        cout << BOLD << GREEN << "\nNATIONAL DISASTER ALERT PORTAL\n" << RESET;
        system("pause");
    }

    class Queue {
    private:
        struct Node {
            string data;
            Node* next;
            Node(string val) : data(val), next(nullptr) {}
        };
        Node* front;
        Node* rear;

    public:
        Queue() : front(nullptr), rear(nullptr) {}

        void enqueue(string val) {
            Node* newNode = new Node(val);
            if (rear == nullptr) {
                front = rear = newNode;
                return;
            }
            rear->next = newNode;
            rear = newNode;
        }

        string dequeue() {
            if (isEmpty()) {
                throw runtime_error("Queue is empty. Cannot dequeue.");
            }
            Node* temp = front;
            string data = temp->data;
            front = front->next;
            if (front == nullptr) {
                rear = nullptr;
            }
            delete temp;
            return data;
        }

        bool isEmpty() const {
            return front == nullptr;
        }

        ~Queue() {
            while (!isEmpty()) {
                dequeue();
            }
        }
    };

    void showPrecautions() {
        Queue precautionsQueue;

        int choice;

        do {
            clearScreen();
            cout << BOLD << BLUE;
            printCenteredTitle("PRECAUTIONS");
            printBorder();
            cout << RESET;
            
            int index = 1;

            Queue tempQueue = precautionsQueue;
            while (!tempQueue.isEmpty()) {
                cout << " " << index++ << ". " << tempQueue.dequeue() << "\n";
            }
            
            cout << BOLD << "\n1. Fire Precautions\n" << RESET;
            cout << BOLD << "2. Earthquake Precautions\n" << RESET;
            cout << BOLD << "3. Tornado Precautions\n" << RESET;
            cout << BOLD << "4. Flood Precautions\n" << RESET;
            cout << BOLD << "5. Back\n" << RESET;
            cout << BOLD << "\nSelect an option: " << RESET;
            cin >> choice;
            clearScreen();

            switch (choice) {
            case 1:
                cout << BOLD << BLUE;
                printCenteredTitle("FIRE PRECAUTIONS");
                printBorder();
                cout << RESET;
                
                cout << BOLD << "\nDO'S DURING THE FIRE\n" << RESET;
                cout << "-> Know your building's evacuation plan.\n";
                cout << "-> Evacuate calmly and quickly whenever a fire alarm sounds.\n";
                cout << "-> Keep important items such as medications and medical equipment handy.\n";
                cout << "-> Know two ways out of any building.\n";
                cout << "-> Before opening a door, feel it with the back of your hand.\n";
                cout << "-> If you encounter smoke during your evacuation, stay low to the floor.\n";
                
                cout << BOLD << "\nDONT'S DURING THE FIRE\n" << RESET;
                cout << "-> Use candles, incense, barbecue grills or other open flames unattended.\n";
                cout << "-> Use halogen lamps near curtains or other combustibles.\n";
                cout << "-> Hang tapestries from walls or ceilings.\n";
                cout << "-> Leave cooking appliances unattended.\n";
                cout << "-> Smoke in building.\n";
                system("pause");
                clearScreen();
                break;

            case 2:
                cout << BOLD << BLUE;
                printCenteredTitle("EARTHQUAKE PRECAUTIONS");
                printBorder();
                cout << RESET;
                
                cout << BOLD << "\nBEFORE AN EARTHQUAKE\n" << RESET;
                cout << "-> Follow and advocate local safe building codes.\n";
                cout << "-> Make a plan and be ready for an emergency.\n";
                cout << "-> Identify the medical centers, fire fighting stations.\n";
                
                cout << BOLD << "\nDURING AN EARTHQUAKE\n" << RESET;
                cout << "-> Keep calm and reassure others.\n";
                cout << "-> During the event, the safest place is an open space.\n";
                cout << "-> If you are indoors, take cover under a desk or table.\n";
                
                cout << BOLD << "\nAFTER AN EARTHQUAKE\n" << RESET;
                cout << "-> Keep stock of drinking water, foodstuff and first-aid equipment.\n";
                cout << "-> Do not spread and believe rumors.\n";
                cout << "-> Turn on your transistor or television for latest information.\n";
                system("pause");
                clearScreen();
                break;

            case 3:
                cout << BOLD << BLUE;
                printCenteredTitle("TORNADO PRECAUTIONS");
                printBorder();
                cout << RESET;
                
                cout << BOLD << "\nDO'S DURING TORNADO\n" << RESET;
                cout << "-> Go to a per-designated shelter area.\n";
                cout << "-> Listen to a Weather Radio for updates.\n";
                cout << "-> Crouch as low as possible to the floor, facing down.\n";
                
                cout << BOLD << "\nDONT'S DURING TORNADO\n" << RESET;
                cout << "-> Don't use your car as shelter.\n";
                cout << "-> Don't open the windows in your home.\n";
                cout << "-> Don't use elevators.\n";
                system("pause");
                clearScreen();
                break;

            case 4:
                cout << BOLD << BLUE;
                printCenteredTitle("FLOOD PRECAUTIONS");
                printBorder();
                cout << RESET;
                
                cout << BOLD << "\nDO'S DURING FLOOD\n" << RESET;
                cout << "-> Raise furniture and electrical appliances.\n";
                cout << "-> Put sandbags in the toilet bowl and cover drain holes.\n";
                cout << "-> Keep emergency lights, phones, and inverters charged.\n";
                
                cout << BOLD << "\nDONT'S DURING FLOOD\n" << RESET;
                cout << "-> Entering floodwaters.\n";
                cout << "-> Walking through moving water.\n";
                cout << "-> Getting near electric poles or fallen power lines.\n";
                system("pause");
                clearScreen();
                break;
            case 5:
                cout << YELLOW << "Going back...\n" << RESET;
                break;
            default:
                cout << RED << "Invalid choice, try again!\n" << RESET;
            }
        } while (choice != 5);
    }

    void toLowerCase(string& str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    struct Helpline {
        string city;
        string services[5];
    };

    void showHelplines() {
        clearScreen();
        cout << BOLD << BLUE;
        printCenteredTitle("HELPLINE SERVICES");
        printBorder();
        cout << RESET;
        
        Helpline helplines[] = {
            {"delhi", {"For Ambulance: Dial 108", "For Police: Dial 100", "For Fire Brigade: Dial 101",
                    "Relief Commissioner For Natural Calamities: Dial 1070", "Earthquake: Dial 1092"}},
            {"noida", {"For Ambulance: Dial 108", "For Police: Dial 100", "For Fire Brigade: Dial 101",
                    "Relief Commissioner For Natural Calamities: Dial 1070", "Earthquake: Dial 1092"}},
            {"lucknow", {"For Ambulance: Dial 108", "For Police: Dial 100", "For Fire Brigade: Dial 101",
                    "Relief Commissioner For Natural Calamities: Dial 1070", "Earthquake: Dial 1092"}},
            {"pune", {"For Ambulance: Dial 108", "For Police: Dial 100", "For Fire Brigade: Dial 101",
                    "Relief Commissioner For Natural Calamities: Dial 1070", "Earthquake: Dial 1092"}},
            {"bangalore", {"For Ambulance: Dial 108", "For Police: Dial 100", "For Fire Brigade: Dial 101",
                    "Relief Commissioner For Natural Calamities: Dial 1070", "Earthquake: Dial 1092"}},
        };

        const int NUM_CITIES = sizeof(helplines) / sizeof(helplines[0]);

        string city;
        cout << BOLD << "\nWHERE DO YOU NEED HELP? ENTER CITY NAME: " << RESET;
        cin >> city;
        clearScreen();
        toLowerCase(city);

        bool cityFound = false;
        for (int i = 0; i < NUM_CITIES; ++i) {
            if (helplines[i].city == city) {
                cityFound = true;
                cout << BOLD << BLUE;
                printCenteredTitle("HELPLINES FOR " + city);
                printBorder();
                cout << RESET;
                
                for (const string& line : helplines[i].services) {
                    cout << line << endl;
                    cout << "\n";
                }
                break;
            }
        }

        if (!cityFound) {
            cout << RED << "City not found. Please try again.\n" << RESET;
        }

        system("pause");
    }

    void changeLanguage() {
    clearScreen();
    printCenteredTitle("LANGUAGE SELECTION");
    printBorder();
    
    cout << BOLD << "1. English\n" << RESET;
    cout << BOLD << "2. Español (Spanish)\n" << RESET;
    cout << BOLD << "3. Français (French)\n" << RESET;
    cout << BOLD << "4. Back\n" << RESET;
    cout << BOLD << "\nSelect an option: " << RESET;
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            currentLanguage = "en";
            cout << GREEN << "Language set to English.\n" << RESET;
            break;
        case 2:
            currentLanguage = "es";
            // Set console to UTF-8 for Spanish characters
            SetConsoleOutputCP(65001);
            cout << GREEN << "Idioma cambiado a Español.\n" << RESET;
            break;
        case 3:
            currentLanguage = "fr";
            // Set console to UTF-8 for French characters
            SetConsoleOutputCP(65001);
            cout << GREEN << "Langue changée en Français.\n" << RESET;
            break;
        case 4:
            return;
        default:
            cout << RED << "Invalid choice.\n" << RESET;
    }
    system("pause");
}

    void loginSignupMenu(RescueManagementSystem& rms, FeedbackSystem& fds) {
        clearScreen();
        cout << BOLD << BLUE;
        printCenteredTitle("LOGIN/SIGNUP MENU");
        printBorder();
        cout << RESET;
        
        bool isLoggedIn = false;
        int choice;
        do {
            clearScreen();
            cout << BOLD << "1. User Login\n" << RESET;
            cout << BOLD << "2. User Signup\n" << RESET;
            cout << BOLD << "3. Admin Login\n" << RESET;
            cout << BOLD << "4. Back\n" << RESET;
            cout << BOLD << "\nSelect an option: " << RESET;
            cin >> choice;
            clearScreen();
            
            switch (choice) {
            case 1:
                if (rms.userLogin()) {
                    cout << GREEN << "User login successful!\n" << RESET;
                    isLoggedIn = true;
                    cout << "Congratulations! You can now fill up the rescue request form.\n";
                    rms.rescueRequestForm();
                }
                else {
                    cout << RED << "Invalid credentials. Please try again.\n" << RESET;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;
            case 2:
                rms.userSignup();
                Sleep(1000);
                clearScreen();
                if (rms.userLogin()) {
                    cout << GREEN << "User login successful after signup!\n" << RESET;
                    isLoggedIn = true;
                    rms.rescueRequestForm();
                }
                else {
                    cout << YELLOW << "Signup complete. Please try logging in.\n" << RESET;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;
            case 3:
                if (rms.adminLogin()) {
                    cout << GREEN << "\nADMIN LOGIN SUCCESSFUL!\n" << RESET;
                    isLoggedIn = true;
                    system("pause");
                    clearScreen();
                    rms.adminMainMenu();
                }
                else {
                    cout << RED << "Invalid admin credentials. Please try again.\n" << RESET;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;
            case 4:
                cout << YELLOW << "Going back...\n" << RESET;
                break;
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!isLoggedIn && choice != 4);
    }

    void mainMenu(RescueManagementSystem& rms, FeedbackSystem& fds) {
        int choice;
        do {
            clearScreen();
            cout << BOLD << BLUE;
            printCenteredTitle(translate("MAIN MENU"));
            printBorder();
            cout << RESET;
            
            cout << BOLD << "1. " << translate("home") << "\n" << RESET;
        cout << BOLD << "2. " << translate("precautions") << "\n" << RESET;
        cout << BOLD << "3. " << translate("helpline") << "\n" << RESET;
        cout << BOLD << "4. " << translate("request") << "\n" << RESET;
        cout << BOLD << "5. " << translate("login") << "\n" << RESET;
        cout << BOLD << "6. " << translate("feedback") << "\n" << RESET;
        cout << BOLD << "7. Change Language\n" << RESET; // This is a special case
        cout << BOLD << "8. " << translate("exit") << "\n" << RESET;
        cout << BOLD << "\n" << translate("select_option") << RESET;
        cin >> choice;
            clearScreen();
            
            switch (choice) {
            case 1:
                showHome();
                break;
            case 2:
                showPrecautions();
                break;
            case 3:
                showHelplines();
                break;
            case 4:
                rescueRequestForm();
                break;
            case 5:
                loginSignupMenu(rms, fds);
                break;
            case 6: {
                int feedbackChoice;
                cout << BOLD << "1. Submit Feedback\n" << RESET;
                cout << BOLD << "2. View Feedback\n" << RESET;
                cout << BOLD << "\nSelect an option: " << RESET;
                cin >> feedbackChoice;

                if (feedbackChoice == 1) {
                    bool goBackToMenu = fds.collectFeedback();
                    if (goBackToMenu) {
                        break;
                    }
                }
                else if (feedbackChoice == 2) {
                    fds.displayFeedback();
                    system("pause");
                }
                else {
                    cout << RED << "Invalid feedback choice.\n" << RESET;
                }
                break;
            }
            case 7:
                changeLanguage();
                break;
            case 8:
                clearScreen();
                cout << BLINK << BOLD << GREEN;
                printCenteredTitle("THANK YOU FOR USING DISASTER RELIEF SYSTEM");
                printBorder(100, '=');
                cout << RESET;
                cout << BOLD << "\nSTAY SAFE! STAY INFORMED!\n\n\n\n\n\n\n" << RESET;
                break;
            default:
                cout << RED << "Invalid choice. Please try again.\n" << RESET;
            }
        } while (choice != 8);
    }
};

int main() {
    // Set console output to support UTF-8 for multi-language support
    SetConsoleOutputCP(65001);
    
    FeedbackSystem fds;
    RescueManagementSystem rms;
    rms.welcomePage();
    Sleep(1000);
    rms.showHome();
    clearScreen();
    rms.mainMenu(rms, fds);
    return 0;
}
