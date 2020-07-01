#include "stdafx.h"
#include "SimulationStateDisplay.h"


#define SHAFT_DISPLAY_WIDTH 12
#define MIN_SHAFT_HEIGHT 6
#define SHAFT_NAME "Shaft"
#define CALLING_UP_STR "u"
#define CALLING_DOWN_STR "d"
#define ELEVATOR_WALL_STR "|"
#define ELEVATOR_SYMBOL_STR "[]"
#define STATUS_LABEL "Status: "
#define EMPTY_ROW_LARGE "           "
#define NON_SHAFT_HEIGHT 4


//Produces output for a given elevator shaft
/*
 Shaft 0
6:  d |[]|
5:    |  |
4:    |  |
3:   u|  |
2:    |  |
1:    |  |
Status: 
Going Up | Disabled | Going Down | Waiting

d = call for going down
u = call for going up
*/


SimulationStateDisplay::SimulationStateDisplay(Elevator::SimulationSettings settings) : simulationSettings(settings)
{
}


//Converts enum to a string nicely..
std::string getStatusDisplayString(Elevator::ElevatorState& elevatorState) {
	switch (elevatorState.movementStatus) {
	case Elevator::MovementStatus::Disabled:
		return "Disabled";
	case Elevator::MovementStatus::MovingDown:
		return "Moving Down";
	case Elevator::MovementStatus::MovingUp:
		return "Moving Up";
	case Elevator::MovementStatus::Waiting:
		return "Waiting";
	}
	return "ERROR";
}


//Pads the current string on the right side so that its length matches the target length
std::string padRight(const std::string& currentString, const size_t targetLength) {
	if (currentString.length() == targetLength)
	{
		return currentString;
	}

	assert(targetLength > currentString.length());
	std::string paddedString = currentString + std::string(targetLength - currentString.length(), ' ');
	assert(paddedString.length() == targetLength);
	return paddedString;
}

//Pads the current string on the left side so that its length matches the target length
std::string padLeft(const std::string& currentString, const size_t targetLength) {
	if (currentString.length() == targetLength)
	{
		return currentString;
	}
	assert(targetLength > currentString.length());
	std::string paddedString = std::string(targetLength - currentString.length(), ' ') + currentString;
	assert(paddedString.length() == targetLength);

	return paddedString;
}

//Centers a string, padding them with spaces on both sides. Currrent string length must be less than the targetWidth
std::string centerString(const std::string& currentString, const size_t targetLength){
	
	//If the string is already the correct length, it does not need to be padded
	if (currentString.length() == targetLength)
	{
		return currentString;
	}

	//The currrentString should be less than the targetWidth, (no logical way to shorten the string)
	assert(currentString.length() < targetLength);

	//Pad the left and right sides of the current string
	size_t totalPadding = targetLength - currentString.length();
	size_t halfPaddingLength = static_cast<size_t>(std::floor(totalPadding / 2));
	std::string halfPad(halfPaddingLength, ' ');
	std::string paddedString = halfPad + currentString + halfPad;

	//If the lengths are not equal, then the rounding from std::floor means that we have to have non equal padding on both sides.
	//Aka need to pad five characters, so pad 2 in the front and three after.
	if (paddedString.length() != targetLength) {
		paddedString += " ";
	}

	assert(paddedString.length() == targetLength);
	return paddedString;
}



//Produces a string for a row.
//For an example, floor 6 with with a down call, with the elevator is the following 
//6:  d | [] |
std::string getFloorDisplayString(Elevator::Floor& floor, const size_t totalFloorCount, size_t& minimumLength, bool displayElevator = false) {
	std::string floorString = std::to_string(floor.floorNumber + 1); //Marks which floor
	floorString = padRight(floorString, std::to_string(totalFloorCount).length()) + ": ";

	//status string for the elevator being called for up and down
	floorString += floor.isCallingForDown() ? CALLING_DOWN_STR : " ";
	floorString += floor.isCallingForUp() ? CALLING_UP_STR : " ";

	//Show the shaft and the elevator if needed
	floorString += ELEVATOR_WALL_STR;
	floorString += displayElevator ? ELEVATOR_SYMBOL_STR : "  ";
	floorString += ELEVATOR_WALL_STR;
	floorString += "  "; //default padding to aid in alignment
	
	return floorString;

}


