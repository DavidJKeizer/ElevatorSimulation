#include "stdafx.h"
#include "CallButton.h"

Elevator::CallButton::CallButton():
	_isCalling(false),
	_floorNumber(0),
	_shaftNumber(0),
	_callDirection(Elevator::MovementDirection::Down)
{}

Elevator::CallButton::CallButton(int _floor, int _shaftNumber, MovementDirection _callDirection) :
	_floorNumber(_floor),
	_shaftNumber(_shaftNumber),
	_callDirection(_callDirection)
{

}


