#pragma once
#include <vector>
#include <string>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"

using std::vector;
using std::string;

enum class PlanStatus {
    AVALIABLE, 
    BUSY,
};

class Plan {
public:
    // Constructor
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);

    // Copy constructor
    Plan(const Plan &other);
    Plan(const Plan &other, const Settlement& settlement);

    // Copy assignment operator
    Plan& operator=(const Plan &other);

    // Destructor
    ~Plan();

    // Public methods
    const int getlifeQualityScore() const;
    const int getEconomyScore() const;
    const int getEnvironmentScore() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    const SelectionPolicy* getSelectionPolicy() const;
    const Settlement& getSettlement() const;
    void step();
    void printStatus();
    const vector<Facility*> &getFacilities() const;
    const vector<Facility*> &getUnderConstructionFacilities() const;
    void addFacility(Facility* facility);
    void addUnderConstructionFacility(Facility* facility);
    const string toString() const;
    const string shortenedToString() const;
    void setScores(int lifeQualityScore, int economyScore, int environmentScore);
    const int getPlanID() const;
    void clearFacilities();
    void clearUnderConstructionFacilities();

private:
    int plan_id;
    const Settlement &settlement; // Reference to avoid deep copying
    SelectionPolicy *selectionPolicy; // Raw pointer to allow dynamic behavior
    PlanStatus status;
    vector<Facility*> facilities;
    vector<Facility*> underConstruction;
    const vector<FacilityType> &facilityOptions; // Reference for efficient handling
    int life_quality_score, economy_score, environment_score;
};
