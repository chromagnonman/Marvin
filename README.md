# Marvin (The Paranoid Android)

[![CI](https://github.com/chromagnonman/Marvin/actions/workflows/ci.yml/badge.svg)](https://github.com/chromagnonman/Marvin/actions/workflows/ci.yml)

Marvin is a C++20 console application for creating and controlling robots in a two-dimensional
grid. It supports multiple named robots, collision detection, movement and rotation, reporting,
and dynamic grid expansion.

![Simulator menu](SimulatorMenu.png)

## Features

- Place multiple named robots at grid coordinates facing north, east, south, or west.
- Move one robot by a chosen number of blocks or move every robot by one block.
- Rotate one or all robots left or right by 90 degrees.
- Prevent robots from leaving the grid or moving into occupied cells.
- Report the grid size and each robot's ID, name, direction, and position.
- Expand rectangular or square grids while preserving existing robot positions.

The default grid is `10x10`. Commands are case-insensitive.

## Requirements

- Windows 11 or a compatible Windows build environment.
- Visual Studio or Visual Studio Build Tools with:
  - Desktop development with C++
  - MSVC v145 for the checked-in project configuration
  - A Windows 10 or newer SDK
  - C++ Clang tools for Windows (clang-format and clang-tidy)
- NuGet package restore enabled for the Google Test dependency.

The CI build uses the compatible MSVC v143 toolset available on the `windows-2022` GitHub-hosted
runner.

## Getting Started

Clone the repository:

```powershell
git clone https://github.com/chromagnonman/Marvin.git
cd Marvin
```

Open `Marvin.sln` in Visual Studio. Select `Debug` and `x64`, then press `Ctrl+F5` to build and run
the simulator.

Visual Studio should restore the NuGet dependency automatically. If it does not, right-click the
solution and select **Restore NuGet Packages**.

## Commands

Examples of supported commands:

```text
PLACE R2D2 1,1 NORTH
MOVE R2D2 2
MOVE ALL
ROTATE R2D2 LEFT
RIGHT ALL
REMOVE R2D2
REMOVE ALL
REPORT
RESIZE 20 15
MENU
```

Press `Ctrl+C` or send end-of-file to exit.

## Build From The Command Line

Run these commands from a Developer PowerShell for Visual Studio:

```powershell
nuget restore Marvin.sln -PackagesDirectory packages
msbuild Marvin.sln /m /p:Configuration=Debug /p:Platform=x64
```

The application is written to `x64\Debug\Marvin.exe`.

## Tests

The `RobotSimulatorTest` project uses Google Test and covers robot movement, rotation, placement,
grid boundaries, rectangular grids, resizing, and occupancy preservation.

Run the tests through Visual Studio Test Explorer or execute the test binary after building:

```powershell
.\x64\Debug\RobotSimulatorTest.exe
```

## Code Quality

The repository includes shared configuration for both tools:

- `.clang-format` enforces the C++20 Microsoft-based formatting style used by the project.
- `.clang-tidy` enables analyzer, bug-prone, performance, portability, and selected modernization
  checks. Findings are treated as errors.

Check formatting without modifying files:

```powershell
$files = git ls-files '*.cpp' '*.h'
clang-format --dry-run --Werror $files
```

clang-tidy runs automatically as part of the `Debug|x64` project build. Formatting is not applied
automatically when a file is saved; in Visual Studio, use **Format Document** (`Ctrl+K, Ctrl+D`).

## Continuous Integration

The [GitHub Actions workflow](.github/workflows/ci.yml) runs for every pull request and every push
to `master`. It:

1. Restores and caches NuGet packages.
2. Verifies clang-format compliance.
3. Builds the `Debug|x64` solution with MSVC v143 and runs clang-tidy.
4. Runs the complete Google Test suite.

## Potential Enhancements

- Add aerial, aquatic, subaquatic, quadrupedal, and wheeled robot models.
- Render the grid and robot positions in the console.
- Add movement and rotation capabilities specific to each robot type.

## License

This project is licensed under the terms in [LICENSE](LICENSE).
