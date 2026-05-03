#pragma once

struct Parameters {   
    double Kmax;
    int Kpoints;
    double D; 
    double dTheta;
    int thetaPoints;
    double minimumFrequency;
    double maximumFrequency;
    double dFrequency;
    int frequencyPoints;
    double Tmax;
};

Parameters loadParameters();