#include <set>
#include <queue>
#include <climits>      // For INT_MAX
#include <algorithm>    // For reverse()
#include "modules/user.h"
#include <limits>
#include <unordered_set>
#include "modules/driver.h"
#include <time.h>
#include "modules/node.h" 
#include <strings.h>
#include "modules/vehicle.h"
#include "modules/edge.h"
#include <functional>

using namespace std;

vector<Node*> arrayOfNodes;

Driver* createRandomDriver(int age, string name, string email, bool gender, string phoneNumber, string licenseNumber, int yearsOfExperience, string vehicleType);

vector<Edge*> aStar(Node* start, Node* goal) {
    if (!start || !goal) {
        cout << "Error: Invalid start or goal node!" << endl;
        return {};
    }

    using QueueElement = pair<float, Node*>; // {f_score, Node*}
    priority_queue<QueueElement, vector<QueueElement>, greater<>> openSet;

    unordered_map<Node*, float> gScore;
    unordered_map<Node*, Edge*> cameFromEdge;
    unordered_map<Node*, float> fScore;

    gScore[start] = 0;
    fScore[start] = Node::heuristic(start, goal);
    openSet.push({ fScore[start], start });

    while (!openSet.empty()) {
        Node* current = openSet.top().second;
        openSet.pop();

        if (current == goal) {
            // Goal found, reconstruct path
            vector<Edge*> path;
            while (cameFromEdge.find(current) != cameFromEdge.end()) {
                Edge* edge = cameFromEdge[current];
                path.push_back(edge);
                current = (edge->node1 == current) ? edge->node2 : edge->node1;
            }
            reverse(path.begin(), path.end());
            return path; // Return the full path as a vector of edges
        }

        // Explore neighbors
        for (size_t i = 0; i < current->neighbors.size(); ++i) {
            Node* neighbor = current->neighbors[i];
            Edge* edge = current->edges[i];
            float tentative_gScore = gScore[current] + Node::cost(current, neighbor);

            if (!gScore.count(neighbor) || tentative_gScore < gScore[neighbor]) {
                cameFromEdge[neighbor] = edge;
                gScore[neighbor] = tentative_gScore;
                float fScore = tentative_gScore + Node::heuristic(neighbor, goal);
                openSet.push({ fScore, neighbor });
            }
        }
    }

    // If we reach here, no path was found
    cout << "Error: No path found from " << start->id << " to " << goal->id << "!" << endl;
    return {}; // Return an empty path if no path exists
}



void DrawIntersectionCircles(int thickness, Color color, int X, int Y) {
    const int radius = 5;
}

bool areEdgesEqual(Edge* edge1, Edge* edge2) {
    // Check if the two edges connect the same two nodes (in any order)
    return (edge1->node1 == edge2->node1 && edge1->node2 == edge2->node2) ||
        (edge1->node1 == edge2->node2 && edge1->node2 == edge2->node1);
}

// Register and store users in json file
void registerUser() {
    string name, email, phoneNumber;
    char gender;
    int age;

    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your gender(M/F): ";
    cin >> gender;
    cin.ignore();
    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your phone number: ";
    getline(cin, phoneNumber);
    cout << "Enter your age: ";
    cin >> age;
    cin.ignore();

    if (gender == 'F' || gender == 'f')
        gender = false; // Female
    else 
        gender = true;  // Male

    User user(age, name, email, gender, phoneNumber);
    user.display();

    user.saveUser();

    cout << "User email: " << user.email << " registered successfully!" << endl;
}

// Register and store drivers in json file
void registerDriver() {
    string name, email, phoneNumber, licenseNumber;
    int age, yearsOfExperience;
    char gender;

    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your phone number: ";
    getline(cin, phoneNumber);
    cout << "Enter your age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter your gender(M/F): ";
    cin >> gender;
    cin.ignore();
    cout << "Enter your license number: ";
    getline(cin, licenseNumber);
    cout << "Enter your years of experience: ";
    cin >> yearsOfExperience;
    cin.ignore();
    cout << "Enter your vehicle type: (car/rickshaw/bike/Bus)";
    string vehicleType;
    getline(cin, vehicleType);

    if (gender == 'F' || gender == 'f')
        gender = false; // Female
    else 
        gender = true;  // Male

    Driver* driver = createRandomDriver(age, name, email, gender, phoneNumber, licenseNumber, yearsOfExperience, vehicleType);

    driver->saveDriver();

    cout << "Driver email: " << driver->email << " registered successfully!" << endl;
    driver->display();
}

