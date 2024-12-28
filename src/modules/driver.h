#ifndef DRIVER_H
#define DRIVER_H

#include "user.h"
#include "node.h"
#include "vehicle.h"
#include <vector>

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
    Node* currentLocation;
    Vehicle* assignedVehicle;

    Driver();
    Driver(int age, string name, string email, bool gender, string phoneNumber, string location, string licenseNumber, int yearsOfExperience, string vehicleType);

    void setLicenseNumber(string newLicenseNumber);
    void setYearsOfExperience(int newYearsOfExperience);
    void setAvailability(bool newAvailability);

    void acceptRide(const User& user);

    void takeRating(double newRating);

    void display() const override;

    void saveDriver() const;
    static Driver loadDriver(const std::string& email);

    void moveAlongPath();
};

#endif