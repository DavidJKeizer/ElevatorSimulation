#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "ElevatorState.h"
#include "ElevatorController.h"
#include <Windows.h>

class SimulationInput
{
	public:
		SimulationInput(Elevator::ElevatorController* elevatorControllerPtr);
		~SimulationInput();

		void enterInputLoop();
		static bool parseInt(std::istringstream& inStringStream, int& value);
	private:
		bool parseAndExecuteCommand(std::istringstream& inStringStream, std::string& input, std::string& command);
		void invalidCommand(std::string& command);
		Elevator::ElevatorController* elevatorControllerPtr;
		void clearInput();
		COORD startingInputCoordinate;
};


