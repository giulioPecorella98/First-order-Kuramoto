import numpy as np
import struct
import matplotlib.pyplot as plt
from pathlib import Path
import os



rightButtonPressed = False
def onButtonPress(event):
    global rightButtonPressed
    if event.button == 3:
        rightButtonPressed = True

def onButtonRelease(event):
    global rightButtonPressed   
    if event.button == 3:
        rightButtonPressed = False

def onScroll(event):
    ax = event.inaxes
    if ax is None:
        return
    delta = 5 if event.button == 'up' else -5 if event.button == 'down' else 0
    if delta == 0:
        return
    if rightButtonPressed:
        ax.view_init(elev = ax.elev + delta, azim = ax.azim)
    else:
        ax.view_init(elev = 30, azim = ax.azim + delta)
    event.canvas.draw_idle()



def densityEvolution():

    simulation = input("Which simulation do you wish to load? (type 's' to see available simulations, 'q' to quit to main menu) ")
    path = Path("save/density")
    path.mkdir(parents = True, exist_ok = True)
    while simulation == 's':
        print(f"The available simulations are: {', '.join(os.listdir(path))}")
        simulation = input("Which simulation do you wish to load? ")    
    if simulation == 'q':
        return
    
    continueAnalysis = True
    rho = []
    r = []
    # Load data from the binary file   
    while continueAnalysis:
        try:
            with open(path / simulation, "rb") as f:
                thetaPoints = int(struct.unpack('i', f.read(4))[0])
                frequencyPoints = int(struct.unpack('i', f.read(4))[0])
                minimumFrequency = struct.unpack('d', f.read(8))[0]
                maximumFrequency = struct.unpack('d', f.read(8))[0]
                finalTime = struct.unpack('d', f.read(8))[0]
                D = struct.unpack('d', f.read(8))[0]
                K = struct.unpack('d', f.read(8))[0]
                g = np.fromfile(f, dtype = np.float64, count = frequencyPoints)
                size = frequencyPoints * thetaPoints * 8
                while True:
                    data = f.read(size)  
                    if len(data) != size:
                        break
                    rho.append(np.frombuffer(data, dtype = np.float64))
                    r.append(struct.unpack('d', f.read(8))[0])
                rho = np.array(rho)
                r = np.array(r)
                timePoints = np.size(r)
                continueAnalysis = False
        except Exception as e:
            print(f"An error occurred while reading the file: {e}. Returning to the main menu...")
            return  
    rho = rho.reshape((timePoints, frequencyPoints, thetaPoints))
    dt = finalTime / (timePoints - 1)
    vmax = np.max(rho)
    vmin = min(np.min(rho), 0)  
    theta = np.linspace(0, 2 * np.pi, thetaPoints)
    frequency = np.linspace(minimumFrequency, maximumFrequency, frequencyPoints)
    print("Plotting the evolution of the density...")

    # Plot the evolution of the density for identical oscillators
    if (frequencyPoints == 1):
        fig, ax = plt.subplots()
        line, = ax.plot(theta, rho[0, 0, :])
        ax.set_xlabel(r"$\theta$")
        ax.set_ylabel(r"$\rho$")
        ax.set_xlim(0, 2 * np.pi)
        ax.set_ylim(vmin * 1.1, vmax * 1.1)
        title = ax.set_title(r"Density $\rho(\theta, t=$" + f"{0:.2f})")
        plt.pause(2)
        for t in range(1, timePoints):
            line.set_ydata(rho[t, 0, :])
            title.set_text(r"Density $\rho(\theta, t=$" + f"{t*dt:.2f})")
            plt.pause(dt)
        plt.pause(2)
        plt.close(fig)

        # Plot initial density
        initialFigure = plt.figure()
        ax_init = initialFigure.add_subplot(111)
        ax_init.plot(theta, rho[0, 0, :], linewidth = 2)
        ax_init.set_xlabel(r"$\theta$")
        ax_init.set_ylabel(r"$\rho$")
        ax_init.set_ylim(vmin * 1.1, vmax * 1.1)
        ax_init.set_title("Initial density at t = 0")
        ax_init.set_xlim(0, 2 * np.pi)
        plt.show(block=False)

        # Plot final density
        finalFigure = plt.figure()
        ax_final = finalFigure.add_subplot(111)
        ax_final.plot(theta, rho[-1, 0, :], linewidth = 2)
        ax_final.set_xlabel(r"$\theta$")
        ax_final.set_ylabel(r"$\rho$")
        ax_final.set_ylim(vmin * 1.1, vmax * 1.1)
        ax_final.set_title(f"Final density at t = {finalTime:.2f}")
        ax_final.set_xlim(0, 2 * np.pi)
        plt.show(block=False)
    
    # Plotting the evolution of the density for non identical oscillators
    else: 
        fig, ax = plt.subplots()
        im = ax.imshow(rho[0, :, :].T,
                extent = [minimumFrequency, maximumFrequency, 0, 2 * np.pi],
                aspect = 'auto', origin = 'lower')
        fig.colorbar(im, ax = ax)
        title = ax.set_title(r"Density $\rho(\theta, \Omega,$" + f"{0:.2f})")
        ax.set_ylabel(r"$\theta$")
        ax.set_xlabel(r"$\Omega$")
        plt.pause(1)
        for t in range(1, timePoints):
            im.set_data(rho[t, :, :].T)
            title.set_text(r"Density $\rho(\theta, \Omega,$" + f"{t*dt:.2f})")
            plt.pause(dt)
        plt.pause(1)
        plt.close(fig) 

        # 3D plots of the initial and final density
        Frequency, Theta = np.meshgrid(frequency, theta)
        fig0 = plt.figure()
        ax0 = fig0.add_subplot(111, projection = '3d')
        ax0.set_box_aspect([1, 1, 0.7])
        ax0.plot_surface(Frequency, Theta, rho[0, :, :].T, cmap = 'viridis', vmin = vmin, vmax = vmax)
        ax0.set_title("Initial density at t = 0")
        ax0.set_ylabel(r"$\theta$")
        ax0.set_xlabel(r"$\Omega$")
        ax0.set_zlabel(r"$\rho$")
        fig1 = plt.figure()
        ax1 = fig1.add_subplot(111, projection = '3d')
        ax1.set_box_aspect([1, 1, 0.7])
        ax1.plot_surface(Frequency, Theta, rho[-1, :, :].T, cmap = 'viridis', vmin = vmin, vmax = vmax)
        ax1.set_title(f"Final density at t = {finalTime:.2f}")
        ax1.set_ylabel(r"$\theta$")
        ax1.set_xlabel(r"$\Omega$")
        ax1.set_zlabel(r"$\rho$")
        mappable = plt.cm.ScalarMappable(cmap = 'viridis')
        mappable.set_array(rho)
        mappable.set_clim(vmin, vmax)
        fig0.colorbar(mappable, ax = ax0, shrink = 0.6, pad = 0.1)
        fig1.colorbar(mappable, ax = ax1, shrink = 0.6, pad = 0.1)
        ax0.set_zlim(vmin, vmax)
        ax1.set_zlim(vmin, vmax)
        fig0.canvas.mpl_connect('button_press_event', onButtonPress)
        fig0.canvas.mpl_connect('button_release_event', onButtonRelease)
        fig0.canvas.mpl_connect('scroll_event', onScroll)
        fig1.canvas.mpl_connect('button_press_event', onButtonPress)
        fig1.canvas.mpl_connect('button_release_event', onButtonRelease)
        fig1.canvas.mpl_connect ('scroll_event', onScroll)
        plt.show(block = False)

        plt.figure()
        plt.plot(frequency, g)
        plt.title(r"Frequency distribution $g(\Omega)$")
        plt.xlabel(r"$\Omega$")
        plt.ylabel(r"$g(\Omega)$")
        xOffsets = 0.1 * max(abs(maximumFrequency), abs(minimumFrequency))
        plt.xlim(minimumFrequency - xOffsets, maximumFrequency + xOffsets)
        plt.ylim(0, np.max(g) * 1.1)
        plt.show(block = False)

    print("Plotting the order parameter evolution")
    plt.figure()
    plt.plot(np.linspace(0, finalTime, timePoints), r)
    plt.title(f"Noise D = {D:.2f}, coupling K = {K:.2f}")
    plt.xlim(0, finalTime)
    plt.ylim(0, 1.1)
    plt.xlabel(r"$t$")
    plt.ylabel(r"$r(t)$")
    plt.show(block = False)

    input("Press Enter to close the plot...")
    plt.close('all')