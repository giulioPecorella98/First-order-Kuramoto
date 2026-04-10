/*
Function  to handle the plotting of the solution using gnuplot.
*/
#include "plot.h"
constexpr double PI = 3.14159265358979323846;

void printSolution(const Grid& f, double currentTime, FILE* gp, int thetaPoints, double dTheta) {
    fprintf(gp, "plot '-' with lines lw 2 title 'Time: %.3f'\n", currentTime);
        for(int i = 0; i < thetaPoints; i++){
            fprintf(gp, "%f %f\n", i * dTheta, f[i]);
        }
    fprintf(gp, "e\n");
    fflush(gp);
}

FILE* startGnuplot(double maxDensity) {
    FILE* gp = popen("gnuplot", "w");
    if(!gp){
        std::cerr << "Error opening gnuplot pipe\n";
        return nullptr;
    }
    fprintf(gp, "set xrange [0:%f]\n", 2 * PI);
    fprintf(gp, "set yrange [0:%f]\n", maxDensity * 1.1);
    fprintf(gp, "set xlabel 'Theta'\n");
    fprintf(gp, "set ylabel 'Density'\n");
    fprintf(gp, "set title 'Time evolution'\n");
    fflush(gp);
    return gp;
}
