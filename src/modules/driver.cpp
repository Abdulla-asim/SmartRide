#include "driver.h"
#include <fstream>
#include <nlohmann/json.hpp>

// Driver class inheriting from Person

Driver::Driver() : Person(), licenseNumber(""), yearsOfExperience(0), averageRating(0.0), numberOfRidesCompleted(0), availability(true), currentNode(nullptr) {}

Driver::Driver(vector<Node*> nodes) : Person(), licenseNumber(""), yearsOfExperience(0), averageRating(0.0), numberOfRidesCompleted(0), availability(true), currentNode(nodes[rand() % nodes.size()]) {}

Driver::Driver(int age, string name, string email, bool gender, string phoneNumber, string licenseNumber, int yearsOfExperience, string vehicleType, Node* currentNode) 
    : Person(age, name, email, gender, phoneNumber), licenseNumber(licenseNumber), yearsOfExperience(yearsOfExperience),
        averageRating(0.0), numberOfRidesCompleted(0), availability(true), assignedVehicle(nullptr), vehicleType(vehicleType), currentNode(currentNode) {}

void Driver::setLicenseNumber(string newLicenseNumber) { licenseNumber = newLicenseNumber; }
void Driver::setYearsOfExperience(int newYearsOfExperience) { yearsOfExperience = newYearsOfExperience; }
void Driver::setAvailability(bool newAvailability) { availability = newAvailability; }

Vehicle* Driver::acceptRide(const User& user) 
{
    if (availability && user.rideStatus == "Requested") 
    {
        availability = false;
        cout << "Driver " << name << " accepted the ride for " << user.name << "." << endl;

        Vehicle* vehicle = new Vehicle(0, vehicleType, currentNode, user.currentLocation);
        vehicle->color = ORANGE; // Change vehicle color to orange
        vehicle->userGoalNode = user.goalLocation; // Set user's goal location
        assignedVehicle = vehicle;

        return vehicle;
        // Compute path from vehicle's current location to user's location, then to user's goal location
        // assignedVehicle->path = aStar(assignedVehicle->currentNode, assignedVehicle->goalNode); // Compute path
    } 
    else 
    {
        cout << "Driver unavailable or invalid ride request!" << endl;
    }
    return nullptr;
}

Vehicle* Driver::startRide(const User& user) 
{
    if (availability) 
    {
        availability = false;
        cout << "Driver " << name << " accepted the ride for " << user.name << "." << endl;

        Vehicle* vehicle = new Vehicle(0, vehicleType, user.currentLocation, user.goalLocation);
        vehicle->color = ORANGE; // Change vehicle color to orange
        vehicle->userGoalNode = user.goalLocation; // Set user's goal location
        assignedVehicle = vehicle;
        reachedDestination = true;

        return vehicle;
        // Compute path from vehicle's current location to user's location, then to user's goal location
        // assignedVehicle->path = aStar(assignedVehicle->currentNode, assignedVehicle->goalNode); // Compute path
    } 
    else 
    {
        cout << "Driver unavailable or invalid ride request!" << endl;
    }
    return nullptr;
}

void Driver::takeRating(double newRating) 
{
    if (newRating < 0 || newRating > 5) 
    {
        cout << "Invalid rating! Ratings must be between 0 and 5." << endl;
        return;
    }
    ratings.push_back(newRating);
    numberOfRidesCompleted++;
    averageRating = 0; // Reset the averageRating
    for (double rating : ratings) 
    {
        averageRating += rating; // Sum all ratings
    }
    averageRating /= ratings.size(); // Calculate average
}

void Driver::display() const 
{
    Person::display();
    cout << "License Number: " << licenseNumber << endl;
    cout << "Vehicle Type: " << vehicleType << endl;
    cout << "Years of Experience: " << yearsOfExperience << endl;
    cout << "Number of Rides Completed: " << numberOfRidesCompleted << endl;
    cout << "Average Rating: " << averageRating << endl;
    cout << "Availability: " << (availability ? "Available" : "Unavailable") << endl;
    cout << "-----------------------------------" << endl;
}

using json = nlohmann::json;

void Driver::saveDriver() const {
    std::ifstream inputFile("drivers.json");
    json driversJson;

    if (inputFile.is_open()) {
        if (inputFile.peek() != std::ifstream::traits_type::eof()) {
            inputFile >> driversJson;
        }
        inputFile.close();
    }

    // Ensure driversJson is an object
    if (!driversJson.is_object()) {
        driversJson = json::object();
    }

    // Update or add the driver information using email as the key
    driversJson[email] = {
        {"name", name},
        {"gender", gender},
        {"phone", phoneNumber},
        {"age", age},
        {"vehicleType", vehicleType},
        {"licenseNumber", licenseNumber},
        {"yearsOfExperience", yearsOfExperience},
        {"averageRating", averageRating},
        {"numberOfRidesCompleted", numberOfRidesCompleted},
        {"availability", availability}
    };

    std::ofstream outputFile("drivers.json");
    if (outputFile.is_open()) {
        outputFile << driversJson.dump(4) << std::endl; // Pretty print with 4 spaces
        outputFile.close();
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

// Load driver
Driver Driver::loadDriver(const std::string& email) 
{
    Driver driver;
    ifstream driverFile("drivers.json");
    if (driverFile.is_open()) 
    {
        nlohmann::json j;
        driverFile >> j;
        if (j.contains(email)) 
        {
            auto item = j[email];
            driver.name = item.value("name", "");
            driver.email = email;
            driver.gender = item.value("gender", false);
            driver.phoneNumber = item.value("phone", "");
            driver.age = item.value("age", 0);
            driver.vehicleType = item.value("vehicleType", "");
            driver.licenseNumber = item.value("licenseNumber", "");
            driver.yearsOfExperience = item.value("yearsOfExperience", 0);
            driver.averageRating = item.value("averageRating", 0.0);
            driver.numberOfRidesCompleted = item.value("numberOfRidesCompleted", 0);
            driver.availability = item.value("availability", true);
        }
        driverFile.close();
    } 
    else 
    {
        cerr << "Error opening file for reading." << endl;
    }
    return driver;
}

// Function to load all drivers from the drivers.json file
std::vector<Driver> Driver::loadAllDrivers() {
    std::vector<Driver> drivers;
    std::ifstream driverFile("drivers.json");
    if (driverFile.is_open()) {
        nlohmann::json j;
        driverFile >> j;
        for (auto it = j.begin(); it != j.end(); ++it) {
            Driver driver;
            auto item = it.value();
            driver.name = item.value("name", "");
            driver.email = it.key();
            driver.gender = item.value("gender", false);
            driver.phoneNumber = item.value("phone", "");
            driver.age = item.value("age", 0);
            driver.vehicleType = item.value("vehicleType", "");
            driver.licenseNumber = item.value("licenseNumber", "");
            driver.yearsOfExperience = item.value("yearsOfExperience", 0);
            driver.averageRating = item.value("averageRating", 0.0);
            driver.numberOfRidesCompleted = item.value("numberOfRidesCompleted", 0);
            driver.availability = item.value("availability", true);
            drivers.push_back(driver);
        }
        driverFile.close();
    } else {
        std::cerr << "Error opening file for reading." << std::endl;
    }
    return drivers;
}

void Driver::moveAlongPath() {
    if (assignedVehicle && !assignedVehicle->path.empty()) {
        assignedVehicle->moveVehicle(); // Update vehicle's position
        if (assignedVehicle->path.empty()) {
            availability = true; // Mark driver as available
        }
    }
}