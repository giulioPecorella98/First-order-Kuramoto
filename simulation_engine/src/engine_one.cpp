#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters_one.h"
#include "order_parameter.h"
#include <filesystem>



int main() {
    // Enter the name of the binary file where the result will be saved
    std::cout << "Enter the name of the file where you want to save the result (print 's' to see saved files): ";
    std::string filename;
    std::cin >> filename;
    while (filename == "s") {
        std::filesystem::path savedDir = std::filesystem::path(PROJECT_ROOT) / "save" / "density";
        if (std::filesystem::exists(savedDir) && std::filesystem::is_directory(savedDir)) {
            std::cout << "Saved files in 'save/density' directory:" << std::endl;
            for (const auto& entry : std::filesystem::directory_iterator(savedDir)) {
                if (entry.is_regular_file()) {
                    std::cout << " - " << entry.path().filename().string() << std::endl;
                }
            }
        } 
        else {
            std::cout << "No saved files found." << std::endl;
        } 
        std::cout << "Enter the name of the file where you want to save the result (print 's' to see saved files): ";
        std::cin >> filename;
    }
    //save the result in a binary file in the "save/density" subfolder
    std::filesystem::path fullpath = std::filesystem::path(PROJECT_ROOT) / "save" / "density" / (filename);
    std::filesystem::create_directories(fullpath.parent_path()); // Ensure the directory exists
    FILE* file = fopen(fullpath.string().c_str(), "wb");
    if (!file) {
        std::cerr << "Error: file not saved. "<< std::endl;
        return 1; 
    } 

    // Load parameters
    Parameters p = loadParameters();

    Grid f(p.thetaPoints, std::vector<double>(p.omegaPoints, 0.0));         // Solution vector
    Grid fnew(p.thetaPoints,  std::vector<double>(p.omegaPoints, 0.0));     // Auxiliary vector
    Frequency g(p.omegaPoints, 0.0);                                        // Vector of natural frequencies

    fwrite(&p.thetaPoints, sizeof(int), 1, file);
    fwrite(&p.omegaPoints, sizeof(int), 1, file);
    fwrite(&p.minimumFrequency, sizeof(double), 1, file);
    fwrite(&p.maximumFrequency, sizeof(double), 1, file);
    fwrite(&p.T, sizeof(double), 1, file);
    fwrite(&p.D, sizeof(double), 1, file);
    fwrite(&p.K, sizeof(double), 1, file);

    // Apply the initial conditions
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequency, p.maximumFrequency);
    fwrite(g.data(), sizeof(double), p.omegaPoints, file);
    for (const auto& row : f) {                                     // theta
        fwrite(row.data(), sizeof(double), p.omegaPoints, file);    // omega
    }
    OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega); 
    fwrite(&ordR.R, sizeof(double), 1, file);

    // Start the simulation
    double updateTime = 0.0;
    for (int t = 0; t < p.steps; t++) {

        // Compute the solution at each time step
        finiteDifference(f, fnew, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequency, p.maximumFrequency, p.dt, p.D, p.K);
        std::swap(f, fnew); 
        // Save the solution a the specified frame intervals
        updateTime += p.dt;
        if (static_cast<int>(updateTime / p.frameInterval) >= 1) {
            for (const auto& row : f) {
                fwrite(row.data(), sizeof(double), p.omegaPoints, file);
            }
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega);
            fwrite(&ordR.R, sizeof(double), 1, file);
            updateTime = 0.0;
        }
        else if (t == p.steps - 1) {
            for (const auto& row : f) {                                     
                fwrite(row.data(), sizeof(double), p.omegaPoints, file); 
            }
            OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega); 
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
