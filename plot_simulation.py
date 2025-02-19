import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm
from scipy.integrate import solve_ivp

# Load simulation data
data = pd.read_csv("data/moran_simulation.csv")

# Get unique N and r values
N_values = sorted(data["N"].unique())
r_values = sorted(data["r"].unique())

# Plot for each (N, r)
fig, axes = plt.subplots(len(N_values), len(r_values), figsize=(12, 8), sharex=True, sharey=True)

for i, N in enumerate(N_values):
    for j, r in enumerate(r_values):
        ax = axes[i, j] if len(N_values) > 1 and len(r_values) > 1 else axes[j]

        # Get data for specific (N, r)
        subset = data[(data["N"] == N) & (data["r"] == r)]

        # Solve deterministic ODE
        def moran_ode(t, y, r):
            return ((r - 1) * y * (1 - y)) / (r * y + 1 - y)

        X0 = N / 2
        ode_solution = solve_ivp(moran_ode, [0, 50], [X0 / N], args=(r,), t_eval=np.linspace(0, 50, 500))

        # Convert to population counts
        ode_time = ode_solution.t
        ode_population = N * ode_solution.y[0]

        # Plot stochastic simulations
        for sim in tqdm(subset["Simulation"].unique()):
            sim_data = subset[subset["Simulation"] == sim]
            ax.plot(sim_data["Time"], sim_data["Population"], color='k', lw=1, alpha=0.05)

        # Plot deterministic ODE solution
        ax.plot(ode_time, ode_population, color='r', lw=2, label="Deterministic ODE")

        ax.set_title(f"N={N}, r={r}")
        ax.set_xlabel("Time")
        ax.set_ylabel("Population")

plt.tight_layout()
plt.show()
