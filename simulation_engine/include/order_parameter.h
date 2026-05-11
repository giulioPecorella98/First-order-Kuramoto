#pragma once
#include <cmath>
#include <vector>
using Frequency = std::vector<double>;
using Density = std::vector<std::vector<double>>;

struct OrderParameter {
    double Rcos;
    double Rsin;
    double R;
};

OrderParameter computeR (Density& f, Frequency& g,  
                         int thetaPoints, double dTheta,
                         int frequencyPoints, double dFrequency); 