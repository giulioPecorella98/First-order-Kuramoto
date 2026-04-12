// Function to set the initial conditions of the simulation.
#include "initial_conditions.h"

void initialConditions(Grid& f, int thetaPoints, double dTheta) {
    double PI = 3.14159265358979323846;
    std::cout << "Please choose one of the following initial conditions:\n";
    std::cout << "1. n-modal Gaussian\n";
    std::cout << "2. to be implemented\n";
    std::cout << "Enter your choice (1 or 2): ";
    int choice;
    std::cin >> choice;
    while ((choice < 1) || (choice > 2) || (std::cin.fail())) {
        std::cout << "Invalid choice. Please enter 1 or 2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;
    }
    double sum = 0;
    switch (choice) {
        case 1:
            std::cout << "Please choose the number of modes: ";
            int n;
            std::cin >> n;
            while ((n < 1) || (std::cin.fail())) {
                std::cout << "Invalid choice. Please enter the number of modes: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> n;
            }
            double mean, variance, amplitude;
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
                    f[i] += amplitude * std::exp(-diff * diff / (2 * variance));
                }
            }
            for (int i = 0; i < thetaPoints; i++) { sum += f[i]; }
            for (int i = 0; i < thetaPoints; i++) {
                f[i] /= (sum * dTheta);
            }
            break;
        case 2:
            std::cout << "This initial condition is not implemented yet, the uniform distribution will be used instead. " << std::endl;
            for (int i = 0; i < thetaPoints; ++i) {
                f[i] = 1.0; 
                sum += f[i];
            }
            break;
        default:
            break;
    }
    // Normalize the distribution
    for (int i = 0; i < thetaPoints; ++i) {
        f[i] /= (sum * dTheta);
    }
}

