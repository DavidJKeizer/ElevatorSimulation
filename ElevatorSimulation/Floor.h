#pragma once

#include "CallButton.h"

namespace Elevator {
	//Stores basic state for each floor, most importantly the call status for each button
	class Floor {
		public:
			Floor(int floorNumber, bool isTopFloor, bool isBottomFloor);
			int floorNumber;
			void callElevator(Elevator::MovementDirection direction);
			bool isCallingForDown();
			bool isCallingForUp();
			void callMet(Elevator::MovementDirection direction);
			void callMet(Elevator::MovementStatus elevatorStatus);


	private:
		bool isTopFloor;
		bool isBottomFloor;
		bool callingDown;
		bool callingUp;
	};

	inline bool Floor::isCallingForDown() {
		return callingDown;
	}

	inline bool Floor::isCallingForUp() {
		return callingUp;
	}

}
