// Function for computing the solution to the mean field Kuramoto PDE with noise level D. 

#include "finite_difference.h"
#include <cmath>

void finiteDifference(const Grid& f, Grid& fnew, Frequency& g,
                      int thetaPoints, double dTheta, 
                      int omegaPoints, double dOmega, double minimumFrequency, double maximumFrequecy, 
                      double dt, double D, double K) {

    // Compute the order parameter R and the mean phase psi
    double Rcos = 0.0, Rsin = 0.0;
    for (int i = 0; i < thetaPoints; i++) {
        double psi = i * dTheta;
        for (int j = 0; j < omegaPoints; j++) {
            Rcos += cos(psi) * f[i][j] * g[j];
            Rsin += sin(psi) * f[i][j] * g[j];;
        }
    }
    Rcos *= dTheta * dOmega;
    Rsin *= dTheta * dOmega;

    for (int j = 0; j < omegaPoints; ++j) {

        double omega = minimumFrequency + j * dOmega;
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
                fnew[i][j] = f[iNext][j] * (dt * D / 2 / dTheta / dTheta - fConvSin * dt / dTheta) 
                        + f[i][j] * (1 - dt * D / dTheta / dTheta + dt / dTheta * fConvSin + dt * fConvCos)
                        + f[iPrev][j] * (dt * D / 2 / dTheta / dTheta); 
            }
            else {
                fnew[i][j] = f[iNext][j] * (dt * D / 2 / dTheta / dTheta)
                        + f[i][j] * (1 - dt * D / dTheta / dTheta - dt / dTheta * fConvSin + dt * fConvCos)
                        + f[iPrev][j] * (dt * D / 2 / dTheta / dTheta + fConvSin * dt / dTheta);
            }
        }
    }

    // Mass conservation for every natural frequency Omega
    double sum = 0;
    for (int j = 0; j < omegaPoints; ++j) {
        for (int i = 0; i < thetaPoints; ++i) {
            sum += fnew[i][j];
        }
        for (int i = 0; i < thetaPoints; ++i) {
            fnew[i][j] /= (sum * dTheta);
        }
        sum = 0;
    }
}