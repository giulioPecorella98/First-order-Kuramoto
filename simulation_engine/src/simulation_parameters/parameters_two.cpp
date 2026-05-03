/*
Function to load the parameters of the simulation by asking the user to input them in the terminal, computing also some derived parameters
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
    int Kpoints;
    std::cout << "2) Enter the number of coupling strength points: ";
    std::cin >> Kpoints;
    while ((Kpoints <= 1) || (std::cin.fail())) {
        std::cout << "Invalid choice. The number of coupling strength points must be an integer greater than 1: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> Kpoints;
    }
    double D; 
    std::cout << "3) Enter the noise level D: ";
    std::cin >> D;  
    while ((D < 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The noise level must be a nonnegative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> D;
    }  
    double dTheta;
    std::cout << "4) Enter the phase discretization: ";
    std::cin >> dTheta; 
    while ((dTheta <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The phase discretization must be a positive number: ";
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
    std::cout << "6) Enter the maximum natural frequency of the oscillators (for identical oscillators consider adding a small value to minimum frequency): ";
    std::cin >> maximumFrequency; 
    while (std::cin.fail() || (maximumFrequency <= minimumFrequency)) {
        std::cout << "Invalid choice. Remember that the minimum frequency is at most equal to the maximum frequency up to a small value: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> maximumFrequency;
    }
    double dFrequency;
    std::cout << "7) Enter the natural frequency discretization: ";
    std::cin >> dFrequency; 
    while ((dFrequency <= 0) || (dFrequency > (maximumFrequency - minimumFrequency)) || (std::cin.fail())) {
        std::cout << "Invalid choice. The natural frequency discretization must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> dFrequency  ;
    }
    double Tmax;
    std::cout << "8) Enter the maximum simulation time: ";
    std::cin >> Tmax;
    while ((Tmax <= 0) || (std::cin.fail())) {
        std::cout << "Invalid choice. The maximum simulation time must be a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> Tmax;
    }

    int thetaPoints = static_cast<int>((2 * PI / dTheta) + 1);
    dTheta = 2 * PI / (thetaPoints - 1);
    int frequencyPoints = static_cast<int>(((maximumFrequency - minimumFrequency) / dFrequency) + 1);
    dFrequency = (maximumFrequency - minimumFrequency) / (frequencyPoints - 1);
    double frequencyMax = std::max(std::abs(minimumFrequency), std::abs(maximumFrequency));
    
    return {Kmax, Kpoints, D, dTheta, thetaPoints, minimumFrequency, maximumFrequency, dFrequency, frequencyPoints, Tmax};
}