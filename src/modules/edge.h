#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <cmath>

using namespace std;

class Node; // Forward declaration of Node class

class Edge
{
public:
    Node* node1;      // First node of the edge
    Node* node2;      // Second node of the edge
    float length;     // Length of the edge
    int no_of_agents; // Number of agents on the edge
    float width;      // Width of the road
    int max_traffic;  // Maximum number of allowed traffic

    // Constructor with width parameter
    Edge(Node* n1, Node* n2, float road_width);

    // Constructor with only two nodes (everything else defaults to 0 or flag values)
    Edge(Node* n1, Node* n2);
};

#endif