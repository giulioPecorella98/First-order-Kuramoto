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
    #ifdef _WIN32  
    // Windows version
    if (system("gnuplot --version >nul 2>&1") != 0) {
        std::cerr << "Gnuplot not available!\n";
        return nullptr;
    }
    FILE* gp = _popen("gnuplot", "w");
    #else
    // Linux/macOS version
    if (system("which gnuplot > /dev/null 2>&1") != 0) {
        std::cerr << "Gnuplot not available!\n";
        return nullptr;
    }
    FILE* gp = popen("gnuplot", "w");
    #endif
    
    if(!gp){
        std::cerr << "Error opening gnuplot! \n";
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
