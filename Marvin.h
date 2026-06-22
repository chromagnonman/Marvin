#ifndef MARVIN_H
#define MARVIN_H

#include "Robot.h"

#include <cstdint>
#include <string>

namespace RobotFactory
{

class Marvin final : public Robot
{
  public:
    explicit Marvin(std::string name = "Marvin");
    Marvin(RobotLocation location, std::string name = "Marvin");
    ~Marvin() override = default;

    void rotate(Rotation rotation) noexcept override;
    void move(std::uint32_t blocks) noexcept override;
};

} // namespace RobotFactory

#endif
