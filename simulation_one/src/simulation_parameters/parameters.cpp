/*
Function to load the parameters of the simulation by asking the user to input them in the terminal. 
The function also computes some derived parameters such as the time step dt, the number of steps, 
and the number of frames to be plotted, by taking care of the stability condition for the finite difference scheme.
*/
#include "parameters.h"
#include <iostream>
#include <limits>
#include <algorithm>

Parameters loadParameters() {
    
    double PI = 3.14159265358979323846;

    std::cout << "\nParameters acquisitions." << std::endl;
    double T;   
    std::cout << "1) Enter the final time T: ";
    std::cin >> T;  
    while ((T <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The final time must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> T;
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
    double K; 
    std::cout << "3) Enter the coupling constant K: ";
    std::cin >> K;
    while ((K < 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The coupling constant must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> K;
    }            
    double dTheta;
    std::cout << "4) Enter the space discretization: ";
    std::cin >> dTheta; 
    while ((dTheta <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The space discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dTheta;
    }     
    double minimumFrequency;
    std::cout << "5) Enter the minimum natural frequency of the oscillators: ";
    std::cin >> minimumFrequency; 
    while (std::cin.fail()) {
        std::cout << "Invalid choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> minimumFrequency;
    }      
    double maximumFrequency;
    std::cout << "6) Enter the maximum natural frequency of the oscillators: ";
    std::cin >> maximumFrequency; 
    while (std::cin.fail() || (maximumFrequency < minimumFrequency)) {
        std::cout << "Invalid choice. Remember that the minimum frequency is at most equal to the maximum frequency: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> maximumFrequency;
    }
    double dOmega;
    std::cout << "7) Enter the natural frequency discretization: ";
    std::cin >> dOmega; 
    while ((dTheta <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The natural frequency discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dOmega;
    }
    double framePerSeconds;
    std::cout << "8) Enter the number of frames per seconds: ";
    std::cin >> framePerSeconds;  
    while ((framePerSeconds <= 0) || (std::cin.fail()) || (framePerSeconds < (1 / T))) {
        std::cout << "Invalid choice. Try again: " << ": ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> framePerSeconds;
    }

    int thetaPoints = static_cast<int>((2 * PI / dTheta) + 1);
    dTheta = 2 * PI / (thetaPoints - 1);
    int omegaPoints = static_cast<int>(((maximumFrequency - minimumFrequency) / dOmega) + 1);
    dOmega = (maximumFrequency - minimumFrequency) / (omegaPoints - 1);
    double omegaMax = std::max(std::abs(minimumFrequency), std::abs(maximumFrequency));
    //Stability condition for the finite difference scheme
    double dtMax = std::min(0.9 * (dTheta * dTheta) / (D + (K + omegaMax) * dTheta + K * dTheta * dTheta), T / 100);  
    int steps = static_cast<int>(T / dtMax) + 1;
    double frameInterval = 1.0 / (T * framePerSeconds);
    double dt = std::min(T /  static_cast<double>(steps), frameInterval);  
    
    return {T, D, K, dTheta, thetaPoints, minimumFrequency, maximumFrequency, dOmega, omegaPoints, dt, steps, frameInterval};
}