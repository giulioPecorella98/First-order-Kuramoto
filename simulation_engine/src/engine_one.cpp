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
    Density f(p.frequencyPoints, std::vector<double>(p.thetaPoints));                // Solution vector
    Density fnew(p.frequencyPoints,  std::vector<double>(p.thetaPoints));            // Auxiliary vector
    Frequency g(p.frequencyPoints); 
    // Define some auxiliary vectors for the finite difference scheme, to avoid computing them at each time step
    double theta;
    std::vector<double> cosine(p.thetaPoints);
    std::vector<double> sine(p.thetaPoints);
    std::vector<int> jNext(p.thetaPoints);
    std::vector<int> jPrev(p.thetaPoints);
    std::vector<double> freq(p.frequencyPoints);
    for (int j = 0; j < p.thetaPoints; j++) {
        theta = j * p.dTheta;
        cosine[j] = cos(theta);
        sine[j] = sin(theta);
        jNext[j] = (j + 1 == p.thetaPoints) ? 0 : j + 1;
        jPrev[j] = (j == 0) ? p.thetaPoints - 1 : j - 1;
    }
    for (int i = 0; i < p.frequencyPoints; i++) {
        freq[i] = p.minimumFrequency + i * p.dFrequency;
    }                                       // Vector of natural frequencies

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
    for (const auto& freq : f) {                                       
        fwrite(freq.data(), sizeof(double), p.thetaPoints, file);    
    }
    OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.dTheta, cosine, sine, p.frequencyPoints, p.dFrequency); 
    fwrite(&ordR.R, sizeof(double), 1, file);

    // Start the simulation
    double updateTime = 0.0;
    std::cout << "\rComputing: [" << std::string(100, ' ') << "] 0%" << std::flush;
    for (int t = 0; t < p.steps; t++) {

        // Compute the solution at each time step
        finiteDifference(f, fnew, g, cosine, sine, jNext, jPrev, freq, p.thetaPoints, p.dTheta, 
                         p.frequencyPoints, p.dFrequency, p.minimumFrequency, p.dt, p.D, p.K, p.alpha);
        std::swap(f, fnew); 
        
        // Save the solution a the specified frame intervals
        updateTime += p.dt;
        if (static_cast<int>(updateTime / p.frameInterval) >= 1) {
            for (const auto& freq : f) {
                fwrite(freq.data(), sizeof(double), p.thetaPoints, file);
            }
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.dTheta, cosine, sine, p.frequencyPoints, p.dFrequency);
            fwrite(&ordR.R, sizeof(double), 1, file);
            updateTime = 0.0;
        }
        else if (t == p.steps - 1) {
            for (const auto& freq : f) {                                     
                fwrite(freq.data(), sizeof(double), p.thetaPoints, file); 
            }
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.dTheta, cosine, sine, p.frequencyPoints, p.dFrequency); 
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
