#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "moran_simulation.h"

// Define rplus and rminus functions
double rplus(int X, int N, double r) {
    return ((N - X) * r * X) / (r * X + (N - X));
}

double rminus(int X, int N, double r) {
    return ((N - X) * X) / (r * X + (N - X));
}

void run_moran_simulation(int nsim, int *N_values, int num_N, double *r_values, int num_r, const char *filename) {
    FILE *file = fopen(filename, "w");
    FILE *abs_file = fopen("data/absorption_times.csv", "w");

    if (!file || !abs_file) {
        printf("Error opening file!\n");
        return;
    }

    // Write headers
    fprintf(file, "Simulation,N,r,Time,Population\n");
    fprintf(abs_file, "N,r,Simulation,AbsorptionTime\n");

    srand(time(NULL));  // Seed random number generator

    for (int i = 0; i < num_N; i++) {
        int N = N_values[i];
        int X0 = N / 2; // Start with half individuals of type A

        for (int j = 0; j < num_r; j++) {
            double r = r_values[j];

            for (int sim = 0; sim < nsim; sim++) {
                int Xt = X0;
                double t = 0.0;

                // Show descriptive progress message
                printf("\rProcessing: N=%d, r=%.2f, Simulation %d/%d", N, r, sim + 1, nsim);
                fflush(stdout); // Force immediate update

                while (Xt > 0 && Xt < N) {
                    double rho_plus = rplus(Xt, N, r);
                    double rho_minus = rminus(Xt, N, r);
                    double rtot = rho_plus + rho_minus;

                    // Prevent division by zero
                    if (rtot == 0) {
                        printf("Warning: rtot=0, skipping update.\n");
                        break;
                    }

                    double tau = -log((double)rand() / RAND_MAX) / rtot;

                    // Ensure tau is finite (fix for inf issue)
                    if (!isfinite(tau) || tau > 1e6) {
                        tau = 1e6; // Large finite value as a fallback
                    }

                    t += tau;

                    double p = (double)rand() / RAND_MAX;
                    Xt += (p < rho_plus / rtot) ? 1 : -1;

                    fprintf(file, "%d,%d,%.2f,%.6f,%d\n", sim, N, r, t, Xt);
                }

                // Store absorption time in the new CSV file
                if (isfinite(t)) {
                    fprintf(abs_file, "%d,%.2f,%d,%.6f\n", N, r, sim, t);
                } else {
                    printf("Warning: Absorption time was infinite, skipping entry.\n");
                }
            }
        }
    }

    printf("\nSimulation complete!\n");

    fclose(file);
    fclose(abs_file);
    printf("Results saved to %s\n", filename);
    printf("Absorption times saved to data/absorption_times.csv\n");
}
