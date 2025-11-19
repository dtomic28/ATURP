#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;


struct Pillow {
    int Y;
    int X;
    int D;
};

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: ./program <input_file>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin.is_open()) {
        cerr << "Error: cannot open input file.\n";
        return 1;
    }

    int N, V, S;
    fin >> N >> V >> S;

    // One multiset per column, storing Y-values
    vector< multiset<int> > colSets(S);

    // colMin[x] = min Y in colSets[x] (or V if empty)
    vector<int> colMin(S, V);

    // Segment tree for max over colMin[]
    int size = 1;
    while (size < S) size <<= 1;

    vector<int> segTree(2 * size, 0);

    // Initialize leaves
    for (int i = 0; i < S; i++)
        segTree[size + i] = V;

    // Build tree
    for (int i = size - 1; i > 0; i--)
        segTree[i] = max(segTree[i << 1], segTree[(i << 1) | 1]);

    // Update segment tree at a single column
    auto updateTree = [&](int pos, int value) {
        int idx = size + pos;
        segTree[idx] = value;
        idx >>= 1;
        while (idx > 0) {
            segTree[idx] = max(segTree[idx << 1], segTree[(idx << 1) | 1]);
            idx >>= 1;
        }
    };

    vector<Pillow> pillows(N + 1);
    int nextId = 1;

    vector<string> results;
    results.reserve(N);

    for (int i = 0; i < N; i++) {
        char op;
        fin >> op;

        if (op == '+') {
            // Add pillow
            int Y, X, D;
            fin >> Y >> X >> D;

            pillows[nextId] = {Y, X, D};

            for (int x = X; x <= X + D - 1; x++) {
                auto &ms = colSets[x];
                ms.insert(Y);

                int newMin = *ms.begin();
                if (newMin != colMin[x]) {
                    colMin[x] = newMin;
                    updateTree(x, newMin);
                }
            }

            nextId++;
        }
        else {
            // Remove pillow
            int K;
            fin >> K;

            Pillow &p = pillows[K];
            int Y = p.Y;
            int X = p.X;
            int D = p.D;

            for (int x = X; x <= X + D - 1; x++) {
                auto &ms = colSets[x];
                auto it = ms.find(Y);
                if (it != ms.end())
                    ms.erase(it);

                int newMin = ms.empty() ? V : *ms.begin();
                if (newMin != colMin[x]) {
                    colMin[x] = newMin;
                    updateTree(x, newMin);
                }
            }
        }

        // Global fall distance = maximum column minimum
        results.push_back(to_string(segTree[1]));
    }

    // Output results at the end
    for (const string &s : results)
        cout << s << '\n';

    return 0;
}