// Sign in as a user by searching from the json file
User signInUser() {
    string email;
    cout << "Enter your email: ";
    getline(cin, email);
    User user = User::loadUser(email);
    
    return user;
}

// Sign in as a driver by searching from the json file
Driver signInDriver() {
    string email;
    cout << "Enter your email: ";
    getline(cin, email);
    Driver driver = Driver::loadDriver(email);
    
    return driver;
}

 
// Find the nearest driver to the user
struct DriverDistance {
    Driver* driver;
    float distance;

    bool operator>(const DriverDistance& other) const {
        return distance > other.distance;
    }
};


// std::priority_queue<DriverDistance, std::vector<DriverDistance>, std::greater<DriverDistance>> driverQueue;

float calculateDistance(Node* node1, Node* node2) {
    float dx = node1->x - node2->x;
    float dy = node1->y - node2->y;
    return sqrt(dx * dx + dy * dy);
}

Driver* findNearestDriver(Node* userLocation, const std::vector<Driver*>& drivers, const string& vehicleType) {
    std::priority_queue<DriverDistance, std::vector<DriverDistance>, std::greater<DriverDistance>> driverQueue;

    for (Driver* driver : drivers) {
        if (driver->availability && driver->assignedVehicle->type == vehicleType) {
            float distance = calculateDistance(userLocation, driver->assignedVehicle->currentNode);
            driverQueue.push({driver, distance});
        }
    }

    if (!driverQueue.empty()) {
        return driverQueue.top().driver;
    }

    return nullptr; // No available drivers
}

// create random driver
Driver* createRandomDriver(int age, string name, string email, bool gender, string phoneNumber, string licenseNumber, int yearsOfExperience, string vehicleType) {
    Node* currentNode = arrayOfNodes[rand() % arrayOfNodes.size()];
    Driver * driver = new Driver(age, name, email, gender, phoneNumber, licenseNumber, yearsOfExperience, vehicleType, currentNode);
    driver->saveDriver();
    return driver;
}

// Create random vehicle
Vehicle createRandomVehicle(const std::vector<Node*>& nodes) 
{
    Node* currentLocation = nodes[rand() % nodes.size()];
    string vehicleType = Vehicle::vehicleTypes[rand() % 4]; // Random vehicle type
    return Vehicle(rand() % 1000 + 1, vehicleType, nodes);
}

// Calculate the cost of the path
float getCost(Vehicle* nearestVehicle, Vehicle* newVehicle) {
    Node* mid = nearestVehicle->goalNode;
    Node* end = newVehicle->goalNode;

    float cost = Node::cost(mid, end)/10000000;

    return cost;
}


