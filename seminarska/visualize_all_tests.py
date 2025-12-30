import sys
import subprocess
from pathlib import Path
import matplotlib.pyplot as plt

EXECUTABLE = r"C:\Users\danijel\CLionProjects\ATURP\cmake-build-debug\seminarska\seminarska.exe"
TEST_DIR = Path("tests")
OUT_DIR = Path("visualizations")
OUT_DIR.mkdir(exist_ok=True)

def read_input(path):
    with open(path) as f:
        data = list(map(float, f.read().split()))
    it = iter(data)

    N = int(next(it))
    X1, Y1, X2, Y2 = next(it), next(it), next(it), next(it)

    bears = []
    for _ in range(N):
        U = next(it); V = next(it)
        A = next(it); B = next(it)
        bears.append((U, V, A, B))

    return X1, Y1, X2, Y2, bears

def inside(x, y, X1, Y1, X2, Y2):
    return X1 < x < X2 and Y1 < y < Y2

for file in sorted(TEST_DIR.glob("test_*.in")):
    print(f"Visualizing {file.name}")
    X1, Y1, X2, Y2, bears = read_input(file)

    plt.figure(figsize=(7,7))

    # Draw trap
    plt.plot([X1,X2,X2,X1,X1], [Y1,Y1,Y2,Y2,Y1], 'k-', lw=2)

    for (U,V,A,B) in bears:
        ts = [i * 0.2 for i in range(200)]
        xs = [U + A*t for t in ts]
        ys = [V + B*t for t in ts]

        caught = any(inside(x,y,X1,Y1,X2,Y2) for x,y in zip(xs,ys))

        if caught:
            plt.plot(xs, ys, 'r-', alpha=0.8)
        else:
            plt.plot(xs, ys, 'b-', alpha=0.4)

        plt.plot(U, V, 'ko', ms=3)

    plt.title(file.name)
    plt.axis('equal')
    plt.grid(True)

    out = OUT_DIR / f"{file.stem}.png"
    plt.savefig(out, dpi=200)
    plt.close()

print("\nAll visualizations created.")
