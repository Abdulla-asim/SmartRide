
class Edge;

class Node {
public:
    int id;                   // Unique identifier for the node
    float x, y;               // Coordinates of the node
    string name;              // Name of the node
    vector<Node*> neighbors;  // List of adjacent nodes
    vector<Edge*> edges;      // List of edges for the node (using pointers)

    // Constructor
    Node(int nodeId, float xCoord, float yCoord, string nodeName)
        : id(nodeId), x(xCoord), y(yCoord), name(nodeName) {}

    // Method to add a neighbor and create the edge
    void addNeighbor(Node* neighbor);

    // Display node details
    void display()
    {
        cout << "Node " << id << " (" << name << ", x: " << x << ", y: " << y << ") is connected to: ";
        for (Node* neighbor : neighbors)
        {
            cout << neighbor->id << " (" << neighbor->name << ") ";
        }
        cout << endl;
    }

    virtual string toString()
    {
        return "node";
    }

    virtual int getType()
    {
        return 0;
    }
};

class TrafficIntersection : public Node {
public:
    int type;             // Intersection type (4-way, 3-way)
    vector<bool> signals; // Array of traffic signals for each direction
    int lightRecord4 = 0;
    int lightRecord3 = 0;

    // Constructor
    TrafficIntersection(int nodeId, float xCoord, float yCoord, string nodeName, int intersectionType)
        : Node(nodeId, xCoord, yCoord, nodeName), type(intersectionType)
    {
        initializeSignals(intersectionType);
    }

    // Method to display intersection details
    void displayIntersection()
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

    void initializeSignals(int intersectionType)
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

    void changeLights()
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

    int getType() override
    {
        return type;
    }
};

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
    Edge(Node* n1, Node* n2, float road_width) : node1(n1), node2(n2), width(road_width), no_of_agents(0)
    {
        // Calculate the length using the Euclidean distance formula
        length = sqrt(pow(n1->x - n2->x, 2) + pow(n1->y - n2->y, 2));

        // Calculate the maximum allowed traffic based on width and length
        float density_factor = 0.123f; // Example density: 10 agents per unit area
        max_traffic = static_cast<int>(width * length * density_factor);
    }

    // Constructor with only two nodes (everything else defaults to 0 or flag values)
    Edge(Node* n1, Node* n2) : node1(n1), node2(n2), length(0), no_of_agents(0), width(0), max_traffic(0) {}
};