int main()
{
    // Create city
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    /*
    "Planeet Namek", "The Abyss", "Crystal Peak", "Colloseum of Fools", "Deepnest", 
        "King\'s Pass", "Final Destination", "Forgotten Crossroads", "Greenpath", "Howling Cliffs",
        "Green Hill Zone", "Fog Canyon", "Dirtmouth", "Royal Waterways"*/

    // Create nodes with names
    TrafficIntersection* node1Intersection = new TrafficIntersection(11, 1000, 800, "Planeet Namek", 4);
    TrafficIntersection* node2Intersection = new TrafficIntersection(12, 600, 400, "The Abyss", 4);
    TrafficIntersection* node3Intersection = new TrafficIntersection(13, 800, 100, "Crystal Peak", 4);
    TrafficIntersection* node4Intersection = new TrafficIntersection(14, 1300, 950, "Colloseum of Fools", 4);
    TrafficIntersection* node5Intersection = new TrafficIntersection(15, 1800, 390, "Deepnest", 3);
    TrafficIntersection* node6Intersection = new TrafficIntersection(16, 1860, 490, "King\'s Pass", 3);
    TrafficIntersection* node7Intersection = new TrafficIntersection(17, 1700, 490, "Final Destination", 3);
    TrafficIntersection* node8Intersection = new TrafficIntersection(18, 1600, 450, "Forgotten Crossroads", 3);
    TrafficIntersection* node9Intersection = new TrafficIntersection(19, 1200, 530, "Greenpath", 3);
    TrafficIntersection* node10Intersection = new TrafficIntersection(20, 1000, 100, "Howling Cliffs", 3);
    TrafficIntersection* node11Intersection = new TrafficIntersection(21, 210, 450, "Green Hill Zone", 3);
    TrafficIntersection* node12Intersection = new TrafficIntersection(22, 500, 900, "Fog Canyon", 3);
    TrafficIntersection* node13Intersection = new TrafficIntersection(23, 800, 800, "Dirtmouth", 3);
    TrafficIntersection* node14Intersection = new TrafficIntersection(24, 1600, 800, "Royal Waterways", 3);

    TrafficIntersection* intersections[14] = {
     node1Intersection,
     node2Intersection,
     node3Intersection,
     node4Intersection,
     node5Intersection,
     node6Intersection,
     node7Intersection,
     node8Intersection,
     node9Intersection,
     node10Intersection,
     node11Intersection,
     node12Intersection,
     node13Intersection,
     node14Intersection
    };

    /* "Kami\'s Lookout", "Tournament of Power", "Chamber of Spirit and Time", "Hidden Leaf Village", "Heuko Mondo",
        "Infinity Castle", "AWC Housing Society", "Abdul Wahab Society", "Soul Society", "City of Tears",
    */
        
    Node* node1 = new Node(1, 200, 200, "Kami\'s Lookout");
    Node* node2 = new Node(2, 400, 600, "Tournament of Power");
    Node* node3 = new Node(3, 1200, 250, "Chamber of Spirit and Time");
    Node* node4 = new Node(4, 830, 500, "Hidden Leaf Village");
    Node* node5 = new Node(5, 1000, 1000, "Heuko Mondo");
    Node* node6 = new Node(6, 1200, 1070, "Infinity Castle");
    Node* node7 = new Node(7, 1850, 860, "AWC Housing Society");
    Node* node8 = new Node(8, 1200, 800, "Abdul Wahab Society");
    Node* node9 = new Node(9, 1400, 400, "Soul Society");
    Node* node10 = new Node(10, 1000, 600, "City of Tears");


    Node* node11 = dynamic_cast<Node*>(node1Intersection);
    Node* node12 = dynamic_cast<Node*>(node2Intersection);
    Node* node13 = dynamic_cast<Node*>(node3Intersection);
    Node* node14 = dynamic_cast<Node*>(node4Intersection);
    Node* node15 = dynamic_cast<Node*>(node5Intersection);
    Node* node16 = dynamic_cast<Node*>(node6Intersection);
    Node* node17 = dynamic_cast<Node*>(node7Intersection);
    Node* node18 = dynamic_cast<Node*>(node8Intersection);
    Node* node19 = dynamic_cast<Node*>(node9Intersection);
    Node* node20 = dynamic_cast<Node*>(node10Intersection);
    Node* node21 = dynamic_cast<Node*>(node11Intersection);
    Node* node22 = dynamic_cast<Node*>(node12Intersection);
    Node* node23 = dynamic_cast<Node*>(node13Intersection);
    Node* node24 = dynamic_cast<Node*>(node14Intersection);
    

    node1->addNeighbor(node21);
    node1->addNeighbor(node13);
    node12->addNeighbor(node21);
    node12->addNeighbor(node2);
    node12->addNeighbor(node13);
    node12->addNeighbor(node4);
    node21->addNeighbor(node22);
    node4->addNeighbor(node19);
    node13->addNeighbor(node3);
    node13->addNeighbor(node20);
    node20->addNeighbor(node3);
    node20->addNeighbor(node15);
    node22->addNeighbor(node6);
    node23->addNeighbor(node5);
    node22->addNeighbor(node23);
    node23->addNeighbor(node11);
    node11->addNeighbor(node5);
    node11->addNeighbor(node8);
    node11->addNeighbor(node10);
    node10->addNeighbor(node19);
    node14->addNeighbor(node6);
    node14->addNeighbor(node7);
    node14->addNeighbor(node24);
    node14->addNeighbor(node8);
    node19->addNeighbor(node9);
    node24->addNeighbor(node17);
    node24->addNeighbor(node16);
    node9->addNeighbor(node18);
    node18->addNeighbor(node17);
    node18->addNeighbor(node15);
    node17->addNeighbor(node16);
    node16->addNeighbor(node15);
    node16->addNeighbor(node20);
    node3->addNeighbor(node15);
    node15->addNeighbor(node14);
    node23->addNeighbor(node21);
    node22->addNeighbor(node12);
    node23->addNeighbor(node12);
    node14->addNeighbor(node11);
    node3->addNeighbor(node12);
    node13->addNeighbor(node23);
    node19->addNeighbor(node11);
    node9->addNeighbor(node24);




    vector<Edge*> uniqueEdges;
    unordered_set<string> edgeSet; // To ensure edges are added only once

    arrayOfNodes = {
    node1, node2, node3, node4, node5, node6,
    node7, node8, node9, node10, node11, node12,
    node13, node14, node15, node16, node17, node18,
    node19, node20, node21, node22, node23, node24
    };


    Vehicle v6(1, "Car", node11, node1);
    Vehicle v2(2, "Rickshaw", node13, node24);
    Vehicle v3(3, "Bike", node15, node2);
    Vehicle v4(4, "Bus", node4, node18);
    Vehicle v5(5, "Car", node21, node19);

    // Creating drivers and assigning vehicles

    Driver* d1 = createRandomDriver(25, "Ali", "ali@driver.com", true, "03001234567", "ABC123", 5, "Car");
    Driver* d2 = createRandomDriver(30, "Ahmed", "ahmed@driver.com", true, "03123456789", "XYZ456", 3, "Rickshaw");
    Driver* d3 = createRandomDriver(35, "Sara", "sara@driver.com", false, "03234567890", "DEF789", 7, "Bike");
    Driver* d4 = createRandomDriver(40, "John", "john@driver.com", true, "03345678901", "GHI012", 4, "Bus");
    Driver* d5 = createRandomDriver(45, "Maria", "maria@driver.com", false, "03456789012", "JKL345", 6, "Car");


    d1->assignedVehicle = &v6;
    d2->assignedVehicle = &v2;
    d3->assignedVehicle = &v3;
    d4->assignedVehicle = &v4;
    d5->assignedVehicle = &v5;

    v6.speed = 0.7;
    v2.speed = 1.4;
    v3.speed = 0.9;
    v4.speed = 0.2;
    v5.speed = 0.7;

    vector<Vehicle> vehicles;
    vehicles.push_back(v6);
    vehicles.push_back(v2);
    vehicles.push_back(v3);
    vehicles.push_back(v4);
    vehicles.push_back(v5);

    vector<Driver*> drivers; // For storing all drivers
    drivers.push_back(d1);
    drivers.push_back(d2);
    drivers.push_back(d3);
    drivers.push_back(d4);
    drivers.push_back(d5);

    /*Image image = LoadImage("car.png");

    Texture2D texture = LoadTextureFromImage(image);*/

    Vector2 buttonPosition = {120, 950};
    Vector2 buttonSize = {150, 50}; 
    bool buttonPressed = false;


    for (Node* node : arrayOfNodes)
    {
        for (Edge* edge : node->edges)
        {
            // Create a unique key for the edge (e.g., "1-2" or "2-1")
            int id1 = edge->node1->id;
            int id2 = edge->node2->id;
            string edgeKey = id1 < id2 ? to_string(id1) + "-" + to_string(id2)
                : to_string(id2) + "-" + to_string(id1);

            // Add to uniqueEdges if not already present
            if (edgeSet.find(edgeKey) == edgeSet.end())
            {
                uniqueEdges.push_back(edge);
                edgeSet.insert(edgeKey);
            }
        }
    }

    for (Edge* edge : uniqueEdges) {
        edge->no_of_agents = edge->max_traffic-(rand()%6);
    }

    //cout << "Welcome to the Traffic Congestion Control System\n";

    const char* locations[] = {
        "Kami\'s Lookout", "Tournament of Power", "Chamber of Spirit and Time", "Hidden Leaf Village", "Heuko Mondo",
        "Infinity Castle", "AWC Housing Society", "Abdul Wahab Society", "Soul Society", "City of Tears",
        "Planeet Namek", "The Abyss", "Crystal Peak", "Colloseum of Fools", "Deepnest", 
        "King\'s Pass", "Final Destination", "Forgotten Crossroads", "Greenpath", "Howling Cliffs",
        "Green Hill Zone", "Fog Canyon", "Dirtmouth", "Royal Waterways"
    };

    const char* vehicleTypes[] = {
    "Rickshaw", "Car", "Bike", "Bus"
    };

    // User and driver registration and sign in

    User currentUser; // For storing the current user
    Driver currentDriver; // For storing the current driver
    bool exit = false;
    do 
    {
        cout << "-----------------------------------\n";
        cout << "Welcome to Driveby\n";
        cout << "Please select an option:\n";
        cout << "1. Register as a user\n";
        cout << "2. Register as a driver\n";
        cout << "3. Sign in as a user\n";
        cout << "4. Sign in as a driver\n";
        cout << "5. Exit the program\n";
        cout << "-----------------------------------\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Registering as a user...\n";
            registerUser();
            break;
        case 2:
            cout << "Registering as a driver...\n";
            registerDriver();
            break;
        case 3:
            cout << "Signing in as a user...\n";
            currentUser = signInUser();
            if (currentUser.email == "") {
                cout << "User not found. Please register first." << endl;
                break;
            }
            else {
                cout << currentUser.email << " Logged in as User!" << endl;
                currentUser.display();
            }
            exit = true;
            break;
        case 4:
            cout << "Signing in as a driver...\n";
            currentDriver = signInDriver();
            if (currentDriver.email == "") {
                cout << "Driver not found. Please register first." << endl;
                break;
            }
            else {
                cout << "Logged in as Driver!" << endl;
                currentDriver.display();
            }
            exit = true;
            break;
        case 5:
            cout << "Exiting the program...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (!exit);

    int vehicle;    // Vehicle to be used for ride
    int start, end; // Starting and ending locations

    // Menu for user and driver options
    if (currentUser.email != "") 
    {
        while (true)
        {
        cout << "-----------------------------------\n";
        cout << "Welcome " << currentUser.name << "!\n";
        cout << "Please select an option:\n";
        cout << "1. Request a ride\n";
        cout << "2. View ride history\n";
        cout << "3. Exit the program\n";
        cout << "-----------------------------------\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Requesting a ride...\n";
            // Select vehicle
            while (true) 
            {
                cout << "-----------------------------------\n";
                cout << "Please select the vehicle you want to use:\n";
                cout << "1. Rickshaw\n";
                cout << "2. Car\n";
                cout << "3. Bike\n";
                cout << "4. Bus\n";
                cout << "Enter -1 to exit the program.\n";
                cout << "-----------------------------------\n";

                cout << "Enter the number corresponding to your vehicle: ";
                cin >> vehicle;
                cin.ignore();

                if (vehicle == -1) {
                    cout << "Exiting the program...\n";
                    return 0;
                }

                if (vehicle >= 1 && vehicle <= 4) {
                    switch (vehicle) {
                    case 1:
                        cout << "Selected Rickshaw.\n";
                        break;
                    case 2:
                        cout << "Selected Car.\n";
                        break;
                    case 3:
                        cout << "Selected Bike.\n";
                        break;
                    case 4:
                        cout << "Selected Bus.\n";
                        break;
                    }
                    break;
                }
                else {
                    cout << "Invalid vehicle, please select a valid option.\n";
                }
            }
            // Enter location
            while (true) {
                cout << "Please select a starting point (1-24):\n";
                for (int i = 0; i < 24; i++) {
                    cout << (i + 1) << ". " << locations[i] << "\n";
                }

                cout << "Enter the number of your starting point: ";
                cin >> start;
                cin.ignore();

                cout << "Enter the number of your destination point (1-24): ";
                cin >> end;
                cin.ignore();

                if (start < 1 || start > 24 || end < 1 || end > 24) {
                    cout << "Invalid input, please select numbers between 1 and 24.\n";
                    continue;
                }

                if (start == end) {
                    cout << "Starting point and destination cannot be the same. Please try again.\n";
                    continue;
                }

                cout << "You selected the starting point: " << locations[start - 1] << "\n";
                cout << "You selected the destination point: " << locations[end - 1] << "\n";

                Vehicle v1(12, vehicleTypes[vehicle - 1], arrayOfNodes);
                Driver* d6 = createRandomDriver(27, "Arham", "arham@driver.com", true, "03001234567", "ABC123", 10, vehicleTypes[vehicle - 1]);
                // v1.color = ORANGE;
                v1.speed = 0.9;
                vehicles.push_back(v1);
                d6->assignedVehicle = &v1;
                drivers.push_back(d6);


                Driver* nearestDriver = findNearestDriver(arrayOfNodes[start - 1], drivers, vehicleTypes[vehicle - 1]);
                if (nearestDriver != nullptr) 
                {
                char choice;
                cout << "Nearest driver found: " << nearestDriver->name << endl;
                nearestDriver->display();
                cout << "Do you want to request the ride? (Y/N): ";
                cin >> choice;
                cin.ignore();
                if (choice == 'Y' || choice == 'y') {
                    currentUser.requestRide(arrayOfNodes[start - 1], arrayOfNodes[end - 1]);
                }
                }
                else 
                {
                    cout << "No drivers available at the moment." << endl;
                    return 0;
                }

                Vehicle* nearestVehicle = nearestDriver->acceptRide(currentUser);
                vehicles.push_back(*nearestVehicle);
                // nearestDriver->assignedVehicle->color = ORANGE;
                cout << "Ride color changed for " << nearestDriver->assignedVehicle->type << endl;

                if (nearestDriver->assignedVehicle->goalNode == nullptr) {
                cout << "goal node is null" << endl;
                }
                if (nearestDriver->assignedVehicle->userGoalNode == nullptr) {
                cout << "user goal node is null" << endl;
                }

                cout << "path of driver: " << nearestDriver->assignedVehicle->currentNode->name << " to " << nearestDriver->assignedVehicle->goalNode->name << " to " << nearestDriver->assignedVehicle->userGoalNode->name << endl;

                // Raylib window
                InitWindow(1920, 1080, "Ride Sharing App");

                Image trafficLight = LoadImage("src\\utils\\light.png");
                Texture2D textureLight = LoadTextureFromImage(trafficLight);

                SetTargetFPS(60);
                int offsetX = -37;
                int offsetY = -50;
                int i = 0;

                Vehicle* newVehicle = nullptr;
                bool userReachedDestination = false; // New flag to track if the user has reached the destination

                while (!WindowShouldClose()) {
                    i++;
                    if (i == 100) {
                        for (TrafficIntersection* intersection : intersections) {
                            intersection->changeLights();
                        }
                        i = 0;
                    }
                    BeginDrawing();
                    ClearBackground(LIGHTGRAY);  // Clear the screen

                    // Draw the traffic lights
                    for (TrafficIntersection* intersection : intersections) {
                        DrawTextureEx(textureLight, Vector2{ intersection->x + offsetX - 20, intersection->y + offsetY + 5 }, 0.0f, 0.09, WHITE);
                    }

                    // Check mouse and button interactions
                    Vector2 mousePosition = GetMousePosition();
                    bool isMouseOverButton = CheckCollisionPointRec(mousePosition, Rectangle{buttonPosition.x, buttonPosition.y, buttonSize.x, buttonSize.y});

                    if (isMouseOverButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        buttonPressed = true;
                    }

                    if (isMouseOverButton) {
                        DrawRectangle(buttonPosition.x, buttonPosition.y, buttonSize.x, buttonSize.y, LIGHTGRAY);
                    } else {
                        DrawRectangle(buttonPosition.x, buttonPosition.y, buttonSize.x, buttonSize.y, DARKGRAY);
                    }

                    // Draw button text
                    DrawText("Add car", buttonPosition.x + 37, buttonPosition.y + 15, 20, RED);

                    // Display the result of button press
                    if (buttonPressed) {
                        int k = rand() % 24;
                        int j = k;
                        while (j == k) {
                            j = rand() % 24;
                        }
                        Vehicle vehicle(1, "Car", arrayOfNodes[k], arrayOfNodes[j]);
                        vehicles.push_back(vehicle);
                        buttonPressed = false;
                    }

                    // Draw the nodes and edges
                    for (Node* node : arrayOfNodes) {
                        DrawCircle(node->x + offsetX, node->y + offsetY, 5, RED);
                        const char* name = node->name.c_str();
                        DrawText(name, node->x + offsetX - 30, node->y + offsetY - 20, 20, PINK);
                    }

                    for (Edge* edge : uniqueEdges) {
                        DrawLine(edge->node1->x + offsetX, edge->node1->y + offsetY, edge->node2->x + offsetX, edge->node2->y + offsetY, RED);
                        float midpointX = (edge->node1->x + edge->node2->x) / 2.0f;
                        float midpointY = (edge->node1->y + edge->node2->y) / 2.0f;
                        string congestionString = to_string(edge->no_of_agents);
                        const char* congestion = congestionString.c_str();
                        DrawText(congestion, midpointX + offsetX, midpointY + offsetY - 20, 25, BLACK);
                    }

                    for (Vehicle& v : vehicles) {
                        if (!v.hasReachedDestination) {
                            DrawRectangle(v.x + offsetX, v.y + offsetY, 25, 30, v.color);

                            if (v.moveVehicle()) {
                                cout << "Vehicle reached destination" << v.id << endl;
                                v.hasReachedDestination = true;
                                nearestDriver->availability = true;
                            }
                        } else if (v.id == 0 && nearestDriver->availability && !nearestDriver->reachedDestination) {
                            cout << "Starting ride" << endl;
                            newVehicle = nearestDriver->startRide(currentUser);
                            nearestDriver->assignedVehicle = newVehicle;
                            vehicles.push_back(*newVehicle);
                        }
                    }

                    // Check if the nearest driver has reached the destination
                    if (nearestDriver->reachedDestination && !userReachedDestination && newVehicle->hasReachedDestination) {
                        userReachedDestination = true; // Set the flag
                    }

                    // Display the message when the user has reached the destination
                    if (userReachedDestination) {
                        DrawText("You have reached your destination!", 800, 500, 30, GREEN);
                    }

                    EndDrawing();
                }
                CloseWindow();

                cout << "VEHICLE REACHED DESTINATION..." << endl;
                float cost = getCost(nearestVehicle, newVehicle);
                cout << "Total cost of the ride: " << cost << endl;


                // Prompt user for rating
                if (nearestDriver != nullptr) 
                {
                    double rating;
                    cout << "Please rate your ride with driver " << nearestDriver->name << " (0-5): ";
                    cin >> rating;

                    // Validate rating
                    while (rating < 0 || rating > 5) {
                        cout << "Invalid rating. Please enter a value between 0 and 5: ";
                        cin >> rating;
                    }

                    // Update and save driver rating
                    nearestDriver->takeRating(rating);
                    nearestDriver->availability = true;
                    nearestDriver->reachedDestination = false;
                    nearestDriver->saveDriver();

                    // Print updated driver ratings
                    cout << "Thank you for rating!" << endl;
                    cout << "Updated Driver Information:" << endl;
                    nearestDriver->display();

                    cout << "Do you want to request another ride? (Y/N): ";
                    char choice;
                    cin >> choice;
                    cin.ignore();
                    if (choice == 'Y' || choice == 'y') {
                        continue;
                    }
                    else {
                        cout << "Exiting the program...\n";
                        return 0;
                    }
                }

                break;
            }
            break;
        case 2:
            cout << "Viewing ride history...\n";
            break;
        case 3:
            cout << "Exiting the program...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
        }
    }
    else if (currentDriver.email != "") {
        cout << "-----------------------------------\n";
        cout << "Welcome " << currentDriver.email << "!\n";
        cout << "Please select an option:\n";
        cout << "1. Accept a ride\n";
        cout << "2. View ride history\n";
        cout << "3. Exit the program\n";
        cout << "-----------------------------------\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Accepting a ride...\n";
            break;
        case 2:
            cout << "Viewing ride history...\n";
            break;
        case 3:
            cout << "Exiting the program...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
