#pragma once

struct Parameters {
    double T;   
    double D; 
    double K; 
    double dTheta;
    double dt;
    int thetaPoints;
    int steps;
    double frameInterval;
    int frameCount;
    double omega;
};

Parameters loadParameters();