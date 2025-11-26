# City Building Simulation

## Overview
A comprehensive C++ simulation system designed to manage and evaluate reconstruction plans for settlements. The system simulates dynamic decision-making processes based on resource allocation, facility construction, and strategic prioritization.

This project focuses on Object-Oriented Design patterns, strict memory management, and algorithmic efficiency.

## Key Features
* **Dynamic Decision-Making Algorithms:** Implemented a polymorphic `SelectionPolicy` system to determine construction strategies:
    * **Naive Policy:** Sequential selection logic.
    * **Balanced Policy:** An algorithm that calculates and minimizes the delta between Life Quality, Economy, and Environmental scores to ensure balanced growth.
    * **Economy & Sustainability Policies:** Targeted strategies prioritizing specific facility categories.
* **Polymorphism & OOP:** Utilized inheritance and abstract base classes for flexible Action handling (`BaseAction`) and policy management.
* **Memory Management:** Enforced the **Rule of 5** (Copy Constructor, Move Constructor, Copy Assignment, Move Assignment, Destructor) to ensure resource safety and prevent memory leaks.
* **Simulation Mechanics:** managed multiple settlement types (Village, City, Metropolis) with varying construction limits and resource constraints.

## Technical Details
* **Language:** C++ (C++11 Standard)
* **Build System:** GNU Make
* **Verification:** Verified zero memory leaks using **Valgrind**.

## Simulation Flow
1. **Initialization:** Parsing configuration files to set up settlements and available facilities.
2. **Step Execution:** The simulation iterates through time steps, where each plan executes its strategy to select and build facilities.
3. **Resource Management:** Tracks construction queues, "Time Left" for projects, and operational status updates.

## How to Run
1. Compile the project using the Makefile:
   ```bash
   make
