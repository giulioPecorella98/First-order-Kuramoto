#include "order_parameter.h"

OrderParameter computeR (Density& f, Frequency& g, 
                         int thetaPoints, double dTheta, 
                         std::vector<double>& cosine, std::vector<double>& sine,
                         int frequencyPoints, double dFrequency) {
    
    double Rcos = 0.0, Rsin = 0.0, R = 0.0;
    for (int i = 0; i < frequencyPoints; i++) {
        for (int j = 0; j < thetaPoints; j++) {
            Rcos += cosine[j] * f[i][j] * g[i];
            Rsin += sine[j] * f[i][j] * g[i];
        }
    }
    Rcos *= dTheta * dFrequency;
    Rsin *= dTheta * dFrequency;
    R = sqrt(Rcos * Rcos + Rsin * Rsin);
    
    return {Rcos, Rsin, R};
}