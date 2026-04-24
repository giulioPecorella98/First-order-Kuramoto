#pragma once

struct Parameters {   
    double Kmax;
    double D; 
    double dTheta;
    int thetaPoints;
    double minimumFrequency;
    double maximumFrequency;
    double dOmega;
    int omegaPoints;
};

Parameters loadParameters();