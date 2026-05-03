#pragma once
#include <vector>
#include "order_parameter.h"

void finiteDifference(Grid& f, Grid& fnew, Frequency& g, 
                      int thetaPoints, double dTheta, 
                      int frequencyPoints, double dFrequency, double minimumFrequency,
                      double dt, double D, double K);
