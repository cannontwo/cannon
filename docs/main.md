# Cannon's Lifetime Repository  {#mainpage}

[TOC]

## Requirements

**TODO**

## Building

### Docker
The easiest way to get started with this repository is to use the included
Dockerfile, which can be built into an image by running

    docker build -t cannon/cannon .

The built image can then be run using the convenience script `run_docker.sh`.
These instructions should work on any operating system that can run Docker, but
won't build the graphics package.

### Linux
*These instructions have only been tested on Ubuntu 18.04*

If you've installed the requirements listed above, you should be able to build
everything by running

    mkdir build
    cd build
    cmake .. -DCANNON_BUILD_GRAPHICS=ON
    make

If you'd like to build this documentation as well, add the cmake option
`-DCANNON_BUILD_DOC=ON`.

## Usage

**TODO**

## Adding scripts
To add a script which will be compiled as its own executable, simply add a
`.cpp` file to the `scripts/` subdirectory, and make sure that it includes a
`main()` function.

**TODO**

## Packages
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

