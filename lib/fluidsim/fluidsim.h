// width and height of grid (assumed to be a square)
#define N 128
#define SCALE 8
// macro to map from 2D coords to a 1D array index
// velocities and densities are defined over 2D grids,
// but we represent them as 1D arrays for efficiency
#define IX(x, y) ((x) + (y) * N)

struct FluidSquare {
    int size;
    float dt;   // time step constant
    float diff; // diffusion constant
    float visc; // viscosity constant
    float *s; // previous density, array of density scalars
    float *density; // current density, array of density scalars
    float *Vx;  // curr. velocity, array of x-component scalars
    float *Vy;  // curr. velocity, array of y-component scalars
    float *Vx0; // prev. velocities, array of x-component scalars
    float *Vy0; // prev. velocities, array of y-component scalars
};
FluidSquare* FluidSquareCreate(int diff, int visc, float dt);
void FluidSquareFree(FluidSquare *sq);
void FluidSquareAddDensity(FluidSquare *sq, int x, int y, float amount);
void FluidSquareAddVelocity(FluidSquare *sq, int x, int y, float amountX, float amountY);
void FluidSquareStep(FluidSquare *sq);
