import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import os

def DataAnalysis():
    simulation =  input("Which simulation do you wish to load? ")
    continueAnalysis = True
    while continueAnalysis:
        try:
            with open(Path("saved_data") / simulation, "rb") as f:
                thetaPoints = np.fromfile(f, dtype = np.int32, count = 1)[0]
                omegaPoints = np.fromfile(f, dtype = np.int32, count = 1)[0]
                timePoints = np.fromfile(f, dtype = np.int32, count = 1)[0]
                minimumFrequency = np.fromfile(f, dtype = np.float64, count = 1)[0]
                maximumFrequency = np.fromfile(f, dtype = np.float64, count = 1)[0]
                finalTime = np.fromfile(f, dtype = np.float64, count = 1)[0]
                g = np.fromfile(f, dtype = np.float64, count=omegaPoints)
                density = np.fromfile(f, dtype = np.float64)
                continueAnalysis = False
        except Exception as e:
            print(f"An error occurred while reading the file: {e}")
            print(f"The available simulations are: {', '.join(os.listdir(Path('saved_data')))}")
            simulation = input("Try another file name: ")

    expected_size = thetaPoints * omegaPoints * timePoints
    if density.size != expected_size:
        raise ValueError(f"Solution vector has wrong dimension: expected {expected_size}, got {density.size}")

    f = density.reshape((timePoints, thetaPoints, omegaPoints))
    dt = finalTime / (timePoints - 1)
    dTheta = 2 * np.pi / thetaPoints
    dOmega = (maximumFrequency - minimumFrequency) / omegaPoints

    print("Plotting the evolution of the density...")
    plt.figure()
    time = 0
    for t in range(timePoints):
        plt.clf()
        plt.imshow(f[t, :, :].T, extent=[0, 2*np.pi, minimumFrequency, maximumFrequency], aspect='auto', origin='lower')
        plt.title(f"Density at time t = {time:.2f}")
        plt.xlabel(r"$\theta$")
        plt.ylabel(r"$\Omega$")
        if t == 0:
            plt.pause(2 - dt) 
        plt.pause(dt) 
        time += dt 

    print("Plotting the order parameter evolution")
    r = np.zeros(timePoints)
    for t in range(timePoints):
        integral = np.zeros((thetaPoints, omegaPoints), dtype=complex);
        for i in range(thetaPoints):
            for j in range(omegaPoints):
                integral[i][j] = f[t][i][j] * np.exp(1j * i * dTheta) * g[j] 
        r[t] = np.abs(np.sum(integral) * dTheta * dOmega)
    plt.figure()
    plt.plot(np.linspace(0, finalTime, timePoints), r)
    plt.title(f"Order parameter over time")
    plt.xlim(0, finalTime)
    plt.ylim(0, 1.1)
    plt.xlabel(r"$t$")
    plt.ylabel(r"$r(t)$")
    plt.show(block=False)

    input("Press Enter to close the plot...")
    plt.close('all')