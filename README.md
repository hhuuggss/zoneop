# Script extender plugin for [zone magic](https://www.nexusmods.com/skyrimspecialedition/mods/22318), a mod for a game about dragons.

Included here are the build instructions supplied with [ExamplePlugin-CommonLibSSE](https://github.com/Ryan-rsm-McKenzie/ExamplePlugin-CommonLibSSE):
## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2019](https://visualstudio.microsoft.com/)
	* C++ Clang tools for Windows
	* Desktop development with C++

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/hhuuggss/zoneop
cd zoneop
git submodule update --init --recursive
git submodule update --recursive --remote
cmake -B build -S .
```
