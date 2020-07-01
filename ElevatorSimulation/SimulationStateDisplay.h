#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include "ElevatorState.h"
#include "SimState.h"
#include "ElevatorShaft.h"
#include <cmath>

class SimulationStateDisplay
{
	public:
		SimulationStateDisplay(Elevator::SimulationSettings settings);
		void displayState(Elevator::SimulationState simulationState);
		void refreshDisplay(Elevator::SimulationState simulationState);


	private: 
		const Elevator::SimulationSettings simulationSettings;
		size_t lastDisplayRowCount;
	
};

