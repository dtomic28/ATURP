import csv
import matplotlib.pyplot as plt
from collections import defaultdict

DATA_FILE = "benchmark_results.csv"
OUT_FILE = "performance_plot_oldž.png"

data = defaultdict(list)

with open(DATA_FILE) as f:
    reader = csv.DictReader(f)
    for row in reader:
        N = int(row["N"])
        alg = row["algorithm"]
        t = float(row["time_sec"])
        data[alg].append((N, t))

plt.figure(figsize=(8, 6))

for alg in sorted(data):
    xs, ys = zip(*sorted(data[alg]))
    plt.plot(xs, ys, marker='o', label=alg)

plt.xlabel("Velikost vhodnih podatkov (N)")
plt.ylabel("Čas izvajanja (sekunde)")
plt.title("Primerjava časovne zahtevnosti algoritmov")
plt.xscale("log")
plt.yscale("log")
plt.grid(True, which="both")
plt.legend()

plt.tight_layout()
plt.savefig(OUT_FILE, dpi=200)
plt.show()

print(f"Plot saved to {OUT_FILE}")
