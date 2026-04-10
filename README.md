# First-order Kuramoto Model Simulator

This project is a numerical simulator for the first order, mean field Kuramoto PDE with identical oscillators

![Kuramoto PDE](docs/equation.png)

---
## Model

The equation describes the evolution of the density function,  with:

- D : noise coefficient  
- K : coupling strength  
- Ω : natural frequency  

---
##  Description

The simulator allows the user to:
- configure system parameters (initial conditions, coupling, noise, etc.)
- run numerical simulations of the Kuramoto dynamics
- save simulation results for post processing and visualization

---


## Output

The simulation produces the solution of the PDE that can be used to analyze:
- synchronization dynamics
- order parameter evolution
- phase distributions

---

##  How to build and run

###  Requirements

- C++ compiler (e.g. `g++`, `clang++`, MSVC)
- CMake (>= 3.10)

---

###  Build

Clone the repository:

```bash
git clone https://github.com/giulioPecorella98/First-order-Kuramoto-1.git
```

Create a build directory:

```bash
cd First-order-Kuramoto-1
mkdir build
cd build
```

Configure and build:
```bash
cmake ..
cmake --build .
```
Run

```bash
./kuramoto.exe

P.S. On Windows, you may need to specify a generator (e.g. MinGW or Visual Studio):

```bash
cmake .. -G "MinGW Makefiles"
```


---


## Notes

This is a numerical implementation for research/educational purposes.
