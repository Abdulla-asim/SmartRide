#include "user.h"
#include "node.h"
#include <fstream>
#include <nlohmann/json.hpp>

string rideStatus; // None, Requested, Active

User::User(int age, string name, string email, bool gender, string phoneNumber)
    : Person(age, name, email, gender, phoneNumber), rideStatus("None"), currentLocation(nullptr) {}

User::User() : Person(), rideStatus("None") {}

bool User::requestRide(Node* currentLocation, Node* goalLocation) 
{
    rideStatus = "Requested";
    this->currentLocation = currentLocation;
    this->goalLocation = goalLocation;

    cout << "--- Ride Request ---" << endl;
    cout << "Ride requested from " << currentLocation->name << " to " << goalLocation->name << endl;
    return true;                // return true
}

void User::setRideStatus(string newRideStatus) { rideStatus = newRideStatus; }

void User::display() const
{
    Person::display();
    cout << "Ride Status: " << rideStatus << endl;
    cout << "-----------------------------------" << endl;
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

    // Check if the user already exists in the JSON array
    bool userExists = false;
    for (auto& item : usersJson) {
        if (item["email"] == email) {
            // Update existing user information
            item["name"] = name;
            item["gender"] = gender;
            item["phone"] = phoneNumber;
            item["age"] = age;
            userExists = true;
            break;
        }
    }

    json userJson = {
        {"name", name},
        {"email", email},
        {"gender", gender},
        {"phone", phoneNumber},
        {"age", age},
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

User User::loadUser(const std::string& email) {
    User user;
    std::ifstream file("users.json");
    if (file.is_open()) {
        cout << "Opened file" << endl;
        json j;
        file >> j;
        for (const auto& item : j) {
            if (item["email"] == email) {
                cout << "User found" << endl;

                user.name = item["name"];
                user.email = item["email"];
                user.gender = item["gender"];
                user.phoneNumber = item["phone"];
                user.age = item["age"];
                return user;
            }
        }
        file.close();
    } else {
        std::cerr << "Error opening file for reading." << std::endl;
    }
    return user; // Return an empty user if not found
}

// User User::loadUser(const std::string& email) {
//     User user;
//     std::ifstream file("users.json");
//     if (file.is_open()) {
//         if (file.peek() != std::ifstream::traits_type::eof()) {
//             json j;
//             try {
//                 file >> j;
//                 for (const auto& item : j) {
//                     if (item["email"] == email) {
//                         user.name = item["name"];
//                         user.email = item["email"];
//                         user.phoneNumber = item["phone"];
//                         user.age = item["age"];
//                         user.location = item["location"];
//                         return user;
//                     }
//                 }
//             } catch (const json::parse_error& e) {
//                 std::cerr << "Parse error: " << e.what() << std::endl;
//             }
//         }
//         file.close();
//     } else {
//         std::cerr << "Error opening file for reading." << std::endl;
//     }
//     return user; // Return an empty user if not found
// }