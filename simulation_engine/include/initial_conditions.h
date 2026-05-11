#pragma once
#include <vector>
#include <cmath>
#include <iostream>
using Frequency = std::vector<double>;
using Density = std::vector<std::vector<double>>;

void initialConditions(Density& f, Frequency& g, 
                       int thetaPoints, double dTheta, 
                       int frequencyPoints, double dFrequency, double minimumFrequency, double maximumFrequency);