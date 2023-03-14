#include "RobotSimulator.h"

int main()
{
    RobotWorldSimulator::RobotGrid robot_excursion;
    RobotWorldSimulator::RobotSimulator simulate(robot_excursion);

    simulate.start();
    
    return 0;
}
