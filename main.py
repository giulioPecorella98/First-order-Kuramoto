import subprocess
import os
from plot import plotting



def main():
    print("\033[91m==========================================================================================")
    print("|             Welcome to the first order mean field Kuramoto model simulator!            |")
    print("==========================================================================================")
    print("\033[92m    Here you have the opportunity to simulate the dynamics of the first order Kuramoto")
    print("    model, choosing the initial distribution of the oscillators, the natural frequency")
    print("    distribution, the noise level D, and the coupling constant K.")
    print("\033[91m------------------------------------------------------------------------------------------")
    print("\n\033[0mWhat do you want to do?")
    print("1) Run a new simulation")
    print("2) Test order parameter dependence on K")
    print("3) Load a saved simulation")
    print("4) Exit")

    userInput = input("Please type your choice: ")
    while(userInput != "4"):
    
        if userInput == "1":    
            print()
            project_root = os.path.dirname(os.path.abspath(__file__))
            exe_path = os.path.join(project_root, "simulation_engine", "engine_one")
            subprocess.run([exe_path])
    
        elif userInput == "2":
            print()
            project_root = os.path.dirname(os.path.abspath(__file__))
            exe_path = os.path.join(project_root, "simulation_engine", "engine_two")
            subprocess.run([exe_path])
    
        elif userInput == "3":
            print()
            plotting.DataAnalysis()
    
        else:
            print("Command not valid, please try again.")
            
        print("\nWhat do you want to do now?")
        print("1) Run a new simulation")
        print("2) Test order parameter dependence on K")
        print("3) Load a saved simulation")
        print("4) Exit")
        userInput = input("Please type your choice: ")

    print("Thank you for using the first order mean field Kuramoto model simulator! See you next time!")

if __name__ == "__main__":
    main()