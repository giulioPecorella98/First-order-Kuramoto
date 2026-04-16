#include "order_parameter.h"

OrderParameter computeR (Grid f, Frequency g, int thetaPoints, int omegaPoints, double dTheta, double dOmega) {
    double Rcos = 0.0, Rsin = 0.0, R = 0.0;
    for (int i = 0; i < thetaPoints; i++) {
        double psi = i * dTheta;
        for (int j = 0; j < omegaPoints; j++) {
            Rcos += cos(psi) * f[i][j] * g[j];
            Rsin += sin(psi) * f[i][j] * g[j];;
        }
    }
    Rcos *= dTheta * dOmega;
    Rsin *= dTheta * dOmega;
    R = sqrt(Rcos * Rcos + Rsin * Rsin);
    
    return {Rcos, Rsin, R};
}