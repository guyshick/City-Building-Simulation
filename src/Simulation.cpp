#include "Simulation.h"
#include "Auxiliary.h"
#include "Action.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <stdexcept>

// Global Backup Variable
extern Simulation *backup;

// Constructor
Simulation::Simulation(const string &configFilePath)
    : isRunning(false), planCounter(0), actionsLog(), settlements(), facilitiesOptions(), plans() {

    std::ifstream configFile(configFilePath); 
    if (!configFile.is_open()) {
        throw std::runtime_error("Could not open config file: " + configFilePath);
    }

    string codeLine;
    while (getline(configFile, codeLine)) {
        if (!codeLine.empty() && codeLine[0] != '#') {
            vector<string> inputs = Auxiliary::parseArguments(codeLine);

            if (!inputs.empty()) {
                if (inputs[0] == "settlement") {
                    int categorynum = std::stoi(inputs[2]);
                    SettlementType type;
                    if (categorynum == 0) {
                        type = SettlementType::VILLAGE;
                    } else if (categorynum == 1) {
                        type = SettlementType::CITY;
                    } else {
                        type = SettlementType::METROPOLIS;
                    }

                    string name = inputs[1];
                    // settlements.push_back(new Settlement(name, type));
                    addSettlement(new Settlement(name,type));
                } 
                else if (inputs[0] == "facility") {
                    string name = inputs[1];
                    int categorynum = std::stoi(inputs[2]);
                    FacilityCategory category;

                    if (categorynum == 0) {
                        category = FacilityCategory::LIFE_QUALITY;
                    } else if (categorynum == 1) {
                        category = FacilityCategory::ECONOMY;
                    } else {
                        category = FacilityCategory::ENVIRONMENT;
                    }

                    int price = std::stoi(inputs[3]);
                    int lifeQuality = std::stoi(inputs[4]);
                    int economy = std::stoi(inputs[5]);
                    int environment = std::stoi(inputs[6]);

                    facilitiesOptions.push_back(FacilityType(name, category, price, lifeQuality, economy, environment));
                } 
                else if (inputs[0] == "plan") {
                    string Settlement_name = inputs[1];
                    string policy = inputs[2];
                    SelectionPolicy* selectionPolicy = nullptr;
                    if (inputs[2] == "nve") {
                        selectionPolicy = new NaiveSelection();
                    } else if (inputs[2] == "bal") {
                        selectionPolicy = new BalancedSelection(0, 0, 0); 
                    } else if (inputs[2] == "eco") {
                        selectionPolicy = new EconomySelection();
                    } else {
                        selectionPolicy = new SustainabilitySelection();
                    }

                    addPlan(getSettlement(Settlement_name),selectionPolicy);
                }
            }
        }
    }
    configFile.close();
}

