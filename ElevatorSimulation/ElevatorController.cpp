#include "stdafx.h"
#include "ElevatorController.h"

#define TICK_DURATION 1 //How long should the thread sleep between ticks


//Creates the default state, simulation display.
Elevator::ElevatorController::ElevatorController(SimulationSettings settings) :
	currentState(SimulationState()), 
	simulationStateDisplay(SimulationStateDisplay(settings))
{
	//Initialize the current state.

	//Start by creating the elevator shafts
	currentState.simulationSettings = settings;
	for (int i = 0; i < settings.numberOfShafts; i++) {
		ElevatorShaft newElevatorShaft(i, settings.numberOfFloors);
		currentState.elevatorShaftVector.push_back(newElevatorShaft);
	}

	//Create the floors, checking to see if we are adding the first and top floors.
	//These floors are special cases because they do not have call buttons for both up and down
	for (int i = 0; i < settings.numberOfFloors; i++) {
		currentState.floorsVector.push_back(Elevator::Floor(i, i == settings.numberOfFloors -1, i == 0));
	}


}

//Default Deconstrutor
Elevator::ElevatorController::~ElevatorController()
{
}

//Calls an elevator to a given floor, based on the direction that the passenger intends to travel
void Elevator::ElevatorController::callElevator(int floor, MovementDirection direction) {
	currentState.floorsVector[floor].callElevator(direction); //Update the model to reflect that an elevator has been called
	
	//We next need to select a shaft to assign this call to.
	//Linear search through each shaft to find which elevator has the lowest cost (as a measure of floors)
	int lowestCost = 2 * currentState.floorsVector.size(); //Start the value at a cost at a maximum value
	size_t lowestCostshaftIndex = 0;

	for (size_t i = 0; i < currentState.elevatorShaftVector.size(); i++) {
		int shaftCost = currentState.elevatorShaftVector[i].costToVisitFloor(floor);
		if (shaftCost < lowestCost) {
			lowestCostshaftIndex = i;
			lowestCost = shaftCost;
		}
	}

	//Assign the request to a chosen shaft.
	currentState.elevatorShaftVector[lowestCostshaftIndex].requestFloor(floor);
	simulationStateDisplay.refreshDisplay(currentState); //Update the view

}

//Requests that a specific elevator travels to a specific floor
//This is used to simulate button presses when inside the elevator
//As well as for when the controller is sending an elevator meet a call request
//Note that the floorNumber is used directly as the priority queue value
void Elevator::ElevatorController::requestFloor(int shaft, int floorNumber) {

	currentState.elevatorShaftVector[shaft].requestFloor(floorNumber); //Add the floor to the elevator queue, which updates the model
	
	ElevatorState elevatorState = currentState.elevatorShaftVector[shaft].getCurrentElevatorState();
	
	//The elevator should ignore requests to go to it's current position
	if (floorNumber == elevatorState.currentPosition) {
		return;
	}

	//Push the floor number onto the appropriate priority queue
	if (floorNumber > elevatorState.currentPosition) {
		elevatorState.floorsAbovePriorityQueue.push(floorNumber);
	}

	if (floorNumber < elevatorState.currentPosition) {
		elevatorState.floorsBelowPriorityQueue.push(floorNumber);
	}

	simulationStateDisplay.refreshDisplay(currentState); //refresh the view
	
}

//Simulates the passage of time. This simulation moves the elevators at a pace of one floor per tick
void Elevator::ElevatorController::simulationTick() {
	//Move the elevators according to their priority queues
	for (int i = 0; i < currentState.elevatorShaftVector.size(); i++) {
		
		//Get the current floor before the elevator moves. This is needed to track if a call was met or not.
		int currentFloor = currentState.elevatorShaftVector[i].getCurrentElevatorState().currentPosition;
		
		//Move the elevator to the next floor in it's queue(s)
		bool servicedFloor = currentState.elevatorShaftVector[i].gotoNextFloorInQueue();

		//If it has serviced a floor, then it means that is responding to a floor call
		//Update the state to reflect that
		if (servicedFloor) {
			MovementStatus movementStatus = currentState.elevatorShaftVector[i].getCurrentElevatorState().movementStatus;
			currentState.floorsVector[currentFloor].callMet(movementStatus);
		}
	}

	simulationStateDisplay.refreshDisplay(currentState); //refresh the view

}

//Simulates multiple ticks, with a short wait period in between ticks
void Elevator::ElevatorController::simulationTick(size_t numberOfTicks) {
	for (size_t i = 0; i < numberOfTicks; i++) {
		simulationTick();
		std::this_thread::sleep_for(std::chrono::seconds(TICK_DURATION));
	}
}

//Utility method for checking if a given floor number is valid. Returns true if valid.
bool Elevator::ElevatorController::isValidFloorNumber(int floorNumber) const {
	return (floorNumber >= 0 && floorNumber < currentState.simulationSettings.numberOfFloors);
}

//Utility method for checking if a given shaft number is valid. Returns true if valid.
bool Elevator::ElevatorController::isValidShaftNumber(int shaftNumber) const {
	return (shaftNumber >= 0 && shaftNumber < currentState.simulationSettings.numberOfShafts);
}
