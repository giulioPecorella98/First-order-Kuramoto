import plot.density_evo as de
import plot.order_parameter_evo as ope

def DataAnalysis():
    print("What do you want to do?")
    print("1) Load a saved simulation")
    print("2) Plot the order parameter R for different values of K")
    choice = input("Please type your choice: ")
    while choice != "1" and choice != "2":
        print("Command not valid, please type again.")
        print("What do you want to do?")
        print("1) Load a saved simulation")
        print("2) Plot the order parameter R for different values of K")
        choice = input("Please type your choice: ")
    
    if choice == "1":
        de.densityEvolution()
    elif choice == "2":
        ope.orderParameterEvolution()

    