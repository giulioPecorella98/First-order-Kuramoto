#include "finite_difference.h"
#include "initial_conditions.h"
#include "parameters_two.h"
#include "order_parameter.h"
#include <filesystem>



int main() {

    // Enter the name of the binary file where the result will be saved
    std::cout << "Enter the name of the file where you want to save the result (print 's' to see saved files): ";
    std::string filename;
    std::cin >> filename;
    while (filename == "s") {

        std::filesystem::path savedDir = std::filesystem::path(PROJECT_ROOT) / "save" / "order_parameter";
        if (std::filesystem::exists(savedDir) && std::filesystem::is_directory(savedDir)) {           
            std::cout << "Saved files in 'save/order_parameter' directory:" << std::endl;
            for (const auto& entry : std::filesystem::directory_iterator(savedDir)) {
                if (entry.is_regular_file()) {
                    std::cout << " - " << entry.path().filename().string() << std::endl;
                }
            }
        } 
        else { std::cout << "No saved files found." << std::endl; } 
        std::cout << "Enter the name of the file where you want to save the result (print 's' to see saved files): ";
        std::cin >> filename;
    }

    std::filesystem::path fullpath = std::filesystem::path(PROJECT_ROOT) / "save" / "order_parameter" / (filename);
    std::filesystem::create_directories(fullpath.parent_path()); // Ensure the directory exists
    FILE* file = fopen(fullpath.string().c_str(), "wb");
    if (!file) {
        std::cerr << "Error: file not saved. "<< std::endl;
        return 1; 
    } 

    Parameters p = loadParameters();                                        // Load parameters 
    fwrite(&p.Kpoints, sizeof(int), 1, file);
    fwrite(&p.Kmax, sizeof(double), 1, file);

    Grid f(p.thetaPoints, std::vector<double>(p.omegaPoints, 0.0));         // Solution vector
    Grid fnew(p.thetaPoints,  std::vector<double>(p.omegaPoints, 0.0));     // Auxiliary vector
    Frequency g(p.omegaPoints, 0.0);                                        // Vector of natural frequencies

    // Apply the initial conditions and run the simulation for different values of K, saving the order parameter R for each value of K in the binary file
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequency, p.maximumFrequency); 
    OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega);              
    double multiplyFactor = p.Kmax / (p.Kpoints - 1);       
    double maxOmega = std::max(std::abs(p.minimumFrequency), std::abs(p.maximumFrequency));
    double R = ordR.R;
    double Rnew;
    double Rold;
    int progress;
    int steps;
    int asymptotic;
    double K;
    double dt; 
    Grid fInitial = f;
    Grid fnewInitial = fnew;

    for (int i = 0; i < p.Kpoints; i++) {

        f = fInitial;
        fnew = fnewInitial;
        Rold = R;
        K = i * multiplyFactor;
        dt = std::min((0.9 * (p.dTheta * p.dTheta) / (p.D + (K + maxOmega) * p.dTheta + K * p.dTheta * p.dTheta)), 0.1);
        steps = static_cast<int>(p.Tmax / dt) + 1;
        asymptotic = 0;

        for (int t = 0; t < steps; t++) {
           
            // Compute the solution at each time step
            finiteDifference(f, fnew, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequency, p.maximumFrequency, dt, p.D, K);
            std::swap(f, fnew); 
            OrderParameter ordRnew =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega); 
            Rnew = ordRnew.R;

            // Check if the order parameter has reached an asymptotic value, in which case we can stop the simulation and save the result
            if (std::abs(Rnew - Rold) < 0.000001) { asymptotic ++; }
            else { asymptotic = 0; }
            if (asymptotic == 100) { 
                fwrite(&Rnew, sizeof(double), 1, file);   
                break; 
            }   
            else if (t == steps - 1) { 
                fwrite(&Rnew, sizeof(double), 1, file); 
                std::cout << "\nWarning: the order parameter did not reach an asymptotic value for K = " << K << " ." << std::endl;
                std::cout << "Consider increasing the maximum simulation time." << std::endl;
            }
            std::swap(Rold, Rnew); 
        }

        // Simulation progress bar
        progress = static_cast<int>((K / p.Kmax) * 100);
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
