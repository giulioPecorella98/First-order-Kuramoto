#pragma once

struct Parameters {   
    double Kmax;
    int Kpoints;
    double D; 
    double dTheta;
    int thetaPoints;
    double minimumFrequency;
    double maximumFrequency;
    double dOmega;
    int omegaPoints;
    double Tmax;
};

Parameters loadParameters();