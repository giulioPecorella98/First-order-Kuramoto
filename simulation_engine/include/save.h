#pragma once
#include "initial_conditions.h"
#include <filesystem>

void saveSolution(std::vector<Grid>& solution, Frequency& g, std::vector<double>& orderParameter,
                  int thetaPoints, int omegaPoints, int tPoints, 
                  double minimumFrequency, double maximumFrequency, double T, double D, double K) ;
