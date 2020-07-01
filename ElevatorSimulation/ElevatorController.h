#pragma once
#include "ElevatorState.h"
#include "SimState.h"
#include "SimulationStateDisplay.h"
#include <thread>
#include <chrono>

namespace Elevator {

	//Class handles inter-elevator shaft logic
	//And updates the view when the state changes
	//This is the primary controller, with the remaining logic in the ElevatorShaft class.
	class ElevatorController
	{
	public:
		ElevatorController(SimulationSettings settings);
		~ElevatorController();
		void callElevator(int floor, MovementDirection direction);	//Calls an elevator to a given floor, based on the direction that the passenger intends to travel
		void requestFloor(int shaft, int floorNumber);		//Requests that a specific elevator travels to a specific floor
		SimulationState getCurrentState() const;			//Returns the current simulation state.
		void simulationTick();								//Simulates the passage of time. This simulation moves the elevators at a pace of one floor per tick
		void simulationTick(size_t numberOfTicks);			//Simulates multiple ticks, with a short wait period in between ticks
		SimulationStateDisplay simulationStateDisplay;		//Used to display the current state in a windows console window

		bool isValidFloorNumber(int floorNumber) const;		//Utility method for checking if a given floor number is valid. Returns true if valid.
		bool isValidShaftNumber(int shaftNumber) const;		//Utility method for checking if a given shaft number is valid. Returns true if valid.

	private:
		SimulationState currentState;						//Only the controller should be able to modify the simulation state. 
		
	};

	inline SimulationState ElevatorController::getCurrentState() const {
		return currentState;
	}
}



