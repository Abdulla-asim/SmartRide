#include "driver.h"
#include <fstream>
#include <nlohmann/json.hpp>

// Driver class inheriting from Person

Driver::Driver() : Person(), licenseNumber(""), yearsOfExperience(0), averageRating(0.0), numberOfRidesCompleted(0), availability(true) {}
Driver::Driver(int age, string name, string email, bool gender, string phoneNumber, string location, string licenseNumber, int yearsOfExperience)
    : Person(age, name, email, gender, phoneNumber, location), licenseNumber(licenseNumber), yearsOfExperience(yearsOfExperience),
        averageRating(0.0), numberOfRidesCompleted(0), availability(true) {}

void Driver::setLicenseNumber(string newLicenseNumber) { licenseNumber = newLicenseNumber; }
void Driver::setYearsOfExperience(int newYearsOfExperience) { yearsOfExperience = newYearsOfExperience; }
void Driver::setAvailability(bool newAvailability) { availability = newAvailability; }

void Driver::acceptRide(const User& user) 
{
    if (availability && user.rideStatus == "Requested") 
    {
        availability = false;
        cout << "Driver " << name << " accepted the ride for " << user.name << "." << endl;
    } 
    else 
    {
        cout << "Driver unavailable or invalid ride request!" << endl;
    }
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
    cout << "Years of Experience: " << yearsOfExperience << endl;
    cout << "Number of Rides Completed: " << numberOfRidesCompleted << endl;
    cout << "Average Rating: " << averageRating << endl;
    cout << "Availability: " << (availability ? "Available" : "Unavailable") << endl;
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

    // Ensure driversJson is an array
    if (!driversJson.is_array()) {
        driversJson = json::array();
    }

    json driverJson = {
        {"name", name},
        {"email", email},
        {"phone", phoneNumber},
        {"age", age},
        {"location", location},
        {"licenseNumber", licenseNumber},
        {"yearsOfExperience", yearsOfExperience}
    };

    driversJson.push_back(driverJson);

    std::ofstream outputFile("drivers.json");
    if (outputFile.is_open()) {
        outputFile << driversJson.dump(4) << std::endl; // Pretty print with 4 spaces
        outputFile.close();
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

// Load driver
Driver* Driver::loadDriver(const std::string& email) 
{
    Driver* driver;
    ifstream driverFile("drivers.json");
    if (driverFile.is_open()) 
    {
        nlohmann::json j;
        driverFile >> j;
        for (const auto& item : j) 
        {
            if (item["email"] == email) 
            {
                driver->name = item["name"];
                driver->email = item["email"];
                driver->phoneNumber = item["phone_number"];
                driver->age = item["age"];
                driver->location = item["location"];
                driver->licenseNumber = item["license_number"];
                driver->yearsOfExperience = item["years_of_experience"];
                driver->averageRating = item["average_rating"];
                driver->numberOfRidesCompleted = item["number_of_rides_completed"];
                driver->availability = item["availability"];
                return driver;
            }
        }
        driverFile.close();
    } 
    else 
    {
        cerr << "Error opening file for reading." << endl;
    }
    return driver;
}