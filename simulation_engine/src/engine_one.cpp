#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters_one.h"
#include "order_parameter.h"
#include "saving_density.h"



int main() {
    // Enter the name of the binary file where the result will be saved, and save the result in a binary file in the "save/density" subfolder
    std::string filename = save();
    if (filename == "quit") {
        std::cout << "Returning to the main menu..." << std::endl;
        return 0;
    }
    std::filesystem::path fullpath = std::filesystem::path(PROJECT_ROOT) / "save" / "density" / (filename);
    FILE* file = fopen(fullpath.string().c_str(), "wb");
    if (!file) {
        std::cerr << "Error: file not saved. "<< std::endl;
        return 1; 
    } 

    Parameters p = loadParameters();                                            // Load parameters
    Grid f(p.thetaPoints, Frequency(p.frequencyPoints, 0.0));         // Solution vector
    Grid fnew(p.thetaPoints,  Frequency(p.frequencyPoints, 0.0));     // Auxiliary vector
    Frequency g(p.frequencyPoints, 0.0);                                        // Vector of natural frequencies

    fwrite(&p.thetaPoints, sizeof(int), 1, file);
    fwrite(&p.frequencyPoints, sizeof(int), 1, file);
    fwrite(&p.minimumFrequency, sizeof(double), 1, file);
    fwrite(&p.maximumFrequency, sizeof(double), 1, file);
    fwrite(&p.T, sizeof(double), 1, file);
    fwrite(&p.D, sizeof(double), 1, file);
    fwrite(&p.K, sizeof(double), 1, file);

    // Apply the initial conditions
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.frequencyPoints, p.dFrequency, p.minimumFrequency, p.maximumFrequency);
    fwrite(g.data(), sizeof(double), p.frequencyPoints, file);
    for (const auto& phase : f) {                                       
        fwrite(phase.data(), sizeof(double), p.frequencyPoints, file);    
    }
    OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.frequencyPoints, p.dTheta, p.dFrequency); 
    fwrite(&ordR.R, sizeof(double), 1, file);

    // Start the simulation
    double updateTime = 0.0;
    for (int t = 0; t < p.steps; t++) {

        // Compute the solution at each time step
        finiteDifference(f, fnew, g, p.thetaPoints, p.dTheta, p.frequencyPoints, p.dFrequency, p.minimumFrequency, p.dt, p.D, p.K);
        std::swap(f, fnew); 
        
        // Save the solution a the specified frame intervals
        updateTime += p.dt;
        if (static_cast<int>(updateTime / p.frameInterval) >= 1) {
            for (const auto& phase : f) {
                fwrite(phase.data(), sizeof(double), p.frequencyPoints, file);
            }
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.frequencyPoints, p.dTheta, p.dFrequency);
            fwrite(&ordR.R, sizeof(double), 1, file);
            updateTime = 0.0;
        }
        else if (t == p.steps - 1) {
            for (const auto& phase : f) {                                     
                fwrite(phase.data(), sizeof(double), p.frequencyPoints, file); 
            }
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.frequencyPoints, p.dTheta, p.dFrequency); 
            fwrite(&ordR.R, sizeof(double), 1, file);
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
    fclose(file);
    std::cout << "Result saved successfully in " << fullpath.generic_string() << "." << std::endl;

    return 0;
}
