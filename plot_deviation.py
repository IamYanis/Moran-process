import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import linregress

# Load deviation data
data = pd.read_csv("data/deviation_results.csv")

# Extract unique values of N and r
N_values = sorted(data["N"].unique())
r_values = sorted(data["r"].unique())

print("N values in dataset:", sorted(data["N"].unique()))


n_sim = 10000 # Choose the same value as in main.c

# ----------------- First Window: Deviation Plots ----------------- #
fig1, axes = plt.subplots(1, 2, figsize=(12, 5))

scaling_exponents = {}

for r in r_values:
    subset = data[data["r"] == r]

    N = subset["N"]
    mean_dev = subset["E_sup_deviation"]
    variance_dev = subset["Variance_sup_deviation"]

    std_err = np.sqrt(variance_dev / n_sim)  # Standard error
    CI_95 = 1.96 * std_err  # 95% confidence interval

    log_N = np.log(N)
    log_mean_dev = np.log(mean_dev)

    SE_log_dev = std_err / mean_dev
    CI_log_95 = 1.96 * SE_log_dev

    slope, intercept, _, _, _ = linregress(N, log_mean_dev)
    scaling_exponents[r] = slope

    axes[0].errorbar(N, mean_dev, yerr=CI_95, marker='o', linestyle='-', capsize=5, label=f"r={r}")
    axes[1].errorbar(N, log_mean_dev, yerr=CI_log_95, marker='o', linestyle='-', capsize=5, label=f"r={r}")

axes[0].set_xlabel("Population size N")
axes[0].set_ylabel("$E[\sup_{t \in [0,T(N)]} |X(t)/N - x(t)|]$")
axes[0].set_title("Supremum deviation")
axes[0].legend()
axes[0].grid()

axes[1].set_xlabel("Population size N")
axes[1].set_ylabel("$Log \; E[\sup_{t \in [0,T(N)]} |X(t)/N - x(t)|]$")
axes[1].set_title("Log supremum deviation")
axes[1].legend()
axes[1].grid()

plt.tight_layout()
plt.show(block=False)  # Keep window open while continuing the script

# ----------------- Print Scaling Exponents ----------------- #
for r, slope in scaling_exponents.items():
    print(f"For r = {r}, estimated scaling exponent: {slope:.4f}")

# ----------------- Second Window: Histograms ----------------- #

# Load absorption times
abs_data = pd.read_csv("data/absorption_times.csv")

# Debug: Check for invalid values
print("Checking for invalid values in AbsorptionTime column...")
print(abs_data["AbsorptionTime"].describe())
print(abs_data["AbsorptionTime"].isna().sum(), "NaN values found")
print(np.isinf(abs_data["AbsorptionTime"]).sum(), "Inf values found")
print("MC number =",n_sim)

# Remove NaN and Inf values before plotting
abs_data = abs_data[np.isfinite(abs_data["AbsorptionTime"])]
selected_N = [50, 100, 300, 1000]

fig2, hist_axes = plt.subplots(1, len(selected_N), figsize=(15, 5), sharey=True)

for i, N in enumerate(selected_N):
    subset = abs_data[abs_data["N"] == N]
    for r in r_values[0:4]:  # Plot histograms for the choosen  r-values
        times = subset[subset["r"] == r]["AbsorptionTime"]
        
        # Remove invalid values
        times = times[np.isfinite(times)]

        hist_axes[i].hist(times, bins=60, alpha=0.5, histtype='stepfilled', label=f"r={r:.2f}", edgecolor="black", range=(0, 200)) #, range=(0, 1e5))



    

    hist_axes[i].set_xlabel("Absorption Time")
    hist_axes[i].set_title(f"N = {N}")
    hist_axes[i].legend()

hist_axes[0].set_ylabel("Frequency")
plt.suptitle("Histograms of Absorption Times for Different N")
plt.tight_layout()
plt.show(block=False)  # Keep window open

plt.show() 


