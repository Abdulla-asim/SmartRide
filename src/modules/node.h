#ifndef NODE_H
#define NODE_H

#include "edge.h"
#include <vector>
#include <string>

using namespace std;

class Node {
public:
    int id;                   // Unique identifier for the node
    float x, y;               // Coordinates of the node
    string name;              // Name of the node
    vector<Node*> neighbors;  // List of adjacent nodes
    vector<Edge*> edges;      // List of edges for the node (using pointers)

    // Constructor
    Node(int nodeId, float xCoord, float yCoord, string nodeName);

    // Method to add a neighbor and create the edge
    void addNeighbor(Node* neighbor);

    // Display node details
    void display();

    virtual string toString();

    virtual int getType();

    static Edge* findEdge(Node* node1, Node* node2);
    static int cost(Node* currentNode, Node* nextNode);
    static int heuristic(Node* node, Node* goal);
};

class TrafficIntersection : public Node {
public:
    int type;             // Intersection type (4-way, 3-way)
    vector<bool> signals; // Array of traffic signals for each direction
    int lightRecord4 = 0;
    int lightRecord3 = 0;

    // Constructor
    TrafficIntersection(int nodeId, float xCoord, float yCoord, string nodeName, int intersectionType);

    // Method to display intersection details
    void displayIntersection();

    void initializeSignals(int intersectionType);
    void changeLights();

    int getType() override;

};

#endif