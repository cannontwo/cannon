# Cannon's Lifetime Repository  {#mainpage}

TODO

[TOC]

# Requirements
* [CMake](https://cmake.org/)
* [OMPL](https://ompl.kavrakilab.org/)
* [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page)
* [CGAL](https://www.cgal.org/)
* [Boost](https://www.boost.org/)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)
* [HDF5](https://support.hdfgroup.org/HDF5/doc/cpplus_RM/index.html)
* [Assimp](https://www.assimp.org/)

# Optional Dependencies
* [Doxygen](https://www.doxygen.nl/index.html) (In order to build documentation)
* [Catch2](https://github.com/catchorg/Catch2) (In order to build tests)
* [Gurobi](https://www.gurobi.com/) (In order to build some optimizers)
* [OpenGL](https://www.opengl.org//) (In order to build graphics)
* [GLFW](https://www.glfw.org/) (In order to build graphics)
* [FreeType](https://www.freetype.org/) (In order to build graphics)

# Building

## Docker
The easiest way to get started with this repository is to use the included
Dockerfile, which can be built into an image by running

    docker build -t cannon/cannon .

The built image can then be run using the convenience script `run_docker.sh`.
These instructions should work on any operating system that can run Docker, but
won't build the graphics package.

## Linux
*These instructions have only been tested on Ubuntu 18.04*

If you've installed the requirements listed above, you should be able to build
everything by running

    mkdir build
    cd build
    cmake .. -DCANNON_BUILD_GRAPHICS=ON
    make

If you'd like to build this documentation as well, add the cmake option
`-DCANNON_BUILD_DOC=ON`.

## Build options
The `CMakeLists.txt` at the root of this repository supports the following
options, which can be supplied during the build process to affect which parts
of the repository get built. 

* `-DCANNON_BUILD_GRAPHICS=<ON/OFF>` --- Whether to build the `graphics` package.
* `-DCANNON_BUILD_RESEARCH=<ON/OFF>` --- Whether to build the `research` package. This option should always remain OFF (the default) unless you have access to my private research repository.
* `-DCANNON_BUILD_DOC=<ON/OFF>` --- Whether to build Doxygen documentation for this repository.
* `-DCANNON_BUILD_LIBRARY=<ON/OFF>` --- Whether to build the `cannon` library.

# Usage

**TODO**

# Adding scripts
To add a script which will be compiled as its own executable, simply add a
`.cpp` file to the `scripts/` subdirectory, and make sure that it includes a
`main()` function.

**TODO**

# Packages
- [Control](@ref control)
- [Geometry](@ref geom)
- [OpenGL Graphics](@ref graphics)
- [Logging](@ref log)
- [Symbolic Logic](@ref logic)
- [Math](@ref math)
- [Machine Learning](@ref ml)
- [Physics](@ref physics)
- [Plotting](@ref plot)
- [Raytracer](@ref ray)
- [Utilities](@ref utils)

