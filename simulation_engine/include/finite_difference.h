#pragma once
#include "order_parameter.h"

void finiteDifference(Density& f, Density& fnew, Frequency& g, 
                      std::vector<double>& cosine, std::vector<double>& sine,
                      std::vector<int>& jNext, std::vector<int>& jPrev,
                      std::vector<double>& freq,
                      int thetaPoints, double dTheta, 
                      int frequencyPoints, double dFrequency, double minimumFrequency,
                      double dt, double D, double K, double alpha);
