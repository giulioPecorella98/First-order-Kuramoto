// Function for computing the solution to the mean field Kuramoto PDE with noise level D and coupling strength K.
#include "finite_difference.h"

void finiteDifference(Density& f, Density& fnew, Frequency& g,
                      int thetaPoints, double dTheta, 
                      int frequencyPoints, double dFrequency, double minimumFrequency,
                      double dt, double D, double K, double alpha) {

    OrderParameter r =  computeR(f, g, thetaPoints, dTheta, frequencyPoints, dFrequency);
    const double C1 = 1 - 2 * D * dt / dTheta / dTheta - alpha * dt / dTheta;
    const double C2 = D * dt / dTheta / dTheta + alpha / 2 * dt / dTheta;
    const double C3 = dt / dTheta / 2;

    double theta, cosine, sine, KRcos, KRsin;
    std::vector<double> fconvSin(thetaPoints);
    std::vector<int> jNext(thetaPoints);
    std::vector<int> jPrev(thetaPoints);
    KRcos = K * r.Rcos;
    KRsin = K * r.Rsin;
    for (int j = 0; j < thetaPoints; j++) {
        theta = j * dTheta;
        cosine = cos(theta);
        sine = sin(theta);
        fconvSin[j] = KRsin * cosine - KRcos * sine;
        jNext[j] = (j + 1 == thetaPoints) ? 0 : j + 1;
        jPrev[j] = (j == 0) ? thetaPoints - 1 : j - 1;
    }   
    for (int i = 0; i < frequencyPoints; i++) {
        double frequency = minimumFrequency + i * dFrequency;
        for (int j = 0; j < thetaPoints; j++) {    
            /* 
            Compute the solution with a Lax-Friedrichs scheme, by taking care of the periodic boundary conditions.
            */
            fnew[i][j] = f[i][j] * C1 + C2 * (f[i][jNext[j]] + f[i][jPrev[j]]) - C3 * (f[i][jNext[j]] * (fconvSin[jNext[j]] + frequency) - f[i][jPrev[j]] * (fconvSin[jPrev[j]]+ frequency));
        }
    }
}