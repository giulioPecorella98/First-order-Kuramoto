#pragma once
#include "initial_conditions.h"
#include <filesystem>

void saveSolution(const std::vector<Grid>& solution, Frequency& g, 
                  int thetaPoints, int omegaPoints, int tPoints, 
                  double minimumFrequency, double maximumFrequency, double T);
