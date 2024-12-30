#ifndef DRIVER_H
#define DRIVER_H

#include "user.h"
#include "node.h"
#include "vehicle.h"
#include <vector>

vector<Edge*> aStar(Node* start, Node* goal);

// Driver class inheriting from Person
class Driver : public Person 
{
public:
    string licenseNumber;
    int yearsOfExperience;
    double averageRating;
    int numberOfRidesCompleted;
    vector<double> ratings;
    bool availability;
    string vehicleType;
    Node* currentNode;
    Vehicle* assignedVehicle;
    bool reachedDestination = false;

    Driver();
    Driver(vector<Node*> nodes);
    Driver(int age, string name, string email, bool gender, string phoneNumber, string licenseNumber, int yearsOfExperience, string vehicleType, Node* currentNode);   

    void setLicenseNumber(string newLicenseNumber);
    void setYearsOfExperience(int newYearsOfExperience);
    void setAvailability(bool newAvailability);

    Vehicle* acceptRide(const User& user);
    Vehicle* startRide(const User& user);

    void takeRating(double newRating);

    void display() const override;

    void saveDriver() const;
    static Driver loadDriver(const std::string& email);

    void moveAlongPath();
};

#endif