/* 
Function for computing the solution to the mean field Kuramoto PDE with noise level D and coupling strength K,
using a Lax-Friedrichs scheme and taking care of the periodic boundary conditions.
*/
#include "finite_difference.h"

void finiteDifference(Density& f, Density& fnew, Frequency& g,
                      std::vector<double>& cosine, std::vector<double>& sine,
                      std::vector<int>& jNext, std::vector<int>& jPrev,
                      std::vector<double>& freq,
                      int thetaPoints, double dTheta, 
                      int frequencyPoints, double dFrequency, double minimumFrequency,
                      double dt, double D, double K, double alpha) {

    OrderParameter r =  computeR(f, g, thetaPoints, dTheta, cosine, sine, frequencyPoints, dFrequency);
    const double C1 = 1 - 2 * D * dt / dTheta / dTheta - alpha * dt / dTheta;
    const double C2 = D * dt / dTheta / dTheta + alpha / 2 * dt / dTheta;
    const double C3 = dt / dTheta / 2;

    double KRcos, KRsin;
    std::vector<double> fconvSin(thetaPoints);
    KRcos = K * r.Rcos;
    KRsin = K * r.Rsin;
    for (int j = 0; j < thetaPoints; j++) {
        fconvSin[j] = KRsin * cosine[j] - KRcos * sine[j];
    }   
    for (int i = 0; i < frequencyPoints; i++) {
        for (int j = 0; j < thetaPoints; j++) {    
            fnew[i][j] = f[i][j] * C1 + C2 * (f[i][jNext[j]] + f[i][jPrev[j]]) 
                         - C3 * (f[i][jNext[j]] * (fconvSin[jNext[j]] + freq[i]) 
                         - f[i][jPrev[j]] * (fconvSin[jPrev[j]]+ freq[i]));
        }
    }
}