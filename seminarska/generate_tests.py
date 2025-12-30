import random
from pathlib import Path

Ns = []
Ns = [100_000, 500_000, 1_000_000, 2_000_000, 5_000_000, 10_000_000]

random.seed(42)

OUT_DIR = Path("tests")
OUT_DIR.mkdir(exist_ok=True)

# Fixed trap (nice for visualization)
X1, Y1, X2, Y2 = -10, -10, 10, 10

for N in Ns:
    filename = OUT_DIR / f"test_{N}.in"
    with open(filename, "w") as f:
        f.write(f"{N} {X1} {Y1} {X2} {Y2}\n")
        for _ in range(N):
            U = random.randint(-100, 100)
            V = random.randint(-100, 100)

            # velocities biased so some bears will be caught, some not
            A = random.randint(-5, 5)
            B = random.randint(-5, 5)

            # avoid stationary for better spread
            if A == 0 and B == 0:
                A = 1

            f.write(f"{U} {V} {A} {B}\n")

    print(f"Generated {filename}")
