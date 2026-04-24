#pragma once
#include <cmath>
#include <vector>
using Grid = std::vector<std::vector<double>>;
using Frequency = std::vector<double>;

struct OrderParameter {
    double Rcos;
    double Rsin;
    double R;
};

OrderParameter computeR (Grid& f, Frequency& g,  int thetaPoints, int omegaPoints, double dTheta, double dOmega); 