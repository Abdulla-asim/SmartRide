#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

struct Edge {
    int to;          // Neighbour node
    double weight;   // Weight (e.g., distance or travel time)
};

class CityGraph {
public:
    vector<string> nodes;                       // List of nodes
    unordered_map<string, int> nodeToIndex;     // For Mapping from intersection names(A,B,C) to indices

    vector<vector<Edge>> graph;                 // Adjacency list

    CityGraph() {}

    // Add a new intersection (node) to the graph
    void addIntersection(const string& name) {
        if (nodeToIndex.find(name) == nodeToIndex.end()) {
            nodeToIndex[name] = nodes.size();
            nodes.push_back(name);
            graph.emplace_back(); // Add an empty list for this node
        }
    }

    // Add a road (edge) between two nodes
    void addWeight(const string& from, const string& to, double weight) {
        if (nodeToIndex.find(from) == nodeToIndex.end() ||
            nodeToIndex.find(to) == nodeToIndex.end()) {
            cerr << "Error: One or both nodes do not exist.\n";
            cout << "from: " << from << " to: " << to << endl;
            return;
        }

        int fromIndex = nodeToIndex[from];
        int toIndex = nodeToIndex[to];

        graph[fromIndex].push_back({toIndex, weight});
        graph[toIndex].push_back({fromIndex, weight}); // Assuming undirected graph
    }

    // Find shortest path using Dijkstra's algorithm
    vector<string> findShortestPath(const string& start, const string& destination) {
        if (nodeToIndex.find(start) == nodeToIndex.end() ||
            nodeToIndex.find(destination) == nodeToIndex.end()) {
            cerr << "Error: One or both nodes do not exist.\n";
            return {};
        }

        int startIndex = nodeToIndex[start];
        int destIndex = nodeToIndex[destination];

        int n = nodes.size();
        vector<double> distances(n, numeric_limits<double>::infinity());
        vector<int> previous(n, -1); // To reconstruct the path
        distances[startIndex] = 0;

        using Node = pair<double, int>; // {distance, node index}
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        pq.push({0, startIndex});

        while (!pq.empty()) {
            auto [dist, current] = pq.top();
            pq.pop();

            if (dist > distances[current]) continue;

            for (const Edge& edge : graph[current]) {
                double newDist = dist + edge.weight;
                if (newDist < distances[edge.to]) {
                    distances[edge.to] = newDist;
                    previous[edge.to] = current;
                    pq.push({newDist, edge.to});
                }
            }
        }

        // Reconstruct the path
        vector<string> path;
        for (int at = destIndex; at != -1; at = previous[at]) {
            path.push_back(nodes[at]);
        }
        reverse(path.begin(), path.end());

        if (path.size() == 1 && path[0] != start) {
            cerr << "Error: No path exists between the nodes.\n";
            return {};
        }

        return path;
    }

    // Display the graph
    void displayGraph() const {
        for (size_t i = 0; i < graph.size(); ++i) {
            cout << nodes[i] << ": ";
            for (const Edge& edge : graph[i]) {
                cout << "(" << nodes[edge.to] << ", " << edge.weight << ") ";
            }
            cout << endl;
        }
    }
};


