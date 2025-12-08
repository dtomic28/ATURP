#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

struct Island {
    int x, y;
    int64_t t;
};

const int64_t INF = (1LL << 60);

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Input: allow "./vaja4 file.txt" OR "./vaja4 < file.txt"
    istream* in = &cin;
    if (argc > 1) {
        static ifstream fin(argv[1]);
        if (fin.is_open()) in = &fin;
    }

    int N, M, K;
    *in >> N >> M >> K;

    vector<Island> islands(K);
    for (int i = 0; i < K; i++)
        *in >> islands[i].x >> islands[i].y >> islands[i].t;

    // Sort by row, then y
    sort(islands.begin(), islands.end(),
        [](auto& a, auto& b) {
            return (a.x < b.x) || (a.x == b.x && a.y < b.y);
        }
    );

    vector<int64_t> result(K + 1, INF);
    result[0] = 0;

    // A[k][0] = end at y=1, A[k][1] = end at y=M
    vector<array<int64_t,2>> minimalTimeToExploreK(K + 1, {INF, INF});
    minimalTimeToExploreK[0][0] = 0;

    int prevRow = 1;
    int index = 0;

    while (index < K) {
        int row = islands[index].x;
        int startingIndex = index;

        //Calculate how many islands in row
        while (index < K && islands[index].x == row) index++;
        int cnt = index - startingIndex;

        // Move down to this row
        int delta = row - prevRow;
        prevRow = row;

        if (delta > 0) {
            for (int k = 0; k <= K; k++) {
                if (minimalTimeToExploreK[k][0] < INF) minimalTimeToExploreK[k][0] += delta;
                if (minimalTimeToExploreK[k][1] < INF) minimalTimeToExploreK[k][1] += delta;
            }
        }

        vector<int> y(cnt);
        vector<int64_t> t(cnt);
        for (int j = 0; j < cnt; j++) {
            y[j] = islands[startingIndex + j].y;
            t[j] = islands[startingIndex + j].t;
        }

        // ----------- Row DP computations -----------

        vector<int64_t> dpAll(cnt + 1, INF);
        dpAll[0] = 0;

        for (int j2 = 0; j2 < cnt; j2++) {
            for (int c = j2 + 1; c >= 1; c--)
                dpAll[c] = min(dpAll[c], dpAll[c - 1] + t[j2]);
        }

        int64_t cross = M - 1;
        vector<int64_t> costLR(cnt + 1), costRL(cnt + 1);
        costLR[0] = cross;
        costRL[0] = cross;

        for (int c = 1; c <= cnt; c++) {
            int64_t sumT = dpAll[c];
            costLR[c] = (sumT >= INF ? INF : cross + sumT);
            costRL[c] = costLR[c];
        }

        // Start left
        vector<int64_t> costLL(cnt + 1, INF), interiorL(cnt + 1, INF);
        costLL[0] = 0;
        interiorL[0] = 0;

        vector<int64_t> dp(cnt + 1, INF);
        dp[0] = 0;

        for (int j2 = 0; j2 < cnt; j2++) {
            vector<int64_t> old = dp;
            int64_t tj = t[j2];
            int maxC = j2 + 1;
            int64_t dist1 = y[j2] - 1;
            int64_t dist2 = dist1 * 2;

            for (int c = maxC; c >= 1; c--) {
                if (old[c - 1] >= INF) continue;

                int64_t inc = old[c - 1] + tj;

                interiorL[c] = min(interiorL[c], inc + dist1);
                costLL[c]    = min(costLL[c],    inc + dist2);

                dp[c] = min(dp[c], inc);
            }
        }

        // Start right (mirror)
        vector<int64_t> costRR(cnt + 1, INF), interiorR(cnt + 1, INF);
        costRR[0] = 0;
        interiorR[0] = 0;

        vector<int> yR(cnt);
        vector<int64_t> tR(cnt);
        for (int j = 0; j < cnt; j++) {
            int idx = cnt - 1 - j;
            yR[j] = M + 1 - y[idx];
            tR[j] = t[idx];
        }

        dp.assign(cnt + 1, INF);
        dp[0] = 0;

        for (int j2 = 0; j2 < cnt; j2++) {
            vector<int64_t> old = dp;
            int64_t tj = tR[j2];
            int maxC = j2 + 1;
            int64_t dist1 = yR[j2] - 1;
            int64_t dist2 = dist1 * 2;

            for (int c = maxC; c >= 1; c--) {
                if (old[c - 1] >= INF) continue;

                int64_t inc = old[c - 1] + tj;

                interiorR[c] = min(interiorR[c], inc + dist1);
                costRR[c]    = min(costRR[c],    inc + dist2);

                dp[c] = min(dp[c], inc);
            }
        }

        // -------- Combine row result with global DP --------

        auto A_prev = minimalTimeToExploreK;
        vector<array<int64_t,2>> newA(K + 1, {INF, INF});

        for (int already = 0; already <= K; already++) {
            int64_t fromL = A_prev[already][0];
            int64_t fromR = A_prev[already][1];

            if (fromL >= INF && fromR >= INF) continue;

            int maxTake = min(cnt, K - already);

            for (int take = 0; take <= maxTake; take++) {
                int newk = already + take;

                // end inside row
                if (take > 0) {
                    if (fromL < INF)
                        result[newk] = min(result[newk], fromL + interiorL[take]);
                    if (fromR < INF)
                        result[newk] = min(result[newk], fromR + interiorR[take]);
                }

                // left start
                if (fromL < INF) {
                    newA[newk][0] = min(newA[newk][0], fromL + costLL[take]);
                    newA[newk][1] = min(newA[newk][1], fromL + costLR[take]);
                }

                // right start
                if (fromR < INF) {
                    newA[newk][1] = min(newA[newk][1], fromR + costRR[take]);
                    newA[newk][0] = min(newA[newk][0], fromR + costRL[take]);
                }
            }
        }

        // update RESULT also from border states
        for (int k = 0; k <= K; k++)
            result[k] = min(result[k], min(newA[k][0], newA[k][1]));

        minimalTimeToExploreK.swap(newA);
    }

    // Output
    for (int k = 1; k <= K; k++) {
        if (k > 1) cout << ' ';
        cout << result[k];
    }
    cout << "\n";

    return 0;
}
