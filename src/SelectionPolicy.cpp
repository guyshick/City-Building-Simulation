#include "SelectionPolicy.h"
#include <sstream>
#include <limits>
#include <iostream>

// NaiveSelection Constructor
NaiveSelection::NaiveSelection() 
: lastSelectedIndex(-1) {}

// NaiveSelection selectFacility Implementation
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

// NaiveSelection toString Implementation
const string NaiveSelection::toString() const {
    return "nve";
}

// NaiveSelection clone Implementation
NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

// BalancedSelection Constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

// BalancedSelection selectFacility Implementation
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for Balanced Selection.");
    }
    
    int bestScore = std::numeric_limits<int>::max();
    size_t bestIndex = -1;

    for (size_t i = 0; i < facilitiesOptions.size(); ++i) {
        const FacilityType& facility = facilitiesOptions[i];

        // Calculate the maximum distance between scores
        int distance1 = std::abs(facility.getLifeQualityScore() + LifeQualityScore - facility.getEnvironmentScore() - EnvironmentScore);
        int distance2 = std::abs(facility.getLifeQualityScore() + LifeQualityScore - facility.getEconomyScore() - EconomyScore);
        int distance3 = std::abs(facility.getEnvironmentScore() + EnvironmentScore - facility.getEconomyScore() - EconomyScore);

        int maxDistance = std::max(distance1, std::max(distance2, distance3));

        // Select the facility with the lowest max distance
        if (maxDistance < bestScore) {
            bestScore = maxDistance;
            bestIndex = i;
        }
    }
    LifeQualityScore += facilitiesOptions[bestIndex].getLifeQualityScore();
    EconomyScore += facilitiesOptions[bestIndex].getEconomyScore();
    EnvironmentScore += facilitiesOptions[bestIndex].getEnvironmentScore();
    return facilitiesOptions[bestIndex];
}

// BalancedSelection toString Implementation
const string BalancedSelection::toString() const {
    return "bal";
}

// BalancedSelection clone Implementation
BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

// EconomySelection Constructor
EconomySelection::EconomySelection()
: lastSelectedIndex(-1) {}

// EconomySelection selectFacility Implementation
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for Economy Selection.");
    }

    for (size_t i = 0; i < facilitiesOptions.size(); ++i) {
        size_t index = (lastSelectedIndex + 1 + i) % facilitiesOptions.size();
        const FacilityType& facility = facilitiesOptions[index];
        if (facility.getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = index;
            return facility;
        }
    }

    throw std::runtime_error("No matching facility found for Economy Selection.");
}

// EconomySelection toString Implementation
const string EconomySelection::toString() const {
    return "eco";
}

// EconomySelection clone Implementation
EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}

// SustainabilitySelection Constructor
SustainabilitySelection::SustainabilitySelection() 
: lastSelectedIndex(-1) {}

// SustainabilitySelection selectFacility Implementation
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for Sustainability Selection.");
    }

    for (size_t i = 0; i < facilitiesOptions.size(); ++i) {
        size_t index = (lastSelectedIndex + 1 + i) % facilitiesOptions.size();
        const FacilityType& facility = facilitiesOptions[index];
        if (facility.getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = index;
            return facility;
        }
    }

    throw std::runtime_error("No matching facility found for Sustainability Selection.");
}

// SustainabilitySelection toString Implementation
const string SustainabilitySelection::toString() const {
    return "env";
}

// SustainabilitySelection clone Implementation
SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
