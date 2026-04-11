#include "finite_difference.h"
#include "initial_conditions.h"
#include "plot.h"
#include "parameters.h"
#include "save.h"
#include <thread>
#include <algorithm>


int main() {

    std::cout << "===============================================================================" << std::endl;
    std::cout << "|       Welcome to the first order mean field Kuramoto model simulator!       |" << std::endl;
    std::cout << "===============================================================================" << std::endl;
    std::cout << " Here you have the opportunity to simulate the dynamics of the first order " << std::endl;
    std::cout << " Kuramoto model for identical oscillators " << std::endl;
    std::cout << " \n d/dt rho(theta,t) = D * d^2/dtheta^2 rho(theta,t) - d/dtheta [rho(theta,t) *" << std::endl;
    std::cout << " (omega + K * int_0^{2pi} sin(theta' - theta) * rho(theta',t) dtheta')],\n" << std::endl;
    std::cout << " choosing the initial distribution of the oscillators, the noise level D," << std::endl;
    std::cout << " the coupling constant K and the natural frequency omega. " << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;
    
    // Load parameters
    Parameters p = loadParameters();

    // Allocate vectors for the finite difference scheme
    Grid f(p.thetaPoints);        // Solution
    Grid fnew(p.thetaPoints);     // Auxiliary vector

    // Apply the initial conditions and start the simulation
    initialConditions(f, p.thetaPoints, p.dTheta);
    std::vector<Grid> solution;      // Vector to store the solution at each frame
    double nextFrameTime = 0.0;
    double maxDensity = *std::max_element(f.begin(), f.end());
    for (int t = 0; t < p.steps; ++t) {
        if (t % p.frameCount == 0) {
            int progress = static_cast<int>((static_cast<double>(t) / p.steps) * 100);
            std::string bar(progress, '=');
            bar.append(100 - progress, ' ');
            std::cout << "\rComputing: [" << bar << "] " << progress << "%" << std::flush;
        }

        // Compute the solution at each time step
        finiteDifference(f, fnew, p.thetaPoints, p.dTheta, p.D, p.dt, p.K, p.omega);
        std::swap(f, fnew); 
        // Update the maximum of the density for plotting purposes
        double currentMaxDensity = *std::max_element(f.begin(), f.end());
        if (currentMaxDensity > maxDensity) {
            maxDensity = currentMaxDensity;
        }
        // Save the solution a the specified frame intervals
        double currentTime = t * p.dt;
        if ((currentTime >= nextFrameTime) || (t == p.steps - 1)) {
            solution.push_back(f); 
            nextFrameTime += p.frameInterval;
        }
    }
    std::cout << "\rComputing: [" << std::string(100, '=') << "] 100%" << std::endl;
    std::cout << "Simulation completed successfully." << std::endl;

    // Start gnuplot and plot the solution
    FILE* gp = startGnuplot(maxDensity);
    if (gp) { 
    double currentTime = 0.0;
        for (Grid f : solution) {
            printSolution(f, currentTime, gp, p.thetaPoints, p.dTheta);
            currentTime += p.frameInterval;
            std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(p.frameInterval)));
        }
    }
	
    size_t tPoints = solution.size();
    saveSolution(solution, p.thetaPoints, tPoints);    // Option to save the solution in a binary file for data analysis
    std::cout << "Hope you enjoyed, bye!" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::cin.get();
    pclose(gp);

    return 0;
}
