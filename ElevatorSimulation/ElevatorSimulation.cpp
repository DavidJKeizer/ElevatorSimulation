// ElevatorSimulation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ElevatorState.h"
#include "SimulationStateDisplay.h"
#include "ElevatorShaft.h"
#include "ElevatorController.h"
#include <iostream>
#include <thread>
#include "SimulationInput.h"


#define ARG_COUNT 3
#define MINIMUM_FLOORS 2
#define MINIMUM_SHAFTS 1


void printUsageError() {
	std::cerr << "Usage: ElevatorSimulation [NumberOfFloors] [Number of Shafts]" <<std::endl;
}

int main(int argc, char** argv)
{
	//Check that enough arguments were supplied
	if (argc != ARG_COUNT) {
		printUsageError();
		exit(-1);
	}
	
	//Attempt to parse the program settings
	int numberOfFloors, numberOfShafts;
	try {
		numberOfFloors = std::stoi(argv[1]);
		numberOfShafts = std::stoi(argv[2]);
	}
	catch(std::exception const& exception){
		std::cerr << "Unable to parse string to integer value. ";
		printUsageError();
		exit(-1);
	}
	
	//Verify that the ranges are correct
	if (numberOfFloors < MINIMUM_FLOORS) {
		std::cerr << "The number of floors must be at least 2. ";
		printUsageError();
		exit(-1);
	}

	if (numberOfShafts < MINIMUM_SHAFTS) {
		std::cerr << "The number of elevator shafts must be at least 1. ";
		printUsageError();
		exit(-1);
	}

	//Define program settings
	Elevator::SimulationSettings simulationSettings;
	simulationSettings.numberOfFloors = numberOfFloors;
	simulationSettings.numberOfShafts = numberOfShafts;

	//Create the controller 
	Elevator::ElevatorController controller(simulationSettings);

	//Create the simulation input handler
	SimulationInput simulationInput(&controller);

	//Display the default state. This allows the display to be refreshed properly
	controller.simulationStateDisplay.displayState(controller.getCurrentState());
	
	//Enter the main input loop
	simulationInput.enterInputLoop();
	return 0;
}

