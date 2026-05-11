#include "order_parameter.h"

OrderParameter computeR (Density& f, Frequency& g, 
                         int thetaPoints, double dTheta, 
                         int frequencyPoints, double dFrequency) {
    
    double Rcos = 0.0, Rsin = 0.0, R = 0.0;
    double psi;
    std::vector<double> cosPsi(thetaPoints);
    std::vector<double> sinPsi(thetaPoints);

    for (int j = 0; j < thetaPoints; j++) {
        psi = j * dTheta;
        cosPsi[j] = cos(psi);
        sinPsi[j] = sin(psi);
    }
    for (int i = 0; i < frequencyPoints; i++) {
        for (int j = 0; j < thetaPoints; j++) {
            Rcos += cosPsi[j] * f[i][j] * g[i];
            Rsin += sinPsi[j] * f[i][j] * g[i];
        }
    }
    Rcos *= dTheta * dFrequency;
    Rsin *= dTheta * dFrequency;
    R = sqrt(Rcos * Rcos + Rsin * Rsin);
    
    return {Rcos, Rsin, R};
}