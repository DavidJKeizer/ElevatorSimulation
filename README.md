This project is an approximate  simulation of a series of an elevator controller.
Usage: $ElevatorSimulation [NumberOfFloors] [Number of Shafts]

When the program is running, commands can be given to control the simulation:

Exit										Exits the simulation.
Call [Floor Number] [Up|Down] 				Calls an elevator to a given floor, with the intention of going up or down.
RequestFloor [Shaft Number] [Floor Number]	Requests an elevator in a given shaft to go to a given floor. This is to simulate passenger input from inside the elevator.
Tick										Executes the simulation for one unit of time, moving elevators one floor.
Tick [Number of Ticks]						Tick repeatedly, with a one second delay between ticks.

Example command sequence:

Call 10 Down: 			Calls the elevator to meet a passenger on floor 10, wishing to go down.
Tick 10: 				Executes ten ticks, causing the elevator to ascend to floor 10.
RequestFloor 0 2:		The passenger in shaft 0, wishes to go to floor 2.
Tick 10:				Execute another 10 ticks, causing the elevator to descent to floor 2.