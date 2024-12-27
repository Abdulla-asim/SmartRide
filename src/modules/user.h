#include "person.h"

// User class inheriting from Person
class User : public Person {
public: 
    string rideStatus; // None, Requested, Active

    User(int age, string name, string email, bool gender, string phoneNumber, string location);

    bool requestRide();

    void setRideStatus(string newRideStatus);

    void display() const override;
};