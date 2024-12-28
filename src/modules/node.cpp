#include "node.h"

// Constructor
Node::Node(int nodeId, float xCoord, float yCoord, string nodeName)
    : id(nodeId), x(xCoord), y(yCoord), name(nodeName) {}

// Method implementation for adding a neighbor and creating an edge
void Node::addNeighbor(Node* neighbor)
{
    neighbors.push_back(neighbor);
    Edge* edge = new Edge(this, neighbor, 3.2); // Create a new edge dynamically
    edges.push_back(edge);                     // Add the edge pointer to the edges list
    neighbor->neighbors.push_back(this);
    Edge* edgeReverse = new Edge(neighbor, this, 3.2);
    neighbor->edges.push_back(edgeReverse);
}

// Display node details
void Node::display()
{
    cout << "Node " << id << " (" << name << ", x: " << x << ", y: " << y << ") is connected to: ";
    for (Node* neighbor : neighbors)
    {
        cout << neighbor->id << " (" << neighbor->name << ") ";
    }
    cout << endl;
}

string Node::toString()
{
    return "node";
}

int Node::getType()
{
    return 0;
}


// Constructor
TrafficIntersection::TrafficIntersection(int nodeId, float xCoord, float yCoord, string nodeName, int intersectionType)
    : Node(nodeId, xCoord, yCoord, nodeName), type(intersectionType)
{
    initializeSignals(intersectionType);
}

// Method to display intersection details
void TrafficIntersection::displayIntersection()
{
    cout << "Traffic Intersection " << id << " (" << name << ")\n";
    cout << "Type: " << (type == 4 ? "4-way" : "3-way") << endl;
    cout << "Signals: ";
    for (bool signal : signals)
    {
        cout << (signal ? "Green" : "Red") << " ";
    }
    cout << endl;
}

void TrafficIntersection::initializeSignals(int intersectionType)
{
    if (intersectionType == 4)
    {
        signals = vector<bool>(4, false); // 4 signals for a 4-way intersection
        signals[0] = true;
    }
    else if (intersectionType == 3)
    {
        signals = vector<bool>(3, false); // 3 signals for a 3-way intersection
        signals[0] = true;
    }
    else
    {
        cerr << "Invalid intersection type. Defaulting to 4-way." << endl;
        signals = vector<bool>(4, false); // Default to 4 signals
    }
}

void TrafficIntersection::changeLights()
{
    if (type == 4)
    {
        signals[lightRecord4] = false;
        lightRecord4 = (lightRecord4 + 1) % type;
        signals[lightRecord4] = true;
    }
    if (type == 3)
    {
        signals[lightRecord3] = false;
        lightRecord3 = (lightRecord3 + 1) % type;
        signals[lightRecord3] = true;
    }
}

int TrafficIntersection::getType()
{
    return type;
}

// Cost function to find the edge and return the length
int Node::cost(Node* currentNode, Node* nextNode)
{
    // Loop through the edges of currentNode to find the one with nextNode
    for (Edge* edge : currentNode->edges)
    {
        if ((edge->node1 == nextNode) || (edge->node2 == nextNode))
        {
            float penalty = 0;
            if(edge->no_of_agents < edge->max_traffic) {
                penalty = 1 + (edge->no_of_agents / edge->max_traffic);
            }
            else {
                penalty = 1000;

            }
            int congestionFactor = 100;
            float congestionCost = congestionFactor * penalty;
            return edge->length + congestionCost; // Include number of agents in cost
        }
    }
    return INT_MAX; // Indicates no edge found
}

// Heuristic function (Euclidean distance)
int Node::heuristic(Node* node, Node* goal)
{
    float euclideanDistance = sqrt(pow(node->x - goal->x, 2) + pow(node->y - goal->y, 2));
    return static_cast<int>(euclideanDistance);
}

// Edge* findEdge(Node* node1, Node* node2);

// Find an edge between two nodes
Edge* Node::findEdge(Node* node1, Node* node2)
{
    for (Edge* edge : node1->edges)
    {
        if ((edge->node2 == node2))
        {
            return edge; // Return the pointer to the edge
        }
    }
    return nullptr; // Return nullptr if no edge found
}

