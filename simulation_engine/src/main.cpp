#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters.h"
#include "save.h"
#include <thread>
#include <algorithm>


int main() {

   /* std::cout << "=======================================================================================" << std::endl;
    std::cout << "|           Welcome to the first order mean field Kuramoto model simulator!           |" << std::endl;
    std::cout << "=======================================================================================" << std::endl;
    std::cout << " Here you have the opportunity to simulate the dynamics of the first order Kuramoto" << std::endl;
    std::cout << " model for identical oscillators, choosing the initial distribution of the oscillators," << std::endl;
    std::cout << " the noise level D, the coupling constant K and the natural frequency omega. " << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;
    */
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
	
    size_t tPoints = solution.size();
    saveSolution(solution, p.thetaPoints, tPoints, p.T);    // Option to save the solution in a binary file for data analysis

    return 0;
}
