#include "user.h"
#include <iostream>

string rideStatus; // None, Requested, Active

User::User(int age, string name, string email, bool gender, string phoneNumber, string location)
    : Person(age, name, email, gender, phoneNumber, location), rideStatus("None") {}

bool User::requestRide() 
{
    if (rideStatus == "None") 
    {   // Request if not already requested
        rideStatus = "Requested";
        return true;                // return true
    }
    return false; // User is Active or has Already Requested
}

void User::setRideStatus(string newRideStatus) { rideStatus = newRideStatus; }

void User::display() const
{
    Person::display();
    cout << "Ride Status: " << rideStatus << endl;
}