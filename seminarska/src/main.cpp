#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Bear {
    long long U, V, A, B;
};

double X1, Y1, X2, Y2;

bool inside(double x, double y) {
    return x > X1 && x < X2 && y > Y1 && y < Y2;
}

bool naive(const Bear& b) {
    const double dt = 0.01;
    const double Tmax = 20000;

    for(double t = 0; t <= Tmax; t += dt) {
        double x = b.U + b.A * t;
        double y = b.V + b.B * t;
        if(inside(x, y)) return true;
    }
    return false;
}

bool naiveWithZeroVelocityCheck(const Bear& b) {
    //Check if zero velocity
    if(b.A == 0 && b.B == 0)
        return inside(b.U, b.V);

    //Check if velocity is zero and X or Y not in trap.
    if(b.A == 0 && !(b.U > X1 && b.U < X2)) return false;
    if(b.B == 0 && !(b.V > Y1 && b.V < Y2)) return false;

    return naive(b);
}

bool naiveWithEarlyStop(const Bear& b) {
    //Check if zero velocity
    if(b.A == 0 && b.B == 0)
        return inside(b.U, b.V);

    //Check if velocity is zero and X or Y not in trap.
    if(b.A == 0 && !(b.U > X1 && b.U < X2)) return false;
    if(b.B == 0 && !(b.V > Y1 && b.V < Y2)) return false;

    //Check if moving in wrong direction
    if(b.U <= X1 && b.A <= 0) return false;
    if(b.U >= X2 && b.A >= 0) return false;
    if(b.V <= Y1 && b.B <= 0) return false;
    if(b.V >= Y2 && b.B >= 0) return false;

    return naive(b);
}

bool getInterval(double U, double A, double L, double R, double& outLow, double& outHigh)
{
    if(A == 0) {
        if(U > L && U < R) {
            outLow  = -DBL_MAX;
            outHigh =  DBL_MAX;
            return true;
        }
        return false;
    }

    double t1 = (L - U) / A;
    double t2 = (R - U) / A;

    outLow  = min(t1, t2);
    outHigh = max(t1, t2);
    return true;
}

bool smart(const Bear& b)
{
    double xLow, xHigh;
    if(!getInterval(b.U, b.A, X1, X2, xLow, xHigh)) return false;

    double yLow, yHigh;
    if(!getInterval(b.V, b.B, Y1, Y2, yLow, yHigh)) return false;

    double low  = max(xLow, yLow);
    double high = min(xHigh, yHigh);

    return high > max(low, 0.0);
}


int main(int argc, char* argv[]) {
    if(argc < 2 || argc > 3) {
        cout << "Usage: ./program input_file [mode]\n";
        cout << "Modes: 0=naive, 1=naive+zero, 2=naive+reject, 3=smart\n";
        return 0;
    }

    ifstream in(argv[1]);

    int mode = 3;               // default: smart
    if(argc == 3) mode = atoi(argv[2]);

    int N;
    in >> N >> X1 >> Y1 >> X2 >> Y2;

    if(X1 > X2) swap(X1, X2);
    if(Y1 > Y2) swap(Y1, Y2);

    vector<Bear> bears(N);
    for(int i = 0; i < N; i++)
        in >> bears[i].U >> bears[i].V >> bears[i].A >> bears[i].B;

    int caught = 0;
    for(const auto& b : bears) {
        bool ok;
        if(mode == 0) ok = naive(b);
        else if(mode == 1) ok = naiveWithZeroVelocityCheck(b);
        else if(mode == 2) ok = naiveWithEarlyStop(b);
        else ok = smart(b);
        if(ok) caught++;
    }

    cout << caught << "\n";
    return 0;
}
