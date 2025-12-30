import subprocess
import time
import csv
from pathlib import Path

# ===== CONFIG =====
EXECUTABLE = r"C:\Users\danijel\CLionProjects\ATURP\cmake-build-release\seminarska\seminarska.exe"
TEST_DIR = Path("tests")
OUT_FILE = "benchmark_results.csv"

MODES = {
    0: "naive",
    1: "naive_zero",
    3: "smart"
}

# ===== RUN BENCHMARK =====
results = []

for test_file in sorted(TEST_DIR.glob("test_*.in"), key=lambda p: int(p.stem.split("_")[1])):
    N = int(test_file.stem.split("_")[1])

    # Warm-up run (not measured)
    subprocess.run([EXECUTABLE, str(test_file), "3"],
                   stdout=subprocess.DEVNULL,
                   stderr=subprocess.DEVNULL)

    for mode, name in MODES.items():
        print(f"Running N={N} mode={name}")

        start = time.perf_counter()
        subprocess.run([EXECUTABLE, str(test_file), str(mode)],
                       stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL)
        end = time.perf_counter()

        elapsed = end - start
        results.append([N, name, elapsed])

# ===== SAVE RESULTS =====
with open(OUT_FILE, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["N", "algorithm", "time_sec"])
    writer.writerows(results)

print(f"\nSaved results to {OUT_FILE}")
