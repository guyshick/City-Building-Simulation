#include "Facility.h"
#include <string>
#include <iostream>

// Auxiliary method to convert enum to string
std::string FacilityStatusToString(FacilityStatus status) {
    switch (status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS: return "UNDER CONSTRUCTIONS";
        case FacilityStatus::OPERATIONAL: return "OPERATIONAL";
        default: return "UNKNOWN";
    }
}

// Auxiliary method to convert enum to string
std::string FacilityCategoryToString(FacilityCategory category) {
    switch (category) {
        case FacilityCategory::LIFE_QUALITY: return "LIFE QUALITY";
        case FacilityCategory::ECONOMY: return "ECONOMY";
        case FacilityCategory::ENVIRONMENT: return "ENVIRONMENT";
        default: return "UNKNOWN";
    }
}

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, 
                           const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), 
      economy_score(economy_score), environment_score(environment_score) {}

const string &FacilityType::getName() const {
     return name;
}



int FacilityType::getCost() const {
    return price;
}

int FacilityType::getLifeQualityScore() const {
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const {
    return environment_score;
}

int FacilityType::getEconomyScore() const {
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const {
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, 
                   const int price, const int lifeQuality_score, const int economy_score, 
                   const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), 
      settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), 
      status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()) {}

const string & Facility::getSettlementName() const{
    return settlementName;
}

const int Facility::getTimeLeft() const{
    return timeLeft;
}

FacilityStatus Facility::step() {
    timeLeft--;
    if (timeLeft == 0) {
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return status;
}

void Facility::setStatus(FacilityStatus status) {
    this->status = status;
}

const FacilityStatus& Facility::getStatus() const {
    return status;
}

const string Facility::toString() const {
    string toString = "Facility name: " + name + "\n"
                      + "Facility category: " + FacilityCategoryToString(category) + "\n"
                      + "Facility price: " + std::to_string(price) + "\n"
                      + "Facility life quality score: " + std::to_string(lifeQuality_score) + "\n"
                      + "Facility life economy score: " + std::to_string(economy_score) + "\n"
                      + "Facility life environment score: " + std::to_string(environment_score) + "\n"
                      + "Facility settlement name: " + settlementName + "\n"
                      + "Facility status: " + FacilityStatusToString(status) + "\n"
                      + "Facility time left: " + std::to_string(timeLeft);
    return toString;
}
