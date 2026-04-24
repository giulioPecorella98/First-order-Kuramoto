import numpy as np
import struct
import matplotlib.pyplot as plt
from pathlib import Path
import os

def densityEvolution():
    simulation =  input("Which simulation do you wish to load? (type 's' to see available simulations) ")
    while simulation == 's':
        print(f"The available simulations are: {', '.join(os.listdir(Path('saved_simulations')))}")
        simulation = input("Which simulation do you wish to load? ")
    
    continueAnalysis = True
    density = []
    r = []
    while continueAnalysis:
        try:
            with open(Path("saved_simulations") / simulation, "rb") as f:
                thetaPoints = int(struct.unpack('i', f.read(4))[0])
                omegaPoints = int(struct.unpack('i', f.read(4))[0])
                minimumFrequency = struct.unpack('d', f.read(8))[0]
                maximumFrequency = struct.unpack('d', f.read(8))[0]
                finalTime = struct.unpack('d', f.read(8))[0]
                D = struct.unpack('d', f.read(8))[0]
                K = struct.unpack('d', f.read(8))[0]
                g = np.fromfile(f, dtype = np.float64, count = omegaPoints)
                size = thetaPoints * omegaPoints
                while True:
                    data = f.read(size * 8)  
                    if len(data) != size * 8:
                        break
                    density.append(np.frombuffer(data, dtype = np.float64))
                    r.append(struct.unpack('d', f.read(8))[0])
                density = np.array(density)
                r = np.array(r)
                timePoints = np.size(r)
                continueAnalysis = False
        except Exception as e:
            print(f"An error occurred while reading the file: {e}")
            print(f"The available simulations are: {', '.join(os.listdir(Path('saved_simulations')))}")
            simulation = input("Try another file name: ")

    rho = density.reshape((timePoints, thetaPoints, omegaPoints))
    dt = finalTime / (timePoints - 1)

    print("Plotting the evolution of the density...")
    vmax = np.max(rho)
    fig, ax = plt.subplots()
    im = ax.imshow(rho[0, :, :].T,
               extent = [0, 2*np.pi, minimumFrequency, maximumFrequency],
               aspect = 'auto', origin = 'lower')
    fig.colorbar(im, ax = ax)
    title = ax.set_title(r"Density $\rho(\theta, \Omega,$" + f"{0:.2f})")
    ax.set_xlabel(r"$\theta$")
    ax.set_ylabel(r"$\Omega$")
    plt.pause(2)
    for t in range(1, timePoints):
        im.set_data(rho[t, :, :].T)
        title.set_text(r"Density $\rho(\theta, \Omega,$" + f"{t*dt:.2f})")
        plt.pause(dt)
    plt.pause(2)
    plt.close(fig) 

    theta = np.linspace(0, 2 * np.pi, thetaPoints)
    omega = np.linspace(minimumFrequency, maximumFrequency, omegaPoints)
    Theta, Omega = np.meshgrid(theta, omega)
    fig0 = plt.figure()
    ax0 = fig0.add_subplot(111, projection = '3d')
    ax0.set_box_aspect([1, 1, 0.7])
    ax0.plot_surface(Theta, Omega, rho[0, :, :].T, cmap = 'viridis', vmin = 0, vmax = vmax)
    ax0.set_title("Initial density at t = 0")
    ax0.set_xlabel(r"$\theta$")
    ax0.set_ylabel(r"$\Omega$")
    ax0.set_zlabel(r"$\rho$")
    fig1 = plt.figure()
    ax1 = fig1.add_subplot(111, projection = '3d')
    ax1.set_box_aspect([1, 1, 0.7])
    ax1.plot_surface(Theta, Omega, rho[-1, :, :].T, cmap = 'viridis', vmin = 0, vmax = vmax)
    ax1.set_title(f"Final density at t = {finalTime:.2f}")
    ax1.set_xlabel(r"$\theta$")
    ax1.set_ylabel(r"$\Omega$")
    ax1.set_zlabel(r"$\rho$")
    mappable = plt.cm.ScalarMappable(cmap = 'viridis')
    mappable.set_array(rho)
    mappable.set_clim(0, vmax)
    fig0.colorbar(mappable, ax = ax0, shrink = 0.6, pad = 0.1)
    fig1.colorbar(mappable, ax = ax1, shrink = 0.6, pad = 0.1)
    ax0.set_zlim(0, vmax)
    ax1.set_zlim(0, vmax)

    def on_scroll(event):
        if event.button == 'up':
            ax1.view_init(elev = 30, azim = ax1.azim + 5)
        elif event.button == 'down':
            ax1.view_init(elev = 30, azim = ax1.azim - 5)
        fig1.canvas.draw_idle()

    fig1.canvas.mpl_connect('scroll_event', on_scroll)
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

    print("Plotting the frequency distribution")
    plt.figure()
    plt.plot(omega, g)
    plt.title("Frequency distribution")
    plt.xlabel(r"$\Omega$")
    plt.ylabel(r"$g(\Omega)$")
    plt.xlim(minimumFrequency, maximumFrequency)
    plt.ylim(0, np.max(g) * 1.1)
    plt.show(block = False)

    input("Press Enter to close the plot...")
    plt.close('all')