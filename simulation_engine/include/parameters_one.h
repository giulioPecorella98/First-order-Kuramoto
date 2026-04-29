#pragma once

struct Parameters {
    double T;   
    double D; 
    double K; 
    double dTheta;
    int thetaPoints;
    double minimumFrequency;
    double maximumFrequency;
    double dFrequency;
    int frequencyPoints;
    double dt;
    int steps;
    double frameInterval;
};

Parameters loadParameters();