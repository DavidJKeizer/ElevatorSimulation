#include "stdafx.h"
#include "ElevatorShaft.h"


//Creates an elevator shaft, used for handling specific elevator logic
Elevator::ElevatorShaft::ElevatorShaft(int _shaftNumber, int numberOfFloors) : 
	shaftNumber(_shaftNumber),
	enabled(true),
	numberOfFloors(numberOfFloors)
{
	//An elevator must travel between at least two floors by definition.
	assert(numberOfFloors >= 2);

	//Default state is each elevator waiting at the bottom
	elevatorState.currentPosition = 0;
	elevatorState.movementStatus = Elevator::MovementStatus::Waiting;
}


//Due to poorly design reliance on enums, this hard coded method changes the movement status of the elevator based on it's traveling direction
Elevator::MovementStatus Elevator::ElevatorShaft::changeDirection() {
	if (!enabled)
		return MovementStatus::Disabled;

	if (elevatorState.movementStatus == MovementStatus::MovingDown) {
		elevatorState.movementStatus = MovementStatus::MovingUp;
	}
	else {
		elevatorState.movementStatus = MovementStatus::MovingDown;
	}

	return elevatorState.movementStatus;
}

//Enables the elevator, allowing input to it again.
void Elevator::ElevatorShaft::enable() {
	elevatorState.movementStatus = MovementStatus::Waiting;
	enabled = true;
}

//Disables the elevator, causing it to ignore all input
void Elevator::ElevatorShaft::disable() {
	elevatorState.movementStatus = MovementStatus::Disabled;
	enabled = true;
}

//Checks if the priority queue is empty based on the movement status of the elevator
bool Elevator::ElevatorShaft::hasFloorsInCurrentDirectionQueue() const{
	if (getCurrentMovementStatus() == MovementStatus::MovingUp) {
		return !elevatorState.floorsAbovePriorityQueue.empty();
	}
	return !elevatorState.floorsBelowPriorityQueue.empty();
}

//Calculates what the current status of the elevator is, based on the priority queue states.
Elevator::MovementStatus Elevator::ElevatorShaft::updateCurrentStatus() {
	if (!enabled)
		return MovementStatus::Disabled;

	MovementStatus currentStatus = getCurrentMovementStatus();

	size_t floorsAboveQueueSize = elevatorState.floorsAbovePriorityQueue.size();
	size_t floorsBelowQueueSize = elevatorState.floorsBelowPriorityQueue.size();

	//Both queues are empty, the elevator is waiting for a request
	if (floorsAboveQueueSize == 0 && floorsBelowQueueSize == 0) {
		setMovementStatus(MovementStatus::Waiting);
		return elevatorState.movementStatus;
	}

	//There is at least one floor in one of the queues
	//From waiting-> to either moving up or moving down
	if (currentStatus == MovementStatus::Waiting) {
		//We are no longer waiting, serve whichever queue is not empty
		currentStatus = floorsAboveQueueSize != 0 ? MovementStatus::MovingUp : MovementStatus::MovingDown;
		setMovementStatus(currentStatus);
		return elevatorState.movementStatus;
	}

	//Make no change, continue moving in the same direction
	if (hasFloorsInCurrentDirectionQueue()) {
		return elevatorState.movementStatus;
	}

	//Current queue is empty, but the other queue is not, change directions
	return changeDirection();
}

//Returns the next floor in the priority queues, based on which direction the elevator is moving
int Elevator::ElevatorShaft::getNextFloorInQueue() const {
	if (getCurrentMovementStatus() == MovementStatus::MovingUp) {	
		return numberOfFloors - elevatorState.floorsAbovePriorityQueue.top(); //When moving up, the priority is inverse of the floor number
	}
	if (getCurrentMovementStatus() == MovementStatus::MovingDown) {
		return elevatorState.floorsBelowPriorityQueue.top();
	}
	return 0;
}


//Removes the next floor in the priority queues, if there is one to pop.
void Elevator::ElevatorShaft::removeNextFloorFromQueue() {
	if (getCurrentMovementStatus() == MovementStatus::MovingUp) {

		if (elevatorState.floorsAbovePriorityQueue.empty())
			return;

		elevatorState.floorsAbovePriorityQueue.pop();
	}
	else if(getCurrentMovementStatus() == MovementStatus::MovingDown){
		if (elevatorState.floorsBelowPriorityQueue.empty())
			return;

		elevatorState.floorsBelowPriorityQueue.pop();
	}
}

//Clamps the floorNumber between 0 and the numberOfFloors.
int Elevator::ElevatorShaft::clampFloor(int floorNumber) {
	if (floorNumber > numberOfFloors) {
		return numberOfFloors;
	}

	if (floorNumber < 0) {
		return 0;
	}

	return floorNumber;
}

//Moves the elevator up or down by one floor, if the status is moving up or moving down
void Elevator::ElevatorShaft::moveElevator() {
	MovementStatus currentStatus = getCurrentMovementStatus();
	if (currentStatus == MovementStatus::MovingUp) {
		elevatorState.currentPosition = clampFloor(elevatorState.currentPosition + 1);
	}

	if (currentStatus == MovementStatus::MovingDown) {
		elevatorState.currentPosition = clampFloor(elevatorState.currentPosition - 1);
	}
}

//Assumes that the priority queue elements are unique
//Returns true if the elevator serviced the floor at it's current position.
bool Elevator::ElevatorShaft::gotoNextFloorInQueue() {
	if (!enabled)
		return false;

	//Updates the MovementStatus state value
	updateCurrentStatus();

	//If we are waiting, then we may have satisfied a call at the current floor.
	if (getCurrentMovementStatus() == MovementStatus::Waiting) {
		return true;
	}

	//We  are not waiting, where is the elevator off to next?
	int nextFloor = getNextFloorInQueue();

	//Elevator was supposed to go to it's now current floor.
	//This means it is responding to a request.
	bool servicedFloor = false;
	if (nextFloor == elevatorState.currentPosition) {
		removeNextFloorFromQueue();
		updateCurrentStatus(); //Recalculate the status as the queues have changed
		servicedFloor = true;
	}

	//Move the elevator based on the direction status
	moveElevator();	
	return servicedFloor;
}

//Takes a floor number, and add's it to the appropriate priority queue with an appropriate priority
void Elevator::ElevatorShaft::requestFloor(int floorNumber) {

	//Ignore requests to the current floor
	if (floorNumber == elevatorState.currentPosition) {
		return;
	}

	if (floorNumber > elevatorState.currentPosition) {
		elevatorState.floorsAbovePriorityQueue.push(numberOfFloors - floorNumber); //Lower floors above should have a higher priority
	}

	if (floorNumber < elevatorState.currentPosition) {
		elevatorState.floorsBelowPriorityQueue.push(floorNumber);
	}
}

//Estimates the cost it would take the elevator to visit a given floor based on it's priority queues
int Elevator::ElevatorShaft::costToVisitFloor(int floorNumber) {
	if (floorNumber == elevatorState.currentPosition) {
		return 0;
	}

	if (floorNumber > elevatorState.currentPosition) {
		return floorNumber - elevatorState.currentPosition + elevatorState.floorsAbovePriorityQueue.size();
	}

	return elevatorState.currentPosition - floorNumber + elevatorState.floorsBelowPriorityQueue.size();

	
}




