/*
Function for computing the solution to the mean field Kuramoto PDE with noise level D. 
*/
#include "finite_difference.h"
#include <cmath>

void finiteDifference(const Grid& f, Grid& fnew, int thetaPoints, double dTheta, double D, double dt, double K, double omega) {
    // Compute the order parameter R and the mean phase psi
    double Rcos = 0.0, Rsin = 0.0;
    for (int j = 0; j < thetaPoints; ++j) {
        double psi = j * dTheta;
        Rcos += cos(psi) * f[j];
        Rsin += sin(psi) * f[j];
    }
    Rcos *= dTheta;
    Rsin *= dTheta;

    for (int i = 0; i < thetaPoints; ++i) {
        
        double theta = i * dTheta;
        // Compute the convolution terms by employing the order parameter R and the mean phase psi.
        double fConvSin = K * (Rsin * cos(theta) - Rcos * sin(theta)) + omega;
        double fConvCos = K * (Rcos * cos(theta) + Rsin * sin(theta));
        /* 
        Compute the solution by mixing finite difference for the diffusive term and upwind scheme 
        for the nonlinear drift, by taking care of the periodic boundary conditions.
        */
        int iNext = (i + 1) % thetaPoints;
        int iPrev = (i - 1 + thetaPoints) % thetaPoints;
        
        if (fConvSin < 0) {
            fnew[i] = f[iNext] * (dt * D / 2 / dTheta / dTheta - fConvSin * dt / dTheta) 
                    + f[i] * (1 - dt * D / dTheta / dTheta + dt / dTheta * fConvSin + dt * fConvCos)
                    + f[iPrev] * (dt * D / 2 / dTheta / dTheta); 
        }
        else {
            fnew[i] = f[iNext] * (dt * D / 2 / dTheta / dTheta)
                    + f[i] * (1 - dt * D / dTheta / dTheta - dt / dTheta * fConvSin + dt * fConvCos)
                    + f[iPrev] * (dt * D / 2 / dTheta / dTheta + fConvSin * dt / dTheta);
        }
    }

    // Mass conservation
    double mass = 0.0;
    for(int i = 0; i < thetaPoints; ++i) {
        mass += fnew[i] * dTheta;
    }
    for(int i = 0; i < thetaPoints; ++i) fnew[i] /= mass;
}