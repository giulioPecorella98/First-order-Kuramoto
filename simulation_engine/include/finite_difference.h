#pragma once
#include "order_parameter.h"

void finiteDifference(Density& f, Density& fnew, Frequency& g, 
                      int thetaPoints, double dTheta, 
                      int frequencyPoints, double dFrequency, double minimumFrequency,
                      double dt, double D, double K, double alpha);
