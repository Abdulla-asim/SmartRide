#include <iostream>
#include <string>
#include <functional>

using namespace std;

// Node structure for the custom map
template <typename Key, typename Value>
struct MapNode {
    Key key;
    Value value;
    MapNode* left;
    MapNode* right;

    MapNode(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

// Custom map class
template <typename Key, typename Value>
class CustomMap {
private:
    MapNode<Key, Value>* root;

    // Helper function to insert a key-value pair
    MapNode<Key, Value>* insertNode(MapNode<Key, Value>* node, const Key& key, const Value& value) {
        if (!node)
            return new MapNode<Key, Value>(key, value);

        if (key < node->key)
            node->left = insertNode(node->left, key, value);
        else if (key > node->key)
            node->right = insertNode(node->right, key, value);
        else
            node->value = value; // Update value if key already exists

        return node;
    }

    // Helper function to find a node by key
    MapNode<Key, Value>* findNode(MapNode<Key, Value>* node, const Key& key) const {
        if (!node || node->key == key)
            return node;

        if (key < node->key)
            return findNode(node->left, key);
        else
            return findNode(node->right, key);
    }

    // Helper function to display the map (in-order traversal)
    void displayInOrder(MapNode<Key, Value>* node) const {
        if (!node)
            return;
        displayInOrder(node->left);
        cout << node->key << ": ";
        node->value.display(); // Assumes Value type has a `display()` method
        cout << "--------------------------" << endl;
        displayInOrder(node->right);
    }

    // Helper function to delete all nodes
    void deleteTree(MapNode<Key, Value>* node) {
        if (!node)
            return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    CustomMap() : root(nullptr) {}

    ~CustomMap() { deleteTree(root); }

    // Insert or update a key-value pair
    void insert(const Key& key, const Value& value) {
        root = insertNode(root, key, value);
    }

    // Find value by key
    Value* find(const Key& key) const {
        MapNode<Key, Value>* node = findNode(root, key);
        return node ? &node->value : nullptr;
    }

    // Display the entire map
    void display() const {
        displayInOrder(root);
    }
};

/*
---

### Integrate CustomMap with User and Driver



Here’s how you can integrate the `CustomMap` with the `User` and `Driver` classes:

```cpp

*/

class Person {
public:
    int age;
    string name;
    string email;
    bool gender;
    string phoneNumber;
    string location;

    Person(int age, string name, string email, bool gender, string phoneNumber, string location)
        : age(age), name(name), email(email), gender(gender), phoneNumber(phoneNumber), location(location) {}

    void setAge(int newAge) { age = newAge; }
    void setName(string newName) { name = newName; }
    void setGender(bool newGender) { gender = newGender; }
    void setPhoneNumber(string newPhoneNumber) { phoneNumber = newPhoneNumber; }
    void setLocation(string newLocation) { location = newLocation; }

    virtual void display() const {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Email: " << email << endl;
        cout << "Gender: " << (gender ? "Male" : "Female") << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Location: " << location << endl;
    }
};

// User class inheriting from Person
class User : public Person {
public:
    string rideStatus; // None, Requested, Active

    User(int age, string name, string email, bool gender, string phoneNumber, string location)
        : Person(age, name, email, gender, phoneNumber, location), rideStatus("None") {}

    bool requestRide() {
            if (rideStatus == "None") {    // Request if not already requested
                rideStatus = "Requested";
                return true;                // return true
            }
            return false; // User is Active or has Already Requested
        }

    void setRideStatus(string newRideStatus) { rideStatus = newRideStatus; }

    void display() const override {
        Person::display();
        cout << "Ride Status: " << rideStatus << endl;
    }
};

// Driver class inheriting from Person
class Driver : public Person {
public:
    string licenseNumber;
    int yearsOfExperience;
    double averageRating;
    int numberOfRidesCompleted;
    vector<double> ratings;
    bool availability;

    Driver(int age, string name, string email, bool gender, string phoneNumber, string location, string licenseNumber, int yearsOfExperience)
        : Person(age, name, email, gender, phoneNumber, location), licenseNumber(licenseNumber), yearsOfExperience(yearsOfExperience),
          averageRating(0.0), numberOfRidesCompleted(0), availability(true) {}

    void setLicenseNumber(string newLicenseNumber) { licenseNumber = newLicenseNumber; }
    void setYearsOfExperience(int newYearsOfExperience) { yearsOfExperience = newYearsOfExperience; }
    void setAvailability(bool newAvailability) { availability = newAvailability; }

    void acceptRide(const User& user) {
        if (availability && user.rideStatus == "Requested") {
            availability = false;
            cout << "Driver " << name << " accepted the ride for " << user.name << "." << endl;
        } else {
            cout << "Driver unavailable or invalid ride request!" << endl;
        }
    }

    void takeRating(double newRating) {
        if (newRating < 0 || newRating > 5) {
            cout << "Invalid rating! Ratings must be between 0 and 5." << endl;
            return;
        }
        ratings.push_back(newRating);
        numberOfRidesCompleted++;
        averageRating = 0; // Reset the averageRating
        for (double rating : ratings) {
            averageRating += rating; // Sum all ratings
        }
        averageRating /= ratings.size(); // Calculate average
    }

    void display() const override {
        Person::display();
        cout << "License Number: " << licenseNumber << endl;
        cout << "Years of Experience: " << yearsOfExperience << endl;
        cout << "Number of Rides Completed: " << numberOfRidesCompleted << endl;
        cout << "Average Rating: " << averageRating << endl;
        cout << "Availability: " << (availability ? "Available" : "Unavailable") << endl;
    }
};

int main() {
    // Create custom maps for users and drivers
    CustomMap<string, User> userMap;
    CustomMap<string, Driver> driverMap;

    // Add some users to the custom map
    userMap.insert("alice@example.com", User(25, "Alice", "alice@example.com", false, "987-654-3210", "New York"));
    userMap.insert("charlie@example.com", User(30, "Charlie", "charlie@example.com", true, "654-123-9870", "Chicago"));

    // Add some drivers to the custom map
    driverMap.insert("bob@example.com", Driver(35, "Bob", "bob@example.com", true, "123-456-7890", "Los Angeles", "D123456", 10));
    driverMap.insert("jane.doe@example.com", Driver(29, "Jane", "jane.doe@example.com", false, "987-654-1230", "San Francisco", "D654321", 5));

    // Display users
    cout << "Users in the system:" << endl;
    userMap.display();

    // Display drivers
    cout << "\nDrivers in the system:" << endl;
    driverMap.display();

    // User requests a ride
    cout << "\nAlice requesting a ride..." << endl;
    User* alice = userMap.find("alice@example.com");
    if (alice && alice->requestRide()) {
        cout << "Ride requested successfully." << endl;
    }

    // Driver accepts the ride
    cout << "\nBob accepting Alice's ride request..." << endl;
    Driver* bob = driverMap.find("bob@example.com");
    if (bob && alice) {
        bob->acceptRide(*alice);
    }

    // Display updated details
    cout << "\nUpdated User and Driver Details:" << endl;
    if (alice) alice->display();
    cout << endl;
    if (bob) bob->display();

    return 0;
}
