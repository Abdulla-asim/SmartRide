#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>

using namespace std;

class Person 
{
public:
    public:
    int age;
    string name;
    string email;
    bool gender; // Male: true, Female: false
    string phoneNumber;

    Person();
    Person(int age, string name, string email, bool gender, string phoneNumber);
    virtual ~Person();

    void setAge(int newAge);
    void setName(string newName);
    void setGender(bool newGender);
    void setPhoneNumber(string newPhoneNumber);
    void setLocation(string newLocation);
    virtual void display() const;
};

#endif