//Returns an uncentered vector of strings representing an elevator shaft
//Updates the rowLength, so that padding is consistent across all rows
std::vector<std::string> getShaftDisplayRows(Elevator::ElevatorShaft& elevatorShaft, std::vector<Elevator::Floor> floorsVector, size_t& rowLength) {
	std::string shaftName = SHAFT_NAME + std::to_string(elevatorShaft.shaftNumber);

	//Handle the case that there are many shafts, and have longer names
	if (shaftName.length() > rowLength) {
		rowLength = shaftName.length();
	}

	std::vector<std::string> displayRows;
	displayRows.push_back(shaftName);

	assert(floorsVector.size() >= 2); //There must be at least two elevator rows
	
	//Build the elevator floor strings
	for (int i = floorsVector.size() -1; i >= 0; i--) {
		std::string floorString = getFloorDisplayString(floorsVector[i], floorsVector.size(), rowLength, i == elevatorShaft.getCurrentElevatorState().currentPosition);
		displayRows.push_back(floorString);
		
	}
	displayRows.push_back(STATUS_LABEL);
	displayRows.push_back(getStatusDisplayString(elevatorShaft.getCurrentElevatorState()));
	displayRows.push_back(EMPTY_ROW_LARGE);
	return displayRows;
}





//Displays the simulation state.
void SimulationStateDisplay::displayState(Elevator::SimulationState simulationState){
	std::vector<Elevator::ElevatorShaft> elevatorShafts = simulationState.elevatorShaftVector;
	
	//We accumulate the each shaft display vector into one master vector
	std::vector<std::string> cumulativeDisplayRows(NON_SHAFT_HEIGHT + simulationState.simulationSettings.numberOfFloors);
	std::fill(cumulativeDisplayRows.begin(), cumulativeDisplayRows.end(), "");

	size_t totalRowLength = 0;
	size_t totalCharacterOutput = 0;

	//Get the console size, to ensure that formatting is not completely broken by odd dimensions.
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo); //Used to get console dimensions
	
	//For each elevator shaft
	for (size_t i = 0; i < elevatorShafts.size(); i++) {
		
		size_t shaftDisplayLength = std::to_string(simulationState.simulationSettings.numberOfFloors).length() + SHAFT_DISPLAY_WIDTH;

		//Ensure that we do not attempt to display more shafts than what will fit on the console window
		if ((totalRowLength + (2 * shaftDisplayLength)) > static_cast<size_t>(consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left)) {
			std::string notificationString = " and " + std::to_string(elevatorShafts.size() - i + 1) + " more.";
			cumulativeDisplayRows[0] += notificationString;
			totalCharacterOutput += notificationString.length();
			break; 
		}


		//Get the display vector for a given shaft
		std::vector<std::string> elevatorShaftRows = getShaftDisplayRows(elevatorShafts[i], simulationState.floorsVector, shaftDisplayLength);

		assert(cumulativeDisplayRows.size() == elevatorShaftRows.size());

		//Add the diplay vector onto the cumulative vector
		for (size_t j = 0; j < cumulativeDisplayRows.size(); j++) {
			std::string centerShaftRow = centerString(elevatorShaftRows[j], shaftDisplayLength);
			
			cumulativeDisplayRows[j] += centerShaftRow;

			totalCharacterOutput += centerShaftRow.length();
			assert(centerShaftRow.length() == shaftDisplayLength);
		}
		totalRowLength += shaftDisplayLength;
	}


	//Output the cumulative vector
	for (size_t i = 0; i < cumulativeDisplayRows.size(); i++) {
		
		std::cout << cumulativeDisplayRows[i] << std::endl;
	}

	lastDisplayRowCount = cumulativeDisplayRows.size();
	
}	

//Move the cursor back to the top left, and overwrite the current display
void SimulationStateDisplay::refreshDisplay(Elevator::SimulationState simulationState) {
	COORD cursorCoordinate;
	cursorCoordinate.X = 0;
	cursorCoordinate.Y = 0;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!SetConsoleCursorPosition(consoleHandle, cursorCoordinate))
	{
		std::cout << "Unable to set cursor position: " << GetLastError() << ". Printing state on new lines." << std::endl;
	}

	displayState(simulationState);
}








