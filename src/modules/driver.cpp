#include "driver.h"
#include <iostream>

// Driver class inheriting from Person

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