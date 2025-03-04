# AOSvsSOA

## Table of Contents
- [Introduction](#introduction)
- [Build and Run](#build-and-run)
- [Possible Output](#possible-output)
- [How does this work](#how-does-this-work)

## Introduction
The purpose of this project is to compare the performance of two different data layout strategies: Array of Structs (AoS) and Struct of Arrays (SoA).

## Build and Run
To clone and run this project, you'll need [Git](https://git-scm.com) and [CMake](https://cmake.org/) installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/Hayk10002/AOSvsSOA

# Go into the repository
$ cd AOSvsSOA

# Generate the build files
$ cmake -DCMAKE_BUILD_TYPE=Release -S . -B build

# Build the project
$ cmake --build build --config Release

# Then, run the executable generated in the `build` directory with the number of particles and iterations to run the test.
$ your/path/to/exe/main.exe {particle_count} {iteration_count}
# example - .../main.exe 1000000 1000
```

## Possible Output
(for particle count 1000000 and iteration count 1000)

```
Array of structs

Total mass time:        816us
Average position time: 1040us
Center of mass time:   1457us

Struct of arrays

Total mass time:        260us
Average position time:  764us
Center of mass time:    931us
```

## How does this work
The project measures the performance difference between Array of Structs (AoS) and Struct of Arrays (SoA) data layouts by performing various computations on a collection of particles.

In the AoS layout, each particle's properties (x, y, z, mass) are stored together in a single struct. In the SoA layout, each property is stored in a separate array. The code benchmarks the time taken to compute the total mass, average position, and center of mass for both layouts, providing insights into the performance impact of different data layouts.