int main() {
    CityGraph city;

    // Add nodes(cities/intersections)
    city.addIntersection("JamshedTown");
    city.addIntersection("GardenEast");
    city.addIntersection("MemonSociety");
    city.addIntersection("FederalBArea");
    city.addIntersection("LalKothi");
    city.addIntersection("Gulshan-e-Iqbal");
    city.addIntersection("Kharadar");
    city.addIntersection("MohammadAliSociety");
    city.addIntersection("Dhoraji");
    city.addIntersection("FaisalBase");
    city.addIntersection("Clifton");
    city.addIntersection("Saddar");
    city.addIntersection("Defence");
    city.addIntersection("Gulshan");
    city.addIntersection("Korangi");
    city.addIntersection("Malir");
    city.addIntersection("ShahFaisal");
    city.addIntersection("Nazimabad");
    city.addIntersection("Liaquatabad");
    city.addIntersection("Orangi");
    city.addIntersection("NorthNazimabad");
    city.addIntersection("Kemari");
    city.addIntersection("Garden");
    city.addIntersection("UniversityRoad");

    // Add roads(edges) with hardcoded weights
    city.addWeight("JamshedTown", "NorthNazimabad", 3.0);
    city.addWeight("JamshedTown", "Defence", 2.0);
    city.addWeight("Saddar", "NorthNazimabad", 4.0);
    city.addWeight("Saddar", "GardenEast", 1.0);
    city.addWeight("Saddar", "MemonSociety", 3.0);
    city.addWeight("Saddar", "FederalBArea", 2.0);
    city.addWeight("Clifton", "Saddar", 1.0);
    city.addWeight("FederalBArea", "Liaquatabad", 4.0);
    city.addWeight("Defence", "MemonSociety", 3.0);
    city.addWeight("Defence", "Orangi", 2.0);
    city.addWeight("Orangi", "MemonSociety", 4.0);
    city.addWeight("Orangi", "Korangi", 1.0);
    city.addWeight("Kemari", "Gulshan-e-Iqbal", 2.0);
    city.addWeight("Garden", "LalKothi", 3.0);
    city.addWeight("Kemari", "Garden", 1.0);
    city.addWeight("Garden", "Clifton", 4.0);
    city.addWeight("Clifton", "LalKothi", 2.0);
    city.addWeight("Clifton", "MohammadAliSociety", 3.0);
    city.addWeight("FaisalBase", "Liaquatabad", 1.0);
    city.addWeight("Clifton", "FaisalBase", 4.0);
    city.addWeight("Gulshan", "Gulshan-e-Iqbal", 2.0);
    city.addWeight("Gulshan", "Kharadar", 3.0);
    city.addWeight("Gulshan", "UniversityRoad", 1.0);
    city.addWeight("Gulshan", "MohammadAliSociety", 4.0);
    city.addWeight("Liaquatabad", "Dhoraji", 3.0);
    city.addWeight("UniversityRoad", "ShahFaisal", 2.0);
    city.addWeight("UniversityRoad", "Malir", 1.0);
    city.addWeight("Dhoraji", "Nazimabad", 4.0);
    city.addWeight("Nazimabad", "ShahFaisal", 2.0);
    city.addWeight("Nazimabad", "Korangi", 3.0);
    city.addWeight("ShahFaisal", "Malir", 1.0);
    city.addWeight("Malir", "Korangi", 4.0);

    // Display the graph
    cout << "City Graph:\n";
    city.displayGraph();
    
    int vehicle;
    std::cout << "Welcome to the Traffic Congestion Control System\n";

    while (true) {
        std::cout << "Please select the vehicle you are using:\n";
        std::cout << "1. Rickshaw\n";
        std::cout << "2. Car\n";
        std::cout << "3. Bike\n";
        std::cout << "4. Truck\n";
        std::cout << "5. Other\n";
        std::cout << "Enter -1 to exit the program.\n";

        std::cout << "Enter the number corresponding to your vehicle: ";
        std::cin >> vehicle;

        if (vehicle == -1) {
            std::cout << "Exiting the program...\n";
            return 0;
            
        }

        if (vehicle >= 1 && vehicle <= 5) {
            switch (vehicle) {
            case 1:
                std::cout << "You selected Rickshaw.\n";
                break;
            case 2:
                std::cout << "You selected Car.\n";
                break;
            case 3:
                std::cout << "You selected Bike.\n";
                break;
            case 4:
                std::cout << "You selected Truck.\n";
                break;
            case 5:
                std::cout << "You selected Other vehicle.\n";
                break;
            }
            break;
        }
        else {
            std::cout << "Invalid choice, please select a valid option.\n";
        }
    }

    const char* locations[] = {
        "JamshedTown", "GardenEast", "MemonSociety", "FederalBArea", "LalKothi",
        "Gulshan-e-Iqbal", "Kharadar", "MohammadAliSociety", "Dhoraji", "FaisalBase",
        "Clifton", "Saddar", "Defence", "Gulshan", "Korangi",
        "Malir", "ShahFaisal", "Nazimabad", "Liaquatabad", "Orangi",
        "NorthNazimabad", "Kemari", "Garden", "UniversityRoad"
    };

    const char* vehicleTypes[] = {
    "Rickshaw", "Car", "Bike", "Truck", "Other"
    };

    int start, end;

    while (true) {
        std::cout << "Please select a starting point (1-24):\n";
        for (int i = 0; i < 24; i++) {
            std::cout << (i + 1) << ". " << locations[i] << "\n";
        }

        std::cout << "Enter the number of your starting point: ";
        std::cin >> start;

        std::cout << "Enter the number of your destination point (1-24): ";
        std::cin >> end;

        if (start < 1 || start > 24 || end < 1 || end > 24) {
            std::cout << "Invalid input, please select numbers between 1 and 24.\n";
            continue;
        }

        if (start == end) {
            std::cout << "Starting point and destination cannot be the same. Please try again.\n";
            continue;
        }

        std::cout << "You selected the starting point: " << locations[start - 1] << "\n";
        std::cout << "You selected the destination point: " << locations[end - 1] << "\n";

        break;
    }

    // Find shortest path
    auto path = city.findShortestPath(locations[start-1], locations[end-1]);
    if (!path.empty()) {
        cout << "\nShortest path from " << locations[start-1] << " to " << locations[end-1] << ":\n";
        for (const auto &node : path) {
            cout << node << " >> ";
        }
        cout << endl;
    }

    return 0;
}
