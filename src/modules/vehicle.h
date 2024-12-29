#ifndef VEHICLE_H
#define VEHICLE_H

#include "edge.h"
#include "node.h"
#include <unordered_map>
#include <raylib.h>

using namespace std;

vector<Edge*> aStar(Node* start, Node* mid, Node* goal);

class Vehicle
{
public:
    int id;                    // Unique ID for the vehicle
    string type;               // Vehicle type (car, truck, bus, motorcycle, etc.)
    float length;              // Length of the vehicle in meters
    Node* currentNode;         // Current node the vehicle is at
    Node* goalNode;            // Goal node for the vehicle
    Edge* currentEdge;         // Current edge the vehicle is on
    std::vector<Edge*> path;   // Planned path for the vehicle
    float x;                   // Current x-coordinate of the vehicle
    float y;                   // Current y-coordinate of the vehicle
    float speed = 0.7;         // Speed of the vehicle
    Node* currentNodeToReach;  // Node the vehicle is currently heading toward
    bool hasReachedDestination = false; // Destination reached status
    bool pickingUp = false;    // Picking up a passenger status
    Node* userGoalNode;        // Goal node for the user
    Color color = BLUE;        // Color of the vehicle

    // Vehicle types
    static string vehicleTypes[4];

    // Constructor
    Vehicle(int id, string type, Node* startNode, Node* goalNode, vector<Node*> nodes);
    Vehicle(int id, string type, vector<Node*> nodes);


    // Set vehicle length based on type
    void setLengthByType();

    // Move the vehicle
    bool moveVehicle();

    // Update the number of agents on an edge
    void updateEdgeAgentCount(Edge* edge, int delta);

    // Move the vehicle's coordinates
    void changeCoordinates();

    // Update destination
    void updateDestination(Node* nextDestination);
};

#endif