#pragma once
#include <vector>
#include <cmath>
#include <iostream>
using Grid = std::vector<std::vector<double>>;
using Frequency = std::vector<double>;

void initialConditions(Grid& f, Frequency& g, 
                       int thetaPoints, double dTheta, 
                       int frequencyPoints, double dFrequency, double minimumFrequency, double maximumFrequency);