#include "Plan.h"
#include <iostream>
#include <sstream>
#include <utility> // For std::move

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {}


Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy->clone()), // Initialize to nullptr to safely manage memory
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {

    // Deep copy facilities
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    // Deep copy underConstruction
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}


Plan::Plan(const Plan &other, const Settlement &newSettlement)
    : plan_id(other.plan_id),
      settlement(newSettlement), // Assign the new settlement
      selectionPolicy(other.selectionPolicy->clone()),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {
    // Deep copy facilities
    for (Facility* facility : other.facilities) {
        Facility* f = new Facility(*facility);
        facilities.push_back(f);
    }
    for (Facility* facility : other.underConstruction) {
        Facility* f = new Facility(*facility);
        underConstruction.push_back(f);
    }
}


// Copy assignment operator
Plan& Plan::operator=(const Plan &other) {
    if (this != &other) {
        // Clean up existing resources
        delete selectionPolicy;
        for (const Facility* facility : facilities) {
            delete facility;
        }
        facilities.clear();
        for (const Facility* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();
        // Do not reassign settlement; it is immutable
        selectionPolicy = other.selectionPolicy->clone();
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;

        for (Facility* facility : other.facilities) {
            facilities.push_back(new Facility(*facility));
        }
        
        for (Facility* facility : other.underConstruction) {
            underConstruction.push_back(new Facility(*facility));
        }
    }
    return *this;
}



// Destructor
Plan::~Plan() {
    delete selectionPolicy; // Free dynamically allocated selection policy
    for (Facility* facility : facilities) {
        delete facility; // Free dynamically allocated facilities
    }
    facilities.clear();
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();
}

// Methods
const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *newPolicy) {
    delete this->selectionPolicy;
    this->selectionPolicy = newPolicy;
}

const SelectionPolicy* Plan:: getSelectionPolicy() const{
    return selectionPolicy;
}

const Settlement& Plan:: getSettlement() const{
    return settlement;
}

void Plan::step() {

    // Check if the plan should be BUSY or AVAILABLE
    if (underConstruction.size() >= static_cast<size_t>(settlement.getConstructionLimit())) {
        status = PlanStatus::BUSY;
    } else {
        status = PlanStatus::AVALIABLE;
    }

    // Add new facilities if AVAILABLE and within limits
    if (status == PlanStatus::AVALIABLE) 
    {
        while (underConstruction.size() < static_cast<size_t>(settlement.getConstructionLimit())) 
        {
            const FacilityType& selectedFacilityType = selectionPolicy->selectFacility(facilityOptions);
            Facility* f = new Facility(selectedFacilityType, settlement.getName());
            underConstruction.push_back(f);
        }
    }

    // Process facilities under construction
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        Facility* facility = *it;
        FacilityStatus status = facility->step(); // Decrement time left
        if (status == FacilityStatus::OPERATIONAL) {
            facilities.push_back(facility); // Move to operational facilities
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
            it = underConstruction.erase(it); // Remove from underConstruction
        } else {
            ++it;
        }
    }
    
    // Re-check status after adding facilities
    if (underConstruction.size() >= static_cast<size_t>(settlement.getConstructionLimit())) {
        status = PlanStatus::BUSY;
    } else {
        status = PlanStatus::AVALIABLE;
    }
}


void Plan::printStatus() {
    std::cout << toString() << std::endl;
}

const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}

const vector<Facility*> &Plan::getUnderConstructionFacilities() const {
    return underConstruction;
}



void Plan::addFacility(Facility* facility) {
    underConstruction.push_back(facility);
}


void Plan::addUnderConstructionFacility(Facility* facility) {
    underConstruction.push_back(facility);
}

const std::string Plan::toString() const {
    std::ostringstream result;
    result << "PlanID: " << plan_id << "\n";
    result << "SettlementName: " << settlement.getName() << "\n";
    result << "PlanStatus: " << (status == PlanStatus::AVALIABLE ? "AVAILABLE" : "BUSY") << "\n";
    result << "SelectionPolicy: " << (selectionPolicy ? selectionPolicy->toString() : "None") << "\n";
    result << "LifeQualityScore: " << life_quality_score << "\n";
    result << "EconomyScore: " << economy_score << "\n";
    result << "EnvironmentScore: " << environment_score << "\n";

    // Print facilities under construction
    for (const Facility* facility : underConstruction) {
        result << "FacilityName: " << facility->getName() << "\n";
        result << "FacilityStatus: UNDER_CONSTRUCTION" << "\n";
    }

    // print existing facilities
    for (const Facility* facility : facilities) {
        result << "FacilityName: " << facility->getName() << "\n";
        result << "FacilityStatus: OPERATIONAL" << "\n";
    }

    return result.str();
}

const std::string Plan::shortenedToString() const{
    std::ostringstream result;
    result << "PlanID: " << plan_id << "\n";
    result << "SettlementName: " << settlement.getName() << "\n";
    result << "LifeQualityScore: " << life_quality_score << "\n";
    result << "EconomyScore: " << economy_score << "\n";
    result << "EnvironmentScore: " << environment_score << "\n";

    return result.str();
}

void Plan::setScores(int lifeQualityScore, int economyScore, int environmentScore) {
    life_quality_score = lifeQualityScore;
    economy_score = economyScore;
    environment_score = environmentScore;
}


const int Plan:: getPlanID() const{
    return plan_id;
}

void Plan::clearFacilities() {
    for (Facility* facility : facilities) {
        delete facility; // Free dynamically allocated memory
    }
    facilities.clear(); // Clear the vector
}

void Plan::clearUnderConstructionFacilities() {
    for (Facility* facility : underConstruction) {
        delete facility; // Free dynamically allocated memory
    }
    underConstruction.clear(); // Clear the vector
}
