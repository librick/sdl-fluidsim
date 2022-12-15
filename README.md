# SDL FLuidSim
C++ fluid simulation based on Daniel Shiffman's implementation of Jos Stam’s "Real-Time Fluid Dynamics for Games" algorithm.  
Daniel Shiffman's implementation in turn is based on Mike Ash's guide to Fluid Simulation.  

I added an ImGUI window to control some aspects of the simulation.  
Not my best code, putting it out there for a friend to reference.  
Feel free to make PRs, MIT licensed.  

## Building
In theory, you should be able to just run `make` and then run the generated binary with `./out`.  
But in theory there is no difference between theory and practice; in practice there is.  
My Makefile is a mess.  

I get things going with:
`make imgui`  
`make particle.o`  
`make`  
`./out`  

## Dependencies
On Debian, dependencies include (non-exhaustive):  
- `build-essential`  
- `libsdl2-dev`  
- `libsdl2-image-dev`  

