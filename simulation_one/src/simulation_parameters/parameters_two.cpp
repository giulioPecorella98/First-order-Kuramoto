/*
Function to load the parameters of the simulation by asking the user to input them in the terminal. 
The function also computes some derived parameters such as the time step dt, the number of steps, 
and the number of frames to be plotted, by taking care of the stability condition for the finite difference scheme.
*/
#include "parameters_two.h"
#include <iostream>
#include <limits>
#include <algorithm>

Parameters loadParameters() {
    
    double PI = 3.14159265358979323846;

    std::cout << "\nParameters acquisitions." << std::endl;
    double Kmax;
    std::cout << "1) Enter the maximum coupling strength: ";
    std::cin >> Kmax;
    while (Kmax < 0 || std::cin.fail()) {
        std::cout << "Invalid choice. The maximum coupling strength must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> Kmax;
    }
    double D; 
    std::cout << "2) Enter the noise level D: ";
    std::cin >> D;  
    while ((D < 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The noise level must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> D;
    }  
    double dTheta;
    std::cout << "3) Enter the space discretization: ";
    std::cin >> dTheta; 
    while ((dTheta <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The space discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dTheta;
    }     
    double minimumFrequency;
    std::cout << "4) Enter the minimum natural frequency of the oscillators: ";
    std::cin >> minimumFrequency; 
    while (std::cin.fail()) {
        std::cout << "Invalid choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> minimumFrequency;
    }      
    double maximumFrequency;
    std::cout << "5) Enter the maximum natural frequency of the oscillators: ";
    std::cin >> maximumFrequency; 
    while (std::cin.fail() || (maximumFrequency < minimumFrequency)) {
        std::cout << "Invalid choice. Remember that the minimum frequency is at most equal to the maximum frequency: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> maximumFrequency;
    }
    double dOmega;
    std::cout << "6) Enter the natural frequency discretization: ";
    std::cin >> dOmega; 
    while ((dOmega <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The natural frequency discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dOmega;
    }

    int thetaPoints = static_cast<int>((2 * PI / dTheta) + 1);
    dTheta = 2 * PI / (thetaPoints - 1);
    int omegaPoints = static_cast<int>(((maximumFrequency - minimumFrequency) / dOmega) + 1);
    dOmega = (maximumFrequency - minimumFrequency) / (omegaPoints - 1);
    double omegaMax = std::max(std::abs(minimumFrequency), std::abs(maximumFrequency));
    
    return {Kmax, D, dTheta, thetaPoints, minimumFrequency, maximumFrequency, dOmega, omegaPoints};
}