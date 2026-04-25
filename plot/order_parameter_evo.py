import numpy as np
import struct
import matplotlib.pyplot as plt
from pathlib import Path
import os

def orderParameterEvolution():

    simulation =  input("Which simulation do you wish to load? (type 's' to see available simulations) ")
    while simulation == 's':
    
        print(f"The available simulations are: {', '.join(os.listdir(Path('saved_order_parameters')))}")
        simulation = input("Which simulation do you wish to load? ")
    
    continueAnalysis = True
    while continueAnalysis:

        try:
            with open(Path("saved_order_parameters") / simulation, "rb") as file:
                
                KPoints = struct.unpack('i', file.read(4))[0]
                K = np.fromfile(file, dtype = np.float64, count = KPoints)
                r = np.fromfile(file, dtype = np.float64, count = KPoints)
                continueAnalysis = False

        except Exception as e:
            
            print(f"An error occurred while reading the file: {e}")
            print(f"The available simulations are: {', '.join(os.listdir(Path('saved_order_parameters')))}")
            simulation = input("Try another file name: ")

    print("Plotting the order parameter evolution")
    plt.figure()
    plt.plot(K, r)
    plt.title(f"Order parameter evolution for {simulation}")
    plt.xlim(0, K[-1])
    plt.ylim(0, 1.1)
    plt.xlabel(r"$K$")
    plt.ylabel(r"$r(K)$")
    plt.show(block = False)

    input("Press Enter to close the plot...")
    plt.close('all')