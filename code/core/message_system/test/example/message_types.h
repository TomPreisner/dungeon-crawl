/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <vector>
#include <string>

////////////////////////////////////////////////////////
struct MessageData {
    virtual std::string get_name() const = 0;
};

////////////////////////////////////////////////////////
struct Location {
    std::string street_address;
    std::string city;
    std::string state;
};

////////////////////////////////////////////////////////
struct SuspectDescription : public MessageData {
    int case_number;
    float height;
    std::string clothing;
    std::string identifiers;

    virtual std::string get_name() const override { return "SuspectDescription"; }
};

////////////////////////////////////////////////////////
struct CatStuckInTree : public MessageData {
    Location location;
    std::string cat_name;

    virtual std::string get_name() const override { return "CatStuckInTree"; }
};

////////////////////////////////////////////////////////
struct FireReport : public MessageData {
    Location location;
    std::vector<std::string> occupants;

    virtual std::string get_name() const override { return "FireReport"; }
};

////////////////////////////////////////////////////////
struct MedicalEvent : public MessageData {
    Location location;
    std::string patient_name;

    virtual std::string get_name() const override { return "MedicalEvent"; }
};

////////////////////////////////////////////////////////
struct CarAccident : public MessageData {
    Location location;
    std::string patient_name;
    std::string car_make;
    std::string car_model;

    virtual std::string get_name() const override { return "CarAccident"; }
};

////////////////////////////////////////////////////////
struct Robbery : public MessageData {
    Location location;

    virtual std::string get_name() const override { return "Robbery"; }
};

////////////////////////////////////////////////////////
struct Interview : public MessageData {
    int case_number;

    virtual std::string get_name() const override { return "Interview"; }
};
