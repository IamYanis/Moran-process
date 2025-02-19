#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "moran_simulation.h"
#include "moran_deviation.h"

#define DT 0.01  // Time step for numerical ODE integration

// Function to numerically solve the ODE 
void solve_ode(double *ode_t, double *ode_x, int steps, double r, double x0) {
    ode_x[0] = x0;
    for (int i = 1; i < steps; i++) {
        double dx_dt = ((r - 1) * ode_x[i - 1] * (1 - ode_x[i - 1])) / (r * ode_x[i - 1] + (1 - ode_x[i - 1]));
        ode_x[i] = ode_x[i - 1] + dx_dt * DT;  // Euler's method
        ode_t[i] = i * DT;  // Store time points
    }
}

void compute_moran_deviation(int nsim, int *N_values, int num_N, double *r_values, int num_r, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "N,r,E_sup_deviation,Variance_sup_deviation,T_N\n");

    srand(time(NULL));  // Seed random number generator

    for (int i = 0; i < num_N; i++) {
        int N = N_values[i];
        printf("Computing deviation for N=%d\n", N); // Debugging statement
        double T_N = log(N);  // Set T(N) function of N

        for (int j = 0; j < num_r; j++) {
            double r = r_values[j];

            double sum_deviation = 0.0;
            double sum_sq_deviation = 0.0;

            for (int sim = 0; sim < nsim; sim++) {
                int Xt = N / 2;  // initial population of type A
                double t = 0.0;
                double max_dev = 0.0;

                // Precompute ODE trajectory for comparison
                int steps = (int)(T_N / DT) + 1;
                double ode_t[steps], ode_x[steps];
                solve_ode(ode_t, ode_x, steps, r, (double)Xt / N);

                int ode_idx = 0;  // Track position in ODE solution

                while (t < T_N && Xt > 0 && Xt < N) {
                    double rho_plus = rplus(Xt, N, r);
                    double rho_minus = rminus(Xt, N, r);
                    double rtot = rho_plus + rho_minus;

                    double tau = -log((double)rand() / RAND_MAX) / rtot;
                    t += tau;

                    // Interpolate ODE solution at time `t`
                    while (ode_idx < steps - 1 && ode_t[ode_idx + 1] < t) {
                        ode_idx++;
                    }
                    double x_det = ode_x[ode_idx];

                    // Compute absolute deviation
                    double x_t = (double)Xt / N;
                    double deviation = fabs(x_t - x_det); // |X(t)/N - x(t)|
                    // Store the sup:
                    if (deviation > max_dev) {
                        max_dev = deviation;
                    }

                    // Gillespie update step
                    double vv = (double)rand() / RAND_MAX;
                    if (vv < rho_plus / rtot) {
                        Xt += 1;
                    } else {
                        Xt -= 1;
                    }
                }

                sum_deviation += max_dev;
                sum_sq_deviation += max_dev * max_dev;
            }
            // MC estimation
            double mean_dev = sum_deviation / nsim;
            double variance_dev = (sum_sq_deviation / nsim) - (mean_dev * mean_dev);

            fprintf(file, "%d,%.2f,%.6f,%.6f,%.6f\n", N, r, mean_dev, variance_dev, T_N);
        }
    }

    fclose(file);
    printf("Deviation results saved to %s\n", filename);
}
