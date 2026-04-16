import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import os
from matplotlib.animation import FuncAnimation

def DataAnalysis():
    simulation =  input("Which simulation do you wish to load? (type 's' to see available simulations) ")
    while simulation == 's':
        print(f"The available simulations are: {', '.join(os.listdir(Path('saved_data')))}")
        simulation = input("Which simulation do you wish to load? ")
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
                D = np.fromfile(f, dtype = np.float64, count = 1)[0]
                K = np.fromfile(f, dtype = np.float64, count = 1)[0]
                r = np.fromfile(f, dtype = np.float64, count=timePoints)
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
    vmax = np.max(f)
    fig, ax = plt.subplots()
    im = ax.imshow(f[0, :, :].T,
               extent=[0, 2*np.pi, minimumFrequency, maximumFrequency],
               aspect='auto',
               origin='lower')
    
    fig.colorbar(im, ax=ax)
    title = ax.set_title(r"Density $\rho(\theta, \Omega,$" + f"{0:.2f})")
    ax.set_xlabel(r"$\theta$")
    ax.set_ylabel(r"$\Omega$")
    for t in range(timePoints - 1):
        im.set_data(f[t, :, :].T)
        title.set_text(r"Density $\rho(\theta, \Omega,$" + f"{t*dt:.2f})")
        plt.pause(dt)
    plt.close(fig) 

    theta = np.linspace(0, 2*np.pi, f.shape[1])
    omega = np.linspace(minimumFrequency, maximumFrequency, f.shape[2])
    Theta, Omega = np.meshgrid(theta, omega)
    fig0 = plt.figure()
    ax0 = fig0.add_subplot(111, projection='3d')
    ax0.set_box_aspect([1, 1, 0.7])
    surf0 = ax0.plot_surface(
        Theta, Omega, f[0, :, :].T,
        cmap='viridis', vmin=0, vmax=vmax
    )
    ax0.set_title("Initial density (t = 0)")
    ax0.set_xlabel(r"$\theta$")
    ax0.set_ylabel(r"$\Omega$")
    ax0.set_zlabel(r"$\rho$")
    fig1 = plt.figure()
    ax1 = fig1.add_subplot(111, projection='3d')
    ax1.set_box_aspect([1, 1, 0.7])
    surf1 = ax1.plot_surface(
        Theta, Omega, f[-1, :, :].T,
        cmap='viridis', vmin=0, vmax=vmax
    )
    ax1.set_title(f"Final density at t = {finalTime:.2f}")
    ax1.set_xlabel(r"$\theta$")
    ax1.set_ylabel(r"$\Omega$")
    ax1.set_zlabel(r"$\rho$")
    mappable = plt.cm.ScalarMappable(cmap='viridis')
    mappable.set_array(f)
    mappable.set_clim(0, vmax)
    fig0.colorbar(mappable, ax=ax0, shrink=0.6, pad=0.1)
    fig1.colorbar(mappable, ax=ax1, shrink=0.6, pad=0.1)
    ax0.set_zlim(0, vmax)
    ax1.set_zlim(0, vmax)

    def on_scroll(event):
        step = 5
        if event.button == 'up':
            ax1.view_init(elev=30, azim=ax1.azim + step)
        elif event.button == 'down':
            ax1.view_init(elev=30, azim=ax1.azim - step)

        fig1.canvas.draw_idle()

    fig1.canvas.mpl_connect('scroll_event', on_scroll)
    plt.show(block=False)

    print("Plotting the order parameter evolution")
    plt.figure()
    plt.plot(np.linspace(0, finalTime, timePoints), r)
    plt.title(f"Noise D = {D:.2f}, coupling K = {K:.2f}")
    plt.xlim(0, finalTime)
    plt.ylim(0, 1.1)
    plt.xlabel(r"$t$")
    plt.ylabel(r"$r(t)$")
    plt.show(block=False)

    input("Press Enter to close the plot...")
    plt.close('all')