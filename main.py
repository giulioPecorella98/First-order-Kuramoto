import subprocess
import os
from plot import plotting

def main():
    print("=========================================================================================")
    print("|            Welcome to the first order mean field Kuramoto model simulator!            |")
    print("=========================================================================================")
    print(" Here you have the opportunity to simulate the dynamics of the first order Kuramoto")
    print(" model for identical oscillators, choosing the initial distribution of the oscillators,")
    print(" the noise level D, the coupling constant K and the natural frequency omega.")
    print("-------------------------------------------------------------------")
    print("\nWhat do you want to do?")
    print("1) Run a new simulation")
    print("2) Load a saved simulation")
    print("3) Exit")

    userInput = input("Please type your choice: ")
    while(userInput != "3"):

        if userInput == "1":    
            project_root = os.path.dirname(os.path.abspath(__file__))
            exe_path = os.path.join(project_root, "simulation_engine", "kuramoto")
            subprocess.run([exe_path])
        
        elif userInput == "2":
            plotting.DataAnalysis()
        
        else:
            print("Command not valid, please type again.")
        print("\nWhat do you want to do now?")
        print("1) Run a new simulation")
        print("2) Load a saved simulation")
        print("3) Exit")
        userInput = input("Please type your choice: ")

    print("Thank you for using the first order mean field Kuramoto model simulator! See you next time!")

if __name__ == "__main__":
    main()
