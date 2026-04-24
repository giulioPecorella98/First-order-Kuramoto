// Function for computing the solution to the mean field Kuramoto PDE with noise level D. 
#include "finite_difference.h"
#include "order_parameter.h"

void finiteDifference(Grid& f, Grid& fnew, Frequency& g,
                      int thetaPoints, double dTheta, 
                      int omegaPoints, double dOmega, double minimumFrequency, double maximumFrequecy, 
                      double dt, double D, double K) {

    OrderParameter r =  computeR(f, g, thetaPoints, omegaPoints, dTheta, dOmega);                  
    for (int j = 0; j < omegaPoints; ++j) {

        double omega = minimumFrequency + j * dOmega;
        for (int i = 0; i < thetaPoints; ++i) {    

            double theta = i * dTheta;
            // Compute the convolution terms by employing the order parameter R and the mean phase psi.
            double fConvSin = K * (r.Rsin * cos(theta) - r.Rcos * sin(theta)) + omega;
            double fConvCos = K * (r.Rcos * cos(theta) + r.Rsin * sin(theta));
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