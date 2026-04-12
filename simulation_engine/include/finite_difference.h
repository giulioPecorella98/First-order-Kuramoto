#pragma once
#include <vector>
using Grid = std::vector<double>;

void finiteDifference(const Grid& f, Grid& fnew, int thetaPoints, 
                      double dTheta, double D, double dt, double K, double omega);
