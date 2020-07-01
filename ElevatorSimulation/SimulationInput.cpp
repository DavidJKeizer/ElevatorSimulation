#include "stdafx.h"
#include "SimulationInput.h"

//Define the command strings
#define EXIT_COMMAND "Exit"
#define CALL_COMMAND "Call"
#define CALL_DIRECTION_UP "Up"
#define CALL_DIRECTION_DOWN "Down"
#define REQUEST_FLOOR_COMMAND "RequestFloor"
#define TICK_COMMAND "Tick"

SimulationInput::SimulationInput(Elevator::ElevatorController* elevatorControllerPtr) : elevatorControllerPtr(elevatorControllerPtr)
{
}
SimulationInput::~SimulationInput()
{
}

//Parse the command direction of "Up" or "Down" into a MovementDirection, stored in direction if successfull.
//Returns true if successful.
bool commandStringToDirection(std::string& directionString, Elevator::MovementDirection& direction) {
	if (directionString == CALL_DIRECTION_UP) {
		direction = Elevator::MovementDirection::Up;
		return true;
	}


	if (directionString == CALL_DIRECTION_DOWN) {
		direction = Elevator::MovementDirection::Down;
		return true;
	}

	return false;
}

//Parse a string into a integer using an open string stream.
//Returns true if successful
bool SimulationInput::parseInt(std::istringstream& inStringStream, int& value) {
	inStringStream >> value;
	return !inStringStream.fail();
}

//Parses a given command, and attempts to parse the command arguments.
//If the command was parsed successfuly, it executes it with the arguments.
bool SimulationInput::parseAndExecuteCommand(std::istringstream& inStringStream, std::string& input, std::string& command) {
	//Exit command is handled by the main input loop. Alternative would be to have this method return a function pointer to the relevant action with args filled out.
	if (command == EXIT_COMMAND) {
		return true;
	}

	if (command == TICK_COMMAND) {
		//Default of 1 tick if no number is given
		if (inStringStream.rdbuf()->in_avail() == 0) {
			elevatorControllerPtr->simulationTick();
			return true;
		}

		//Parse how many ticks
		int tickCount;
		if (!parseInt(inStringStream, tickCount)) {
			return false;
		}

		//Command is valid
		elevatorControllerPtr->simulationTick(tickCount);
		return true;
	}

	if (command == CALL_COMMAND) {
		int floorNumber;
		std::string direction;
		if (!parseInt(inStringStream, floorNumber)) {
			return false;
		}

		//Convert the string into a movement direction
		inStringStream >> direction;
		Elevator::MovementDirection callDirection;
		if (!commandStringToDirection(direction, callDirection)) {
			return false;
		}

		//Decrease the floorNumber by 1, because ground is 1, not 0. For ease of use, internally the floors start at 0
		if (elevatorControllerPtr->isValidFloorNumber(floorNumber-1)) {
			elevatorControllerPtr->callElevator(floorNumber-1, callDirection);
			return true;
		}
		
		
	}


	if (command == REQUEST_FLOOR_COMMAND) {
		int shaftNumber, floorNumber;
		if (!parseInt(inStringStream, shaftNumber)) {
			return false;
		}

		if (!parseInt(inStringStream, floorNumber)) {
			return false;
		}
		
		//Decrease the floorNumber by 1, because ground is 1, not 0. For ease of use, internally the floors start at 0
		if (elevatorControllerPtr->isValidFloorNumber(floorNumber - 1)
			&& elevatorControllerPtr->isValidShaftNumber(shaftNumber)) {
			elevatorControllerPtr->requestFloor(shaftNumber, floorNumber-1);
			return true;
		}
		
		
	}
	return false;
}

//Based on https://stackoverflow.com/questions/35800020/how-do-i-find-the-coordinates-of-the-cursor-in-a-console-window;
//Returns the current coordinate of the console cursor
//Windows only.
COORD getCursorCoordinate() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO   consoleBufferInfo;
	if (GetConsoleScreenBufferInfo(consoleHandle, &consoleBufferInfo)) {
		return  consoleBufferInfo.dwCursorPosition;
	}
	std::cout << "Unable to get cursor position: " << GetLastError() << std::endl;
	return COORD();
}

//Sets the row of the console cursor.
//Windows only.
void setCursorCooridinateHeight(SHORT height) {
	COORD cursorCoordinate;
	cursorCoordinate.X = 0;
	cursorCoordinate.Y = height;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!SetConsoleCursorPosition(consoleHandle, cursorCoordinate))
	{
		std::cout << "Unable to set cursor position: " << GetLastError() << ". Printing state on new lines." << std::endl;
	}
}

//Main input loop. Handles commands, parses them, until the user exits.
void SimulationInput::enterInputLoop() {
	std::string command;

	//The starting coordinate is stored, as this marks the region that all further input should be displayed from
	startingInputCoordinate = getCursorCoordinate();

	while (command != EXIT_COMMAND) {
		
		std::cout << "Please input a simulation command: {Call, RequestFloor, Tick, or  Exit}" << std::endl;
		std::string input;
		std::getline(std::cin, input); //Get line so that we have multiple args
		std::istringstream inStringStream(input);

		std::string command;
		inStringStream >> command;

		if (command == EXIT_COMMAND) {
			break;
		}

		clearInput();
		if (!parseAndExecuteCommand(inStringStream, input, command)) {
			invalidCommand(command);
		}
		else {
			std::cout << "Command okay. ";
		}
	}

	
}

void SimulationInput::invalidCommand(std::string& command) {
	std::cout << "Invalid command: " << command << " Please enter again. ";
}


//from https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
//Returns how wide the console is, in characters. Returns 100 if that fails.
int getConsoleWidth() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		return csbi.srWindow.Right - csbi.srWindow.Left + 1;
	}
	//Console always has some width, so return a reasonable number
	return 100;
}

//Overwrite the last few lines written with spaces, so that the input region is cleared.
//This prevents text being printed on top of eachother.
void SimulationInput::clearInput() {
	COORD currentCursorCoordinate = getCursorCoordinate();
	SHORT heightDelta = currentCursorCoordinate.Y - startingInputCoordinate.Y; //How many lines to overwrite with spaces.
	setCursorCooridinateHeight(startingInputCoordinate.Y);
	for (SHORT i = 0; i < heightDelta; i++) {
		printf("%*s", getConsoleWidth(), ""); //based on https://stackoverflow.com/questions/14678948/how-to-repeat-a-char-using-printf
	}
	setCursorCooridinateHeight(startingInputCoordinate.Y);

}




