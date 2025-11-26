#include "Action.h"
#include "Auxiliary.h"
#include "Simulation.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <stdexcept>

// BaseAction Implementation
BaseAction::BaseAction()
    : errorMsg("Error: <error_msg>"), status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
}

const std::string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

// SimulateStep Implementation
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; i++) {
        for(Plan& plan : simulation.getPlans()){
            plan.step();
        }
    }
    complete();
}

const std::string SimulateStep::toString() const {
    return "step " + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

// AddPlan Implementation
AddPlan::AddPlan(const std::string &settlementName, const std::string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    if (simulation.isSettlementExists(settlementName)) {
        if (selectionPolicy == "nve") {
            simulation.addPlan(simulation.getSettlement(settlementName), new NaiveSelection());
        } else if (selectionPolicy == "bal") {
            simulation.addPlan(simulation.getSettlement(settlementName), new BalancedSelection(0, 0, 0));
        } else if (selectionPolicy == "eco") {
            simulation.addPlan(simulation.getSettlement(settlementName), new EconomySelection());
        } else {
            simulation.addPlan(simulation.getSettlement(settlementName), new SustainabilitySelection());
        }
        complete();
    } else {
        error("Settlement " + settlementName + " does not exist.");
    }
}

const std::string AddPlan::toString() const {
    return "AddPlan: Added plan to settlement " + settlementName + " with policy " + selectionPolicy;
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

// AddSettlement Implementation
AddSettlement::AddSettlement(const std::string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    simulation.addSettlement(new Settlement(settlementName, settlementType));
    complete();
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

const std::string AddSettlement::toString() const {
    string type = "";

    // Add the type to the stringstream
    if (settlementType == SettlementType::VILLAGE) {
        type = "0";
    } else if (settlementType == SettlementType::CITY) {
        type = "1";
    } else {
        type = "2";
    }
    string toString = "settlement " + settlementName + " " + type;
    return toString;
}

// AddFacility Implementation
AddFacility::AddFacility(const std::string &facilityName, const FacilityCategory facilityCategory,
                         const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
    : facilityName(facilityName), facilityCategory(facilityCategory), price(price),
      lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}




void AddFacility::act(Simulation &simulation) {
    FacilityType facilityToAdd = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    simulation.addFacility(facilityToAdd); // No dynamic allocation
    complete();
}




AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}

const std::string AddFacility::toString() const {
    return "AddFacility: Added facility " + facilityName;
}



// PrintPlanStatus Implementation
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    simulation.getPlan(planId).printStatus();
    complete();
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const std::string PrintPlanStatus::toString() const {
    return "planStatus " + std::to_string(planId);
}



// ChangePlanPolicy Implementation
ChangePlanPolicy::ChangePlanPolicy(const int planId, const std::string &newPolicy)
    : planId(planId), newPolicy(newPolicy), oldPolicy(""){}

void ChangePlanPolicy::act(Simulation &simulation) {
    Plan &p = simulation.getPlan(planId);
    if(p.getSelectionPolicy()->toString() == newPolicy){
        error("illegal input");
    }
    oldPolicy = p.getSelectionPolicy()->toString();
    try {
        if (newPolicy == "nve") {
            p.setSelectionPolicy(new NaiveSelection());
        } else if (newPolicy == "bal") {
            p.setSelectionPolicy(new BalancedSelection(p.getlifeQualityScore(),p.getEconomyScore(),p.getEnvironmentScore()));
        } else if (newPolicy == "eco") {
            p.setSelectionPolicy(new EconomySelection());
        } else {
            p.setSelectionPolicy(new SustainabilitySelection());
        }
        complete();
    } catch (const std::exception &e) {
        error(e.what());
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const std::string ChangePlanPolicy::toString() const {
    std::ostringstream result;
    result << "PlanID: " << planId<< "\n";
    result << "previousPolicy: " << oldPolicy << "\n";
    result << "newPolicy: " << newPolicy << "\n";
    return result.str();
}

// PrintActionsLog Implementation
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    // Retrieve the list of actions from the simulation
    const auto &actionsLog = simulation.getActionsLog();

    // Iterate through the actions log and print each action
    for (const BaseAction *action : actionsLog) {
        std::cout << action->toString() << " ";
        
        // Print the status of the action
        if (action->getStatus() == ActionStatus::COMPLETED) {
            std::cout << "COMPLETED";
        } else {
            std::cout << "ERROR";
        }
        
        std::cout << std::endl;
    }
    complete(); // Mark this action as completed
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const std::string PrintActionsLog::toString() const {
    return "PrintActionsLog: Printed actions log.";
}

// Close Implementation
Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

Close *Close::clone() const {
    return new Close(*this);
}

const std::string Close::toString() const {
    return "Close: Closed the simulation.";
}

// BackupSimulation Implementation
BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    simulation.backUp();
    complete();
}

BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}

const std::string BackupSimulation::toString() const {
    return "backup";
}

// RestoreSimulation Implementation
RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    simulation.restore();
    complete();
}

RestoreSimulation *RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}

const std::string RestoreSimulation::toString() const {
    return "RestoreSimulation: Restored from backup.";
}
