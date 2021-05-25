# Cannon's Lifetime Repository

## About This Repository
More information TODO

## Build Status
| Workflow | Status 
|----------------|---------------------------------------------------------------------------------------------|
| Documentation  | ![Documentation](https://github.com/cannontwo/cannon/workflows/Documentation/badge.svg)    
| Docker Testing | ![Docker Image CI](https://github.com/cannontwo/cannon/workflows/Docker%20Image%20CI/badge.svg)|

## Requirements
* [CMake](https://cmake.org/)
* [OMPL](https://ompl.kavrakilab.org/)
* [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page)
* [CGAL](https://www.cgal.org/)
* [Boost](https://www.boost.org/)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)
* [HDF5](https://support.hdfgroup.org/HDF5/doc/cpplus_RM/index.html)
* [Assimp](https://www.assimp.org/)

## Optional Dependencies
* [Doxygen](https://www.doxygen.nl/index.html) (In order to build documentation)
* [Catch2](https://github.com/catchorg/Catch2) (In order to build tests)
* [Gurobi](https://www.gurobi.com/) (In order to build some optimizers)
* [OpenGL](https://www.opengl.org//) (In order to build graphics)
* [GLFW](https://www.glfw.org/) (In order to build graphics)
* [FreeType](https://www.freetype.org/) (In order to build graphics)

## Installation Instructions
If all dependencies are installed, you can execute the following commands at
the top-level directory to build this repository. These instructions have only
been tested on Ubuntu 18.04.

    mkdir build
    cd build
    cmake ..
    make

## Build options
The `CMakeLists.txt` at the root of this repository supports the following
options, which can be supplied during the build process to affect which parts
of the repository get built. 

* `-DCANNON_BUILD_GRAPHICS=<ON/OFF>` --- Whether to build the `graphics` package.
* `-DCANNON_BUILD_RESEARCH=<ON/OFF>` --- Whether to build the `research` package. This option should always remain OFF (the default) unless you have access to my private research repository.
* `-DCANNON_BUILD_DOC=<ON/OFF>` --- Whether to build Doxygen documentation for this repository.
* `-DCANNON_BUILD_LIBRARY=<ON/OFF>` --- Whether to build the `cannon` library.
