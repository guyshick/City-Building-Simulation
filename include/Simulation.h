#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
public:
    // Default Constructor
    Simulation(const string &configFilePath);

    // Rule of 5
    ~Simulation();                                  // Destructor
    Simulation(const Simulation &other);           // Copy Constructor
    Simulation(Simulation &&other) noexcept;       // Move Constructor
    Simulation &operator=(const Simulation &other); // Copy Assignment Operator
    Simulation &operator=(Simulation &&other) noexcept; // Move Assignment Operator

    // Public Methods
    void start();
    void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
    void addAction(BaseAction *action);
    bool addSettlement(Settlement *settlement);
    bool addFacility(FacilityType facility);
    bool isSettlementExists(const string &settlementName);
    Settlement &getSettlement(const string &settlementName);
    Plan &getPlan(const int planID);
    bool planExists(const int planID);
    bool isFacilityExist(const string &facilityName);
    vector<Plan>& getPlans();
    void step();
    void close();
    void open();
    const std::vector<BaseAction *> &getActionsLog() const;
    void backUp(); // Create a backup of the current simulation state
    void restore();

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
    vector<Plan> plans;
};


