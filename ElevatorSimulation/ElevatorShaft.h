#pragma once
#include "ElevatorState.h"
#include "CallButton.h"
#include <assert.h>
#include <stdio.h>
#include "Floor.h"
#include <queue>
#include <algorithm>


namespace Elevator {

	//Handles logic and updates the state for an individual elevator
	class ElevatorShaft {
		public:
			ElevatorShaft(int shaftNumber, int numberOfFloors);
			const int shaftNumber;
			ElevatorState getCurrentElevatorState() const;		//Returns the current state of the elevator (for display usage)
			void setMovementStatus(MovementStatus status);		//Overrides the movement status of the elevator
			void enable();										//Unused, enables and disables elevator input
			void disable();
			MovementStatus changeDirection();					//Changes which direction the elevator is moving
						
			bool gotoNextFloorInQueue();						//Moves the elevator to the next floor, based on its queue
			MovementStatus updateCurrentStatus();				//Calculates the Movement status of the elevator
			MovementStatus getCurrentMovementStatus()const;		
			bool hasFloorsInCurrentDirectionQueue()const;		//Returns true if there are more floors it needs to visit along its travelling direction
			int getNextFloorInQueue() const;					//Returns the floor where the elevator is going to next
			void removeNextFloorFromQueue();					//Removes the last floor from the priority queue
			void moveElevator();								//Moves the elevator up or down one floor, depending on its movement status
			void requestFloor(int floorNumber);					//Adds a floor to the priority queues
			int costToVisitFloor(int floorNumber);				//Estimates the cost to visit a floor (as a measure of floors)

		private:
			ElevatorState elevatorState;
			bool enabled;
			int clampFloor(int floorNumber);
			const int numberOfFloors;

	};

	//Inline member functions

	inline Elevator::ElevatorState Elevator::ElevatorShaft::getCurrentElevatorState() const {
		return elevatorState;
	}

	inline void Elevator::ElevatorShaft::setMovementStatus(MovementStatus status) {
		elevatorState.movementStatus = status;
	}

	inline MovementStatus Elevator::ElevatorShaft::getCurrentMovementStatus() const{
		return elevatorState.movementStatus;
	}

}

