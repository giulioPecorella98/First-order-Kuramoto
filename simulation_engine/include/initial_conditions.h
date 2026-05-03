#pragma once
#include <vector>
#include <cmath>
#include <iostream>
using Frequency = std::vector<double>;
using Grid = std::vector<Frequency>;

void initialConditions(Grid& f, Frequency& g, 
                       int thetaPoints, double dTheta, 
                       int frequencyPoints, double dFrequency, double minimumFrequency, double maximumFrequency);