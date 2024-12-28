#ifndef USER_H
#define USER_H

#include "person.h"

// User class inheriting from Person
class User : public Person {
public: 
    string rideStatus; // None, Requested, Active

    User();
    User(int age, string name, string email, bool gender, string phoneNumber, string location);

    bool requestRide();

    void setRideStatus(string newRideStatus);

    void display() const override;

    void saveUser() const;
    static User loadUser(const std::string& email);
};

#endif