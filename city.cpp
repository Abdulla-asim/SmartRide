#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <cmath>

using namespace std;

struct Edge {
    int to;      // Neighbor node
    double weight; // It can be Distance or traffic related
};

/*
    ////    ADJACENCY LIST:

    Graph graph = {
        {{1, 2.0}, {2, 4.0}}, // Node 0 connects to 1 having weight (2.0), 2 having weight (4.0)
        {{2, 1.0}},           // Node 1 connects to 2 having weight (1.0)
        {{3, 3.0}},           // Node 2 connects to 3 having weight (3.0)
        {}                    // But Node 3 has no outgoing edges
    };
*/

using Graph = vector<vector<Edge>>; // Adjacency list representation

vector<double> dijkstra(const Graph& graph, int start) {
    int n = graph.size();
    vector<double> distances(n, numeric_limits<double>::infinity());
    distances[start] = 0;

    using Node = pair<double, int>; // {distance, node}
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();

        if (dist > distances[current]) continue;

        for (const Edge& edge : graph[current]) {
            double newDist = dist + edge.weight;
            if (newDist < distances[edge.to]) {
                distances[edge.to] = newDist;
                pq.push({newDist, edge.to});
            }
        }
    }
    return distances;
}

int main() {
    // Example graph
    Graph graph(5);
    graph[0].push_back({1, 2.0});  // Edge 0 -> 1 with weight 2.0
    graph[1].push_back({2, 3.0});  // Edge 1 -> 2 with weight 3.0
    graph[2].push_back({3, 1.0});  // Edge 2 -> 3 with weight 1.0
    graph[3].push_back({4, 4.0});  // Edge 3 -> 4 with weight 4.0
    graph[1].push_back({4, 6.0});  // Edge 1 -> 4 with weight 6.0

    int userLocation = 0;   // Start node
    int driverLocation = 4; // Driver's node
    int destination = 3;    // Destination node

    // Step 1: Driver to User
    auto distancesFromDriver = dijkstra(graph, driverLocation);
    cout << "Driver to User: " << distancesFromDriver[userLocation] << endl;

    // Step 2: User to Destination
    auto distancesFromUser = dijkstra(graph, userLocation);
    cout << "User to Destination: " << distancesFromUser[destination] << endl;

    return 0;
}

