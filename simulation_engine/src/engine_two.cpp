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
        std::filesystem::path savedDir = std::filesystem::path(PROJECT_ROOT) / "saved_order_parameters";
        if (std::filesystem::exists(savedDir) && std::filesystem::is_directory(savedDir)) {
            std::cout << "Saved files in 'saved_order_parameters' directory:" << std::endl;
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
    //save the result in a binary file in the "saved_order_parameters" subfolder
    std::filesystem::path fullpath = std::filesystem::path(PROJECT_ROOT) / "saved_order_parameters" / (filename);
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
    Frequency g(p.omegaPoints, 0.0);                                       // Vector of natural frequencies

    int Kpoints = 100;
    fwrite(&Kpoints, sizeof(int), 1, file);
    std::vector<double> K(Kpoints, 0.0);
    for (int i = 0; i < Kpoints; i++) {
        K[i] = i * p.Kmax / (Kpoints - 1);
    }
    fwrite(K.data(), sizeof(double), Kpoints, file);

    // Apply the initial conditions and run the simulation for different values of K, saving the order parameter R for each value of K in the binary file
    initialConditions(f, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequency, p.maximumFrequency); 
    OrderParameter ordR =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega);                                
    for (int i = 0; i < Kpoints; i++) {
        double Tmax = 10.0;
        double dt = std::min((0.9 * (p.dTheta * p.dTheta) / (p.D + (K[i] + p.maximumFrequency) * p.dTheta + K[i] * p.dTheta * p.dTheta)), 0.1);
        int steps = static_cast<int>(Tmax / dt) + 1;
        double R = ordR.R;
        double Rnew;
        int asymptotic = 0;
        for (int t = 0; t < steps; t++) {
            // Compute the solution at each time step
            finiteDifference(f, fnew, g, p.thetaPoints, p.dTheta, p.omegaPoints, p.dOmega, p.minimumFrequency, p.maximumFrequency, dt, p.D, K[i]);
            std::swap(f, fnew); 
            OrderParameter ordRnew =  computeR(f, g, p.thetaPoints, p.omegaPoints, p.dTheta, p.dOmega); 
            Rnew = ordRnew.R;
            if (std::abs(Rnew - R) < 0.0001) { 
                asymptotic ++; 
            }
            else { asymptotic = 0.0; }
            if (asymptotic == 10 || t == steps - 1) { 
                fwrite(&Rnew, sizeof(double), 1, file);   
                break; 
            }   
            std::swap(R, Rnew);
        }
        // Simulation progress bar
        int progress = static_cast<int>((K[i] / p.Kmax) * 100);
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
