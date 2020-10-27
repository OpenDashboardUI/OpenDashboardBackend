# OpenDashboard - Windows Guide

## Installation


## Development environment

The following explanations are valid for x64 (commands for x86 might differ slightly).

### Setup toolchain

#### CMake

Download and install the latest version of [CMake](https://cmake.org/download/)

Note: Make sure the CMake directory (e.g. **C:\Program Files\CMake\bin**) is added to your **PATH** variable.

#### MSVC

Download and install [Visual Studio Community Edition](https://visualstudio.microsoft.com/) (tested with 2019). This will provide you with the MSVC++ compiler.

#### vcpkg

For comfortable dependency management you can use **vcpkg**. You can find an installation instruction [here](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019).

After the installation you can build all dependencies that are needed by OpenDashboard by executing the **scripts/build_vcpkg_dependencies.bat** script.

Note: Make sure the vcpkg directory (e.g. **C:\vcpkg**) is added to your **PATH** variable.

#### Python

Download and install [Python 3.x](https://www.python.org/).

Note: Make sure the Python directory (e.g. **C:\Program Files\Python39**) is added to your **PATH** variable.

#### Qt5

Download and install [Qt5 open source version](https://www.qt.io/download). The following options need to be selected during the installation process (can be changed later on):

**Qt 5.x.y:**

* MSVC 2019 64-bit
* Qt *

**Development and Designer Tools:**

* Qt Creator X.Y.Z
* Qt Creator X.Y.Z CDB Debugger Support
* Debugging Tools for Windows

Note: Make sure that the Qt5 binary dir for MSVC (e.g. **C:\Qt\5.15.1\msvc2019_64\bin**) is added to your **PATH** variable.

## Build & Deploy

### Build

Get the source code:

	git clone git@github.com:OpenDashboardUI/OpenDashboardBackend.git
	cd OpenDashboardBackend
	git submodule init
	git submodule update

Create a build directory:

	mkdir OpenDashboardBackend_build_msvc2019
	cd OpenDashboardBackend_build_msvc2019

Generate build environment with CMake:

	 cmake -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE=PATH_TO_VCPKG\vcpkg\scripts\buildsystems\vcpkg.cmake ..\OpenDashboardBackend
	 
Invoke the build process by the following command or by using the solution file with Visual Studio:

	cmake --build . --config Release


### Deploy

Create a deployable output directory:

	cd OpenDashboardBackend_build_msvc2019
	cmake --build . --target windeployqt_open_dashboard_backend --config Release