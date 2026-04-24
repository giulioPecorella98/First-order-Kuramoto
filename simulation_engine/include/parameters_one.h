#pragma once

struct Parameters {
    double T;   
    double D; 
    double K; 
    double dTheta;
    int thetaPoints;
    double minimumFrequency;
    double maximumFrequency;
    double dOmega;
    int omegaPoints;
    double dt;
    int steps;
    double frameInterval;
};

Parameters loadParameters();