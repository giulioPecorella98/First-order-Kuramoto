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
                thetaPoints = np.fromfile(f, dtype=np.int32, count=1)[0]
                timePoints = np.fromfile(f, dtype=np.int64, count=1)[0]
                finalTime = np.fromfile(f, dtype=np.float64, count=1)[0]
                density = np.fromfile(f, dtype=np.float64)
                continueAnalysis = False
        except Exception as e:
            print(f"An error occurred while reading the file: {e}")
            print(f"The available simulations are: {', '.join(os.listdir(Path('saved_data')))}")
            simulation = input("Try another file name: ")

    expected_size = thetaPoints * timePoints
    if density.size != expected_size:
        raise ValueError(f"Solution vector has wrong dimension: expected {expected_size}, got {density.size}")

    f = density.reshape((timePoints, thetaPoints))
    theta = np.linspace(0, 2 * np.pi, thetaPoints, endpoint=True)
    dt = finalTime / (timePoints - 1)

    print("Plotting the evolution of the density...")
    plt.figure()
    time = 0
    ymax = np.max(f) * 1.5
    for t in range(timePoints):
        plt.clf()
        plt.plot(theta, f[t, :])
        plt.title(f"Density at time t = {time:.2f}")
        plt.xlim(0, 2 * np.pi)
        plt.xlabel(r"$\theta$")
        plt.ylim(0, ymax)
        plt.ylabel(r"$\rho (\theta,t)$")
        if t == 0:
            plt.pause(2 - dt) 
        plt.pause(dt) 
        time += dt 

    print("Plotting the order parameter...")
    r = np.abs(np.sum(f * np.exp(1j * theta), axis=1)) *  2 * np.pi / (thetaPoints - 1)
    plt.figure()
    plt.plot(np.linspace(0, finalTime, timePoints), r)
    plt.title(f"Order parameter over time")
    plt.xlim(0, finalTime)
    plt.ylim(0, 1.1)
    plt.show(block=False)

    input("Press Enter to close the plot...")
    plt.close('all')