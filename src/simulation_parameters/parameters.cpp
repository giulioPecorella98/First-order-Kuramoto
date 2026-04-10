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
    std:: cout << "1) Enter the final time T: ";
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
        std::cout << "Invalid choice. The noise level must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> D;
    }   
    double K; 
    std::cout << "3) Enter the coupling constant K: ";
    std::cin >> K;
    while ((K < 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The coupling constant must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> K;
    }     
    double omega;
    std::cout << "4) Enter the natural frequency of the oscillators: ";
    std::cin >> omega; 
    while (std::cin.fail()) {
        std::cout << "Invalid choice. The space discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> omega;
    }             
    double dTheta;
    std::cout << "5) Enter the space discretization (care: if not small enough numerical diffusion may arise!): ";
    std::cin >> dTheta; 
    while ((dTheta <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The space discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dTheta;
    }
    double framePerSeconds;
    std::cout << "6) Enter the number of frames per seconds: ";
    std::cin >> framePerSeconds;  
    while ((framePerSeconds <= 0) || (std::cin.fail()) || (framePerSeconds < (1 / T))) {
        std::cout << "Invalid choice. Try again: " << ": ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> framePerSeconds;
    }
    int thetaPoints = static_cast<int>((2 * PI / dTheta) + 1) + 1;
    dTheta = 2 * PI / (thetaPoints - 1);
    double dt = std::min(0.9 * (dTheta * dTheta) / (D + (K + omega) * dTheta + K * dTheta * dTheta), T / 100);  //Stability condition for the finite difference scheme
    int steps = static_cast<int>(T / dt);
    double frameInterval = 1.0 / framePerSeconds;
    int frameCount = std::max(1, static_cast<int>(steps / 100.0));
    return {T, D, K, dTheta, dt, thetaPoints, steps, frameInterval, frameCount, omega};
}
