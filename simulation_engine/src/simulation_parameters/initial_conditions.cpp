// Function to set the initial conditions of the simulation.
#include "initial_conditions.h"

void initialConditions(Grid& f, Frequency& g, int thetaPoints, double dTheta, int omegaPoints, double dOmega, double minimumFrequency, double maximumFrequecy) {   
    double PI = 3.14159265358979323846;
    int choice, n;
    double mean, variance, amplitude;
    double sum = 0;
    
    g = Frequency(omegaPoints, 0.0);
    std::cout << "Please choose one of the following natural frequency initial conditions:" << std::endl;
    std::cout << "1. n-modal Gaussian-type" << std::endl;
    std::cout << "2. uniform distribution" << std::endl;
    std::cout << "3. to be implemented" << std::endl; 
    std::cout << "Enter your choice (1, 2 or 3): ";
    std::cin >> choice;
    while ((choice < 1) || (choice > 3) || (std::cin.fail())) {
        std::cout << "Invalid choice. Please enter 1, 2 or 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;
    }
    switch (choice) {
        case 1:
            std::cout << "Please choose the number of modes: ";
            std::cin >> n;
            while ((n < 1) || (std::cin.fail())) {
                std::cout << "Invalid choice. Please enter the number of modes: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> n;
            }
            for (int mode = 0; mode < n; ++mode) {
                std::cout << "Enter the mean of the distribution for mode " << mode + 1 << ": ";
                std::cin >> mean;
                while ((mean < minimumFrequency) || (mean > maximumFrequecy) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The mean must be in the interval [" << minimumFrequency << ", " << maximumFrequecy << "]: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> mean;
                }
                std::cout << "Enter the variance of the distribution for mode " << mode + 1 << ": ";
                std::cin >> variance;
                while ((variance <= 0) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The variance must be a positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> variance;
                }
                std::cout << "Enter the amplitude of the distribution for mode " << mode + 1 << ": ";
                std::cin >> amplitude;  
                while ((amplitude < 0) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The amplitude must be a positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> amplitude;
                }
                for (int j = 0; j < omegaPoints; j++) {
                    double omega = minimumFrequency + j * dOmega;
                    double diff = omega - mean;
                    g[j] += amplitude * std::exp(- diff * diff / (2 * variance));
                }    
            }
            break;
        case 2:
            for (int j = 0; j < omegaPoints; j++) {
                g[j] = 1.0; 
            }
            break;
        case 3:
            std::cout << "This initial condition is not implemented yet, the uniform distribution will be used instead. " << std::endl;
            for (int j = 0; j < omegaPoints; j++) {
                g[j] = 1.0; 
            }
            break;
        default:
            break;
    }
    // Normalization
    for (int j = 0; j < omegaPoints; j++) { sum += g[j]; }
    for (int j = 0; j < omegaPoints; j++) { g[j] /= (sum * dOmega); }

    f = Grid(thetaPoints, std::vector<double>(omegaPoints, 0.0));  
    std::cout << "Please choose one of the following initial conditions for the density:" << std::endl;
    std::cout << "1. n-modal Gaussian-type in phase, uniform in natural frequency" << std::endl;
    std::cout << "2. to be implemented" << std::endl;
    std::cout << "Enter your choice (1 or 2): ";
    std::cin >> choice;
    while ((choice < 1) || (choice > 2) || (std::cin.fail())) {
        std::cout << "Invalid choice. Please enter 1 or 2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;
    }
    switch (choice) {
        case 1:
            std::cout << "Please choose the number of modes: ";
            std::cin >> n;
            while ((n < 1) || (std::cin.fail())) {
                std::cout << "Invalid choice. Please enter the number of modes: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> n;
            }
            for (int mode = 0; mode < n; ++mode) {
                std::cout << "Enter the mean of the distribution for mode " << mode + 1 << ": ";
                std::cin >> mean;
                while ((mean < 0) || (mean > 2 * PI) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The mean must be in the interval [0,2pi]: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> mean;
                }
                std::cout << "Enter the variance of the distribution for mode " << mode + 1 << ": ";
                std::cin >> variance;
                while ((variance <= 0) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The variance must be a positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> variance;
                }
                std::cout << "Enter the amplitude of the distribution for mode " << mode + 1 << ": ";
                std::cin >> amplitude;  
                while ((amplitude < 0) || (std::cin.fail())) {
                    std::cout << "Invalid choice. The amplitude must be a positive number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> amplitude;
                }
                for (int i = 0; i < thetaPoints; i++) {
                    double theta = i * dTheta;
                    double diff = theta - mean;
                    if (diff > PI) {diff -= 2*PI;}
                    else if (diff < -PI) {diff += 2*PI;}
                    for (int j = 0; j < omegaPoints; j++) {
                        f[i][j] += amplitude * std::exp(-diff * diff / (2 * variance));
                    }    
                }
            }
            break;
        case 2:
            std::cout << "This initial condition is not implemented yet, the uniform distribution will be used instead. " << std::endl;
            for (int j = 0; j < omegaPoints; j++) {
                 for (int i = 0; i < thetaPoints; i++) {
                    f[i][j] = 1.0; 
                }
            }
            break;
        default:
            break;
    }
    // Normalization for every natural frequency Omega
    sum = 0;
    for (int j = 0; j < omegaPoints; ++j) {
        for (int i = 0; i < thetaPoints; ++i) { sum += f[i][j]; }
        for (int i = 0; i < thetaPoints; ++i) { f[i][j] /= (sum * dTheta); }
        sum = 0;
    }
}

