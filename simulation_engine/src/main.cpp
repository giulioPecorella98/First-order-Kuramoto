#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters.h"
#include "save.h"
#include <thread>
#include <algorithm>



int main() {
    // Load parameters
    Parameters p = loadParameters();

    // Allocate vectors for the finite difference scheme
    Grid f(p.thetaPoints, std::vector<double>(p.omegaPoints));         // Solution
    Grid fnew(p.thetaPoints,  std::vector<double>(p.omegaPoints));     // Auxiliary vector
    Frequency g(p.omegaPoints);                                        // Vector of natural frequencies

    // Apply the initial conditions and start the simulation
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequecy, p.maximumFrequecy);
    std::vector<Grid> solution;      // Vector to store the solution at each frame
    solution.push_back(f);
    double currentTime = 0.0;
    double nextFrameTime = p.frameInterval;
    for (int t = 0; t < p.steps; ++t) {
        if (t % p.frameCount == 0) {
            int progress = static_cast<int>((static_cast<double>(t) / p.steps) * 100);
            std::string bar(progress, '=');
            bar.append(100 - progress, ' ');
            std::cout << "\rComputing: [" << bar << "] " << progress << "%" << std::flush;
        }
        // Compute the solution at each time step
        finiteDifference(f, fnew, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequecy, p.maximumFrequecy, p.dt, p.D, p.K);
        std::swap(f, fnew); 
        // Save the solution a the specified frame intervals
        currentTime += p.dt;
        if ((currentTime >= nextFrameTime) || (t == p.steps - 1)) {
            solution.push_back(f); 
            nextFrameTime += p.frameInterval;
        }
    }
    std::cout << "\rComputing: [" << std::string(100, '=') << "] 100%" << std::endl;
    std::cout << "Simulation completed successfully." << std::endl;
	
    int tPoints = static_cast<int>(solution.size());
    saveSolution(solution, g, p.thetaPoints, p.omegaPoints, tPoints, p.minimumFrequecy, p.maximumFrequecy, p.T); 

    return 0;
}
