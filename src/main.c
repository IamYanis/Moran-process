#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "moran_simulation.h"
#include "moran_deviation.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [simulation | deviation]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "simulation") == 0){
        printf("Running Moran simulation...\n");
        int nsim = 10000;
        int N_values[] = {10, 20, 50, 80, 100, 200, 300, 500, 700, 1000};
        int num_N = sizeof(N_values) / sizeof(N_values[0]);

        double r_values[] = {1.01, 1.03, 1.05, 1.1, 1.2, 1.3, 1.5};  
        int num_r = sizeof(r_values) / sizeof(r_values[0]);

        run_moran_simulation(nsim, N_values, num_N, r_values, num_r, "data/moran_simulation.csv");
    }
    else if (strcmp(argv[1], "deviation") == 0) {
        printf("Computing Moran deviation...\n");
        int nsim = 10000;

        //int N_values[] = {50, 378, 706, 1034, 1362, 1690, 2018, 2346, 2674, 3000, 3500, 4000};
        //int N_values[] = {50, 378, 706, 1034, 1362, 1690};
        int N_values[] = {10, 20, 50, 80, 100, 200, 300, 500, 700, 1000};
        int num_N = sizeof(N_values) / sizeof(N_values[0]);

        double r_values[] = {1.01, 1.03, 1.05, 1.1, 1.2, 1.3, 1.5};  
        int num_r = sizeof(r_values) / sizeof(r_values[0]);

        compute_moran_deviation(nsim, N_values, num_N, r_values, num_r, "data/deviation_results.csv");
    }
    else {
        printf("Invalid argument. Use 'simulation' or 'deviation'.\n");
        return 1;
    }

    return 0;
}
