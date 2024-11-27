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
    vector<string> nodes;                               // List of nodes
    unordered_map<string, int> nodeToIndex;     // For Mapping from intersection names(A,B,C) to indices

    vector<vector<Edge>> graph;                         // Adjacency list

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

    // Add nodes
    city.addIntersection("A");
    city.addIntersection("B");
    city.addIntersection("C");
    city.addIntersection("D");

    // Add roads
    city.addWeight("A", "B", 5.0);
    city.addWeight("A", "C", 10.0);
    city.addWeight("B", "C", 3.0);
    city.addWeight("C", "D", 1.0);

    // Display the graph
    cout << "City Graph:\n";
    city.displayGraph();

    // Find shortest path
    auto path = city.findShortestPath("A", "D");
    if (!path.empty()) {
        cout << "\nShortest path from A to D:\n";
        for (const auto &node : path) {
            cout << node << " ";
        }
        cout << endl;
    }

    return 0;
}