// Destructor
Simulation::~Simulation() {
    for (BaseAction *action : actionsLog) {
        delete action;
    }
    actionsLog.clear();
    for (Settlement *settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
}


// Copy Constructor
Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(),              
      settlements(),        
      facilitiesOptions(), 
      plans()
       { 


        // Deep copy actionsLog
        for (BaseAction *action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        // Deep copy settlements
        for (Settlement *settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }

        for (FacilityType facility : other.facilitiesOptions) {
            facilitiesOptions.push_back(facility);
        }

        for(const Plan &plan : other.plans){
        plans.push_back(Plan(plan,getSettlement(plan.getSettlement().getName())));
        }
        
    }




// Move Constructor
Simulation::Simulation(Simulation &&other) noexcept
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(std::move(other.actionsLog)),
      settlements(std::move(other.settlements)),
      facilitiesOptions(std::move(other.facilitiesOptions)),
      plans(std::move(other.plans)) {

    other.isRunning = false;
    other.planCounter = 0;
}




Simulation& Simulation::operator=(const Simulation& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    // Clean up existing data
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    facilitiesOptions.clear();
    for (const FacilityType& facility : other.facilitiesOptions) {
        facilitiesOptions.push_back(FacilityType(facility)); // Store pointer to facility
    }

    plans.clear();
    for(const Plan &plan : other.plans){
        plans.push_back(Plan(plan,getSettlement(plan.getSettlement().getName())));
    }

    return *this;
}



// Move Assignment Operator
Simulation &Simulation::operator=(Simulation &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    for (BaseAction *action : actionsLog) {
        delete action;
    }
    for (Settlement *settlement : settlements) {
        delete settlement;
    }

    actionsLog.clear();
    plans.clear();
    settlements.clear();
    facilitiesOptions.clear();

    isRunning = other.isRunning;
    planCounter = other.planCounter;
    actionsLog = std::move(other.actionsLog);
    settlements = std::move(other.settlements);
    facilitiesOptions = std::move(other.facilitiesOptions);
    plans = std::move(other.plans);

    other.isRunning = false;
    other.planCounter = 0;

    return *this;
}




void Simulation::start() {
    isRunning = true; // Update the state of the simulation
    open();
    std::cout << "The simulation has started" << std::endl;

    while (true) {
        std::cout << ">";
        std::string input;
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        try {
            if (command == "step") {
                int steps;
                iss >> steps;
                if (iss.fail() || steps <= 0) {
                    throw std::runtime_error("Invalid input: steps must be a positive integer.");
                }
                BaseAction *action = new SimulateStep(steps);
                action->act(*this);
                addAction(action);
            } else if (command == "plan") {
                std::string settlementName, selectionPolicy;
                iss >> settlementName >> selectionPolicy;
                if (settlementName.empty() || !isSettlementExists(settlementName)) {
                    throw std::runtime_error("Cannot create this plan");
                }
                BaseAction *action = new AddPlan(settlementName, selectionPolicy);
                action->act(*this);
                addAction(action);
            } else if (command == "settlement") {
                std::string settlementName;
                int settlementTypeInt;
                iss >> settlementName >> settlementTypeInt;
                if (settlementName.empty() || iss.fail()) {
                    throw std::runtime_error("missing or invalid arguments for settlement.");
                }
                if(isSettlementExists(settlementName)){
                    throw std::runtime_error("Settlement already exists");
                }
                SettlementType settlementType = static_cast<SettlementType>(settlementTypeInt);
                BaseAction *action = new AddSettlement(settlementName, settlementType);
                action->act(*this);
                addAction(action);

            } else if (command == "facility") {
                std::string facilityName;
                int price, category, lifeQualityScore, economyScore, environmentScore;
                iss >> facilityName >> category >> price >> lifeQualityScore >> economyScore >> environmentScore;
                if (facilityName.empty() || category < 0 || category > 2 || price < 0 || lifeQualityScore < 0 || economyScore < 0 || environmentScore < 0) {
                    throw std::runtime_error("invalid arguments for facility.");
                }
                if(isFacilityExist(facilityName)){
                    throw std::runtime_error("Facility already exists");
                }
                FacilityCategory facilityCategory = static_cast<FacilityCategory>(category);
                BaseAction *action = new AddFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
                action->act(*this);
                addAction(action);

            } else if (command == "planStatus") {
                int planID;
                iss >> planID;
                if (!planExists(planID)) {
                    throw std::runtime_error("Plan doesn't exist");
                }
                BaseAction *action = new PrintPlanStatus(planID);
                action->act(*this);
                addAction(action);
            } else if (command == "changePolicy") {
                int planID;
                std::string selectionPolicy;
                iss >> planID >> selectionPolicy;
                if (!planExists(planID) || !(selectionPolicy == "nve" || selectionPolicy == "bal" || selectionPolicy == "eco" || selectionPolicy == "env")) {
                    throw std::runtime_error("invalid arguments for changePolicy");
                }
                if(getPlan(planID).getSelectionPolicy()->toString() == selectionPolicy){
                    throw std::runtime_error("Cannot change selection policy");
                }
                BaseAction *action = new ChangePlanPolicy(planID, selectionPolicy);
                action->act(*this);
                addAction(action);
            } else if (command == "log") {
                BaseAction *action = new PrintActionsLog();
                action->act(*this);
                addAction(action);
            } else if (command == "close") {
                BaseAction *action = new Close();
                action->act(*this);
                addAction(action);
                break; // Exit the loop to terminate the simulation
            } else if (command == "backup") {
                BaseAction *action = new BackupSimulation();
                action->act(*this);
                addAction(action);
            } else if (command == "restore") {
                if(backup == nullptr){
                    throw std::runtime_error("No backup available");
                }
                BaseAction *action = new RestoreSimulation();
                action->act(*this);
                addAction(action);
            } else {
                std::cout << "Unknown command: " << command << std::endl;
            }
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}
            

void Simulation:: addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    // Construct the Plan object
    Plan newPlan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    planCounter ++;
    // Add the Plan to the vector
    plans.push_back(newPlan);
}

void Simulation::addAction(BaseAction *action) {
    if (!action) {
        throw std::runtime_error("Null action cannot be added.");
    }
    actionsLog.push_back(action); // Store pointer for later cleanup
}

bool Simulation:: addSettlement(Settlement *settlement){
    settlements.push_back(settlement);
    return true;
}

bool Simulation:: addFacility(FacilityType facility){
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    // Iterate through the settlements vector
    for (Settlement* settlement : settlements) {
        if (settlement->getName() == settlementName) { // Assuming Settlement has a getName() method
            return true; // Settlement found
        }
    }
    return false; // Settlement not found
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    for (Settlement* settlement : settlements) {
        if (settlement->getName() == settlementName) { // Assuming Settlement has a getName() method
            return *settlement; // Dereference the pointer and return a reference
        }
    }
    // If no settlement is found, throw an exception
    throw std::runtime_error("Settlement not found: " + settlementName);
}

Plan &Simulation::getPlan(const int planID){
    for (Plan &plan : plans) { // Use reference to avoid copying
        if (plan.getPlanID() == planID) {
            return plan;
        }
    }
    // If no plan is found, throw an exception
    throw std::runtime_error("Plan not found");
}

bool Simulation::planExists(const int planID){
    for (Plan &plan : plans) {
        if (plan.getPlanID() == planID) { // Assuming Settlement has a getName() method
            return true; // Settlement found
        }
    }
    return false;
}

bool Simulation::isFacilityExist(const string &facilityName){
    for(FacilityType facility : facilitiesOptions){
        if(facility.getName() == facilityName)
            return true;
    }
    return false;
}

std::vector<Plan>& Simulation::getPlans() {
    return plans;
}

void Simulation:: step(){
    for (Plan &plan : plans) {
        plan.step();
    }
}

void Simulation:: close(){
    for (Plan &plan : plans) {
        std::cout << plan.shortenedToString() << std::endl;
    }
    isRunning = false;
}

void Simulation:: open(){
    isRunning = true;
}

const std::vector<BaseAction *> &Simulation::getActionsLog() const {
    return actionsLog;
}

void Simulation::backUp() {
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }

    backup = new Simulation(*this); // Deep copy the current state
}

void Simulation::restore() {
    if (backup == nullptr) {
        throw std::runtime_error("No backup exists to restore from.");
    }

    // Use the copy assignment operator to copy the backup state
    *this = *backup;
}

