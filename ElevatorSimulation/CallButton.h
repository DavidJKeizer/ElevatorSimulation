#pragma once
#include "ElevatorState.h"


namespace Elevator {
	//Stores the state for the call buttons.
	//Unused.
	class CallButton {
		public: 
			CallButton(int floor, int shaftNumber, MovementDirection callDirection);
			CallButton();

			const int getFloorNumber() const;
			const int getShaftNumber() const;
			const MovementDirection getCallDirection() const;
			
			bool isCalling() const;

			void setFloorNumber(int floorNumber);
			void setShaftNumber(int shaftNumber);
			void setCallDirection(MovementDirection callDirection);

		private:
			bool _isCalling;
			int _floorNumber;
			int _shaftNumber;
			MovementDirection _callDirection;
	};

	//Inline member functions
	inline bool Elevator::CallButton::isCalling()const {
		return _isCalling;
	}

	inline const int Elevator::CallButton::getFloorNumber() const {
		return _floorNumber;
	}

	inline const int Elevator::CallButton::getShaftNumber() const {
		return _shaftNumber;
	}

	inline const Elevator::MovementDirection Elevator::CallButton::getCallDirection() const {
		return _callDirection;
	}

	inline void Elevator::CallButton::setFloorNumber(int floorNumber) {
		_floorNumber = floorNumber;
	}

	inline void Elevator::CallButton::setShaftNumber(int shaftNumber) {
		_shaftNumber = shaftNumber;
	}

	inline void Elevator::CallButton::setCallDirection(Elevator::MovementDirection callDiretion) {
		_callDirection = callDiretion;
	}
}


