#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(std::size_t size)
        : parents(size+1), distances(size+1, 0)
    {
        for (std::size_t i = 1; i <= size; ++i)
            makeSet(static_cast<int>(i));
    }

    void makeSet(int x) {
        if (x >= static_cast<int>(parents.size())) {
            parents.resize(x+1);
            distances.resize(x+1);
        }
        parents[x] = x;
        distances[x] = 0;
    }

    void connect(int m, int s) {
        const int rm = findSet(m);
        if (rm == s) {
            return;
        }
        parents[rm] = s;
        distances[rm] = std::abs(rm - s) % kMod;
    }

    int64_t query(int x) {
        findSet(x);
        return distances[x];
    }

private:
    static constexpr int kMod = 1000;

    int findSet(int x) {
        const int p = parents[x];
        if (p == x) {
            return x;
        }
        const int root = findSet(p);
        distances[x] = distances[x] + distances[p];
        parents[x] = root;
        return root;
    }

    std::vector<int> parents;
    std::vector<int64_t> distances;
};

void processStream(std::istream& in, std::ostream& out) {
    int n = 0;
    if (!(in >> n) || n <= 0) {
        return;
    }

    DisjointSet dsu(static_cast<std::size_t>(n));

    char op;
    while (in >> op) {
        if (op == 'C') {
            //Connect m-th node with sth node (s node becomes main ship).
            int m = 0, s = 0;
            in >> m >> s;
            dsu.connect(m, s);
        } else if (op == 'G') {
            //Query length from Xth ship to main ship in it's fleet
            int x = 0;
            in >> x;
            out << dsu.query(x) << '\n';
        } else {
            std::string discard;
            std::getline(in, discard);
        }
    }
}

int main(int argc, char* argv[]) {
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
