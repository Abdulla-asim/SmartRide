#include "node.h"

// Constructor with width parameter
Edge::Edge(Node* n1, Node* n2, float road_width) : node1(n1), node2(n2), width(road_width), no_of_agents(0)
{
    // Calculate the length using the Euclidean distance formula
    length = sqrt(pow(n1->x - n2->x, 2) + pow(n1->y - n2->y, 2));

    // Calculate the maximum allowed traffic based on width and length
    float density_factor = 0.123f; // Example density: 10 agents per unit area
    max_traffic = static_cast<int>(width * length * density_factor);
}

// Constructor with only two nodes (everything else defaults to 0 or flag values)
Edge::Edge(Node* n1, Node* n2) : node1(n1), node2(n2), length(0), no_of_agents(0), width(0), max_traffic(0) {}
