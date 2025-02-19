# Moran Deviation CI Histogram Project

With the Terminal, go to the correspond file: Moran_deviation_CI_hist, then

To compile the Deviation Computation (Supremum Deviation Calculation), write:
gcc -I include src/main.c src/moran_simulation.c src/moran_deviation.c -o moran_sim -lm

To run the Moran Stochastic Simulation:
./bin/moran_sim.exe simulation

To run the Supremum Deviation Computation:
./bin/moran_dev.exe deviation

To plot Expected Supremum Deviation vs. N and the histograms:
python plot_deviation.py
