#pragma once
#include <cstdio>  
#include <cstdlib>
#include <vector>
#include <iostream>
using Grid = std::vector<double>;

void printSolution(const Grid& f, double currentTime, FILE* gp, int thetaPoints, double dTheta);
FILE* startGnuplot(double maxDensity);