#pragma once
#include <cmath>
#include <vector>
using Frequency = std::vector<double>;
using Grid = std::vector<Frequency>;

struct OrderParameter {
    double Rcos;
    double Rsin;
    double R;
};

OrderParameter computeR (Grid& f, Frequency& g,  
                         int thetaPoints, int frequencyPoints, 
                         double dTheta, double dFrequency); 