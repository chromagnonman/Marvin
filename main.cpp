#include "RobotSimulator.h"

int main()
{
    using namespace Simulator;
    
    RobotGrid robot_excursion;
    RobotSimulator simulate(robot_excursion);

    simulate.start();

    return 0;
}
