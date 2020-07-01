#pragma once
#include <queue>

namespace Elevator {

	//Defines common enums and types for use across the simulation
	enum class MovementStatus {MovingUp = 0, MovingDown = 1, Disabled, Waiting};
	enum class MovementDirection {Up = 0, Down = 1};

	struct ElevatorState {
		MovementStatus movementStatus; //Is the elevator moving? If so, what direction. 
		std::priority_queue<int> floorsAbovePriorityQueue;  //When moving up, which floor is next?
		std::priority_queue<int> floorsBelowPriorityQueue;
		int currentPosition;
	};

	class SimulationSettings {
		public:
			int numberOfFloors;
			int numberOfShafts;
	};


}

