#pragma once
#include <vector>
using Grid = std::vector<std::vector<double>>;
using Frequency = std::vector<double>;

void finiteDifference(const Grid& f, Grid& fnew, Frequency& g,
                      int thetaPoints, double dTheta, 
                      int omegaPoints, double dOmega, double minimumFrequency, double maximumFrequecy, 
                      double dt, double D, double K);
