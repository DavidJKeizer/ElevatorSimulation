#pragma once
#include "ElevatorShaft.h"

namespace Elevator {
	//Core state for representing all of the floors and elevators
	//Includes for the simulation settings for ease of use
	struct SimulationState {
		std::vector<ElevatorShaft> elevatorShaftVector;
		std::vector<Floor>floorsVector;
		SimulationSettings simulationSettings;
	};
}
