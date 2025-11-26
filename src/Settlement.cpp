#include "Settlement.h"
#include <string>
#include <iostream>

Settlement:: Settlement(const string &name, SettlementType type)
: name(name), type(type){}

const string & Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement:: toString() const
{
     string toString = "Settlement Name: " + name;

    // Add the type to the stringstream
    switch (type) {
        case SettlementType::VILLAGE:
            return toString + "Type: Village";
        case SettlementType::CITY:
            return toString + "Type: City";
        case SettlementType::METROPOLIS:
            return toString + "Type: Metropolis";
    }
    return "Unknown";
}

const int Settlement::getConstructionLimit() const{
    if(type == SettlementType::VILLAGE){
        return 1;
    }
    else if(type == SettlementType::CITY){
        return 2;
    }
    else{
        return 3;
    }
}

