#include "user.h"
#include <fstream>
#include <nlohmann/json.hpp>

string rideStatus; // None, Requested, Active

User::User(int age, string name, string email, bool gender, string phoneNumber, string location)
    : Person(age, name, email, gender, phoneNumber, location), rideStatus("None") {}

User::User() : Person(), rideStatus("None") {}

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


using json = nlohmann::json;

void User::saveUser() const {
    std::ifstream inputFile("users.json");
    json usersJson;

    if (inputFile.is_open()) {
        if (inputFile.peek() != std::ifstream::traits_type::eof()) {
            inputFile >> usersJson;
        }
        inputFile.close();
    }

    // Ensure usersJson is an array
    if (!usersJson.is_array()) {
        usersJson = json::array();
    }

    json userJson = {
        {"name", name},
        {"email", email},
        {"phone", phoneNumber},
        {"age", age},
        {"location", location}
    };

    usersJson.push_back(userJson);

    std::ofstream outputFile("users.json");
    if (outputFile.is_open()) {
        outputFile << usersJson.dump(4) << std::endl; // Pretty print with 4 spaces
        outputFile.close();
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

User* User::loadUser(const std::string& email) {
    User *user;
    std::ifstream file("users.json");
    if (file.is_open()) {
        json j;
        file >> j;
        for (const auto& item : j) {
            if (item["email"] == email) {
                user->name = item["name"];
                user->email = item["email"];
                user->phoneNumber = item["phone"];
                user->age = item["age"];
                user->location = item["location"];
                return user;
            }
        }
        file.close();
    } else {
        std::cerr << "Error opening file for reading." << std::endl;
    }
    return user; // Return an empty user if not found
}