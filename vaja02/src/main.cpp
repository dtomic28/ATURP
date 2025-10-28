#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

/**
 * Disjoint-set (Union-Find) with edge weights:
 *  - parent_[i] : parent of node i (root has parent_[i] == i)
 *  - dist_[i]   : distance from i to parent_[i], modulo 1000
 *
 * Semantics required by the task:
 *  - connect(M, S):
 *      Let r = find(M). Attach r directly under S.
 *      The edge weight from r -> S is |r - S| % 1000.
 *  - query(X):
 *      Return the sum of weights on the path X -> root(X), modulo 1000.
 *
 * Path compression accumulates distances so dist_[x] becomes distance to the root.
 */

class DisjointSet {
public:

    explicit DisjointSet(std::size_t size)
        : parent_(size + 1), dist_(size + 1, 0) // 1-based indexing
    {
        for (std::size_t i = 1; i <= size; ++i) {
            parent_[i] = static_cast<int>(i);
        }
    }

    /// Attach the set containing M under S, with edge weight |root(M) - S| % 1000.
    void connect(int m, int s) {
        const int rm = find(m);
        if (rm == s) {
            return; // Already directly under S (or same node) — nothing to do.
        }
        parent_[rm] = s;
        dist_[rm] = std::abs(rm - s) % kMod;
    }

    /// Return distance from X to its root (sum of edge weights), modulo 1000.
    int query(int x) {
        find(x);
        return dist_[x];
    }

private:
    static constexpr int kMod = 1000;

    // Returns root of x; along the way compresses path and accumulates distances.
    int find(int x) {
        const int p = parent_[x];
        if (p == x) {
            return x; // x is root
        }
        const int root = find(p);
        dist_[x] = dist_[x] + dist_[p];
        parent_[x] = root;
        return root;
    }

    std::vector<int> parent_;
    std::vector<int> dist_;
};

/**
 * Process commands from `in` and write query results to `out`.
 * Input format:
 *   N
 *   C M S
 *   G X
 *   ... (until EOF)
 */
void processStream(std::istream& in, std::ostream& out) {
    int n = 0;
    if (!(in >> n) || n <= 0) {
        return;
    }

    DisjointSet dsu(static_cast<std::size_t>(n));

    char op;
    while (in >> op) {
        if (op == 'C') {
            int m = 0, s = 0;
            in >> m >> s;
            dsu.connect(m, s);
        } else if (op == 'G') {
            int x = 0;
            in >> x;
            out << dsu.query(x) << '\n';
        } else {
            // Unknown token: skip rest of the line to stay in sync.
            std::string discard;
            std::getline(in, discard);
        }
    }
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc >= 2) {
        std::ifstream fin(argv[1]);
        if (!fin) {
            std::cerr << "Error: cannot open file: " << argv[1] << '\n';
            return 1;
        }
        processStream(fin, std::cout);
    }
    return 0;
}
