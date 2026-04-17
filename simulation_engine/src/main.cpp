#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters.h"
#include "order_parameter.h"
#include "save.h"



int main() {
    // Load parameters
    Parameters p = loadParameters();

    Grid f(p.thetaPoints, std::vector<double>(p.omegaPoints, 0.0));         // Solution vector
    Grid fnew(p.thetaPoints,  std::vector<double>(p.omegaPoints, 0.0));     // Auxiliary vector
    Frequency g(p.omegaPoints);                                        // Vector of natural frequencies

    // Apply the initial conditions
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequecy, p.maximumFrequecy);
    std::vector<Grid> solution;      // Vector to store the solution at each frame
    std::vector<double> ordPar;      // Vector to store the order parameter at each frame
    solution.push_back(f);
    OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega); 
    ordPar.push_back(ordR.R);

    // Start the simulation
    double updateTime = 0.0;
    for (int t = 0; t < p.steps; t++) {

        // Compute the solution at each time step
        finiteDifference(f, fnew, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequecy, p.maximumFrequecy, p.dt, p.D, p.K);
        std::swap(f, fnew); 
        // Save the solution a the specified frame intervals
        updateTime += p.dt;
        if (static_cast<int>(updateTime / p.frameInterval) >= 1) {
            solution.push_back(f); 
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega); 
            ordPar.push_back(ordR.R);
            updateTime = 0.0;
        }
        else if (t == p.steps - 1) {
            solution.push_back(f); 
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega); 
            ordPar.push_back(ordR.R);
        }

        // Simulation progress bar
        int progress = static_cast<int>((static_cast<double>(t) / p.steps) * 100);
        if (progress >= 1) {
            std::string bar(progress, '=');
            bar.append(100 - progress, ' ');
            std::cout << "\rComputing: [" << bar << "] " << progress << "%" << std::flush;
        }
    }
    std::cout << "\rComputing: [" << std::string(100, '=') << "] 100%" << std::endl;
    std::cout << "Simulation completed successfully." << std::endl;
	
    int tPoints = static_cast<int>(solution.size());
    saveSolution(solution, g, ordPar, p.thetaPoints, p.omegaPoints, tPoints, p.minimumFrequecy, p.maximumFrequecy, p.T, p.D, p.K); 

    return 0;
}
