#include "person.h"

Person::Person() : age(0), name(""), email(""), gender(true), phoneNumber(""), location("") {}
Person::Person(int age, string name, string email, bool gender, string phoneNumber, string location)
    : age(age), name(name), email(email), gender(gender), phoneNumber(phoneNumber), location(location) {}

Person::~Person() {}

void Person::setAge(int newAge) { age = newAge; }
void Person::setName(string newName) { name = newName; }
void Person::setGender(bool newGender) { gender = newGender; }
void Person::setPhoneNumber(string newPhoneNumber) { phoneNumber = newPhoneNumber; }
void Person::setLocation(string newLocation) { location = newLocation; }

void Person::display() const {
    cout << "-----------------------------------" << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Email: " << email << endl;
    cout << "Gender: " << (gender ? "Male" : "Female") << endl;
    cout << "Phone Number: " << phoneNumber << endl;
    cout << "Location: " << location << endl;
}
