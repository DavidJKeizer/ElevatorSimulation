#include "stdafx.h"
#include "Floor.h"

//Base constructor
Elevator::Floor::Floor(int floorNumber, bool isTopFloor, bool isBottomFloor) :
	floorNumber(floorNumber),
	callingDown(false),
	callingUp(false),
	isBottomFloor(isBottomFloor),
	isTopFloor(isTopFloor)
{}

//Updates the call button status for the floor
void Elevator::Floor::callElevator(Elevator::MovementDirection direction) {
	if (direction == MovementDirection::Down) {
		callingDown = true;
	}

	if (direction == MovementDirection::Up) {
		callingUp = true;
	}

	//If the floor is at the bottom, the elevator cannot go lower, and won't have a button for this.
	//We can ignore this call
	if (isBottomFloor) {
		callingDown = false;
	}

	//If the floor is at the top, the elevator cannot go higher, and won't have a button for this.
	//We can ignore this call
	if (isTopFloor) {
		callingUp = false;
	}

}

//Clear the call flags for a given direction
void Elevator::Floor::callMet(Elevator::MovementDirection direction) {

	if (direction == Elevator::MovementDirection::Down) {
		callingDown = false;
	}
	else if (direction == MovementDirection::Up) {
		callingUp = false;
	}
}

//Overload for clearing the call flags for a given direction. 
void Elevator::Floor::callMet(Elevator::MovementStatus elevatorStatus) {
	if (elevatorStatus == MovementStatus::MovingDown) {
		callingDown = false;
	}
	else if(elevatorStatus == MovementStatus::MovingUp){
		callingUp = false;
	}
	else {
		callingDown = false;
		callingUp = false;
	}

}