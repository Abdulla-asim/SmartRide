#include "vehicle.h"

string Vehicle::vehicleTypes[4] = {"Car", "Truck", "Bus", "Bike"};

// Constructor
Vehicle::Vehicle(int id, string type, Node* startNode, Node* goalNode)
    : id(id), type(type), currentNode(startNode), goalNode(goalNode), currentEdge(nullptr), x(startNode->x), y(startNode->y)
{
    cout << "Called A* search on vehicle" << id << endl;
    this->path = aStar(currentNode, goalNode);
    if (this->path.empty())
    {
        cerr << "No path found for the vehicle from start to goal." << endl;
        return;
    }

    this->currentEdge = this->path.front();
    this->path.erase(this->path.begin());

    // Set the next node to reach
    if (currentEdge->node1 == currentNode)
    {
        this->currentNodeToReach = currentEdge->node2;
    }
    else
    {
        this->currentNodeToReach = currentEdge->node1;
    }

    setLengthByType();
}

// constructor with random start and goal nodes if not provided
Vehicle::Vehicle(int id, string type, vector<Node*> nodes)
    : id(id), type(type), currentNode(nodes[rand() % nodes.size()]), goalNode(nodes[rand() % nodes.size()]), currentEdge(nullptr), x(currentNode->x), y(currentNode->y)
{
    while (currentNode == goalNode)
    {
        goalNode = nodes[rand() % nodes.size()];
    }
    // Initialize the path using A* search
    this->path = aStar(currentNode, goalNode);
    if (this->path.empty())
    {
        cerr << "No path found for the vehicle from start to goal." << endl;
        return;
    }

    this->currentEdge = this->path.front();
    this->path.erase(this->path.begin());

    // Set the next node to reach
    if (currentEdge->node1 == currentNode)
    {
        this->currentNodeToReach = currentEdge->node2;
    }
    else
    {
        this->currentNodeToReach = currentEdge->node1;
    }

    setLengthByType();
}

// Set vehicle length based on type
void Vehicle::setLengthByType()
{
    static unordered_map<string, float> averageLengths = {
        {"Car", 4.5}, {"Truck", 12.0}, {"Bus", 10.5}, {"Bike", 2.0}, {"Rickshaw", 1.8}};

    if (averageLengths.find(type) != averageLengths.end())
    {
        length = averageLengths[type];
    }
    else
    {
        cerr << "Unknown vehicle type: " << type << ". Assigning default length of 4.5 meters." << endl;
        length = 4.5;
    }
}

// Move the vehicle
bool Vehicle::moveVehicle()
{
    // If the vehicle has reached its goal
    // if (this->x == goalNode->x && this->y == goalNode->y)
    if (currentNode == goalNode)
    {
        if (currentEdge)
        {
            updateEdgeAgentCount(currentEdge, -1); // Decrement agent count on the edge
        }
        
        hasReachedDestination = true;
        return true;
    }

    // If at the current node
    if (this->x == currentNode->x && this->y == currentNode->y)
    {
        if (this->path.empty())
        {
            this->path = aStar(currentNode, this->goalNode); // Recompute the path if necessary
            if (this->path.empty())
            {
                cerr << "No path found to the goal!" << endl;
                return false;
            }
        }

        this->currentEdge = this->path.front(); // Take the first edge
        this->path.erase(this->path.begin());

        // Determine the next node to reach
        if (currentEdge->node1 == currentNode)
        {
            this->currentNodeToReach = currentEdge->node2;
        }
        else
        {
            this->currentNodeToReach = currentEdge->node1;
        }

        updateEdgeAgentCount(currentEdge, 1); // Increment agent count on the new edge
        changeCoordinates();                 // Start moving toward the next node
    }
    else if (this->x == currentNodeToReach->x && this->y == currentNodeToReach->y)
    {
        // Reached the intermediate node
        currentNode = currentNodeToReach;

        // Decrement agents on the current edge
        updateEdgeAgentCount(currentEdge, -1);

        // If path is not empty, take the next edge
        if (!this->path.empty())
        {
            this->currentEdge = this->path.front();
            this->path.erase(this->path.begin());
            this->currentNodeToReach = (currentEdge->node1 == currentNode) ? currentEdge->node2 : currentEdge->node1;
        }
    }
    else
    {
        // Continue moving toward the next node
        changeCoordinates();
    }
    return false;
}

// Update the number of agents on an edge
void Vehicle::updateEdgeAgentCount(Edge* edge, int delta)
{
    if (edge)
    {
        Edge* forwardEdge = Node::findEdge(edge->node1, edge->node2);
        Edge* backwardEdge = Node::findEdge(edge->node2, edge->node1);

        if (forwardEdge)
            forwardEdge->no_of_agents += delta;

        if (backwardEdge)
            backwardEdge->no_of_agents += delta;
    }
}

// Move the vehicle's coordinates
void Vehicle::changeCoordinates()
{
    float dx = currentNodeToReach->x - this->x;
    float dy = currentNodeToReach->y - this->y;

    // Distance to the next node
    float distanceToNextNode = sqrt(dx * dx + dy * dy);

    if (distanceToNextNode <= 5.0f)
    {
        // Snap to the next node if within range
        this->x = currentNodeToReach->x;
        this->y = currentNodeToReach->y;
    }
    else
    {
        // Calculate movement direction
        float directionX = dx / distanceToNextNode;
        float directionY = dy / distanceToNextNode;

        // Move based on speed
        this->x += directionX * speed;
        this->y += directionY * speed;
    }
}

// Update destination
void Vehicle::updateDestination(Node* nextDestination)
{
    this->goalNode = nextDestination;
    this->hasReachedDestination = false;
    this->path = aStar(currentNode, nextDestination);
    if (this->path.empty())
    {
        cerr << "No path found for the vehicle" << id<< " from start to goal." << endl;
        return;
    }
}

