# Marvin (The Paranoid Android)
A simple robot simulator written in C++. User can issue commands to create a robot onto a two-dimensional grid environment and control its movement.

![Menu](SimulatorMenu.png)

## Features

- Place a robot onto the cartesian plane using X and Y coordinates with direction that the robot will be facing. (e.g., North, South, West, and East).
- Move robot(s) one unit forward from its relative position to the direction the robot is currently facing.
- Rotate robot(s) in a 90-degree angle from left to right and vice versa.
- Report the current grid size and all the robots' info: ID, name, direction, and position.
- Move particular robot more than one block e.g. `Move Marvin 2 (blocks)`
- Dynamically expand the size of the grid (default is 10x10).


## Getting started
Prerequisites:
- This is a Windows console application, your machine should be running on Windows 11 or newer.
- Install the latest Visual Studio IDE available online with at least C++17 compiler. The community edition will suffice.
- Download or clone the repo:
	```
	git@github.com:chromagnonman/Marvin.git
	```
## Running the application
-  Open the entire solution or project by double-clicking on the `.sln` or `.vcxproj` file. Pressing `CTRL + F5` should run the console application.

## Executing the unit tests
- The solution also contains a `RobotSimulatorTest` project utilizing Google Test framework. To execute the tests, open the `Test Explorer` and click the play button.

	![TestExplorer](TestExplorer.png)

## Potential Enhancements
- Piping commands e.g. `Move R2D2 left` will move `R2D2` one block to the left.
- Support for nonbipedaled robots that may move faster (more than one block at a time) and can rotate in 180 or 360 degrees.
- Display the actual grid with all the robots mapped out.

## Caveats
- When executing the unit tests, you may encounter linker errors. You need to explicitly add the `.obj` files (Marvin, RobotGrid, and RobotSimulator) to the `Additional Dependencies`
  of the `RobotSimulatorTest` project.

	![Linker](Linker.png)
- You might also need to update the `Additional Library Directories`.

	![IncludeDirectories](IncludeDirectories.png)
- Lastly, you will probably need to set the `Runtime Library` to `Multi-threaded (/MT)`.

	![RuntimeLibrary](RuntimeLibrary.png)
	

