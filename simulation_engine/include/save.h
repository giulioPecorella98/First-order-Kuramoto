#pragma once
#include "initial_conditions.h"
#include <filesystem>

void saveSolution(const std::vector<Grid>& solution, int thetaPoints, size_t tPoints, double T);
