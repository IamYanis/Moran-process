#ifndef MORAN_SIMULATION_H
#define MORAN_SIMULATION_H

// Declare functions so they are recognized in other files
double rplus(int X, int N, double r);
double rminus(int X, int N, double r);

void run_moran_simulation(int nsim, int *N_values, int num_N, double *r_values, int num_r, const char *filename);

#endif
