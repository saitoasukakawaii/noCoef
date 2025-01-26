from plot_tools import gnuplot
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal
import pandas as pd
from numpy import polyfit
from plot_tools import *
import os
import re

config = {
    "font.family": 'Arial', # Times New Roman
    "font.size": 16,
    "mathtext.fontset": 'stix',
    # "font.serif": ['Helvetica'],
}
mpl.rcParams.update(config)

if __name__ == "__main__":
    # id = [1, 12, 14, 29, 35, 49]
    id = [3, 17, 13, 37]
    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot()
    # Ylim = [60, 130]
    for number in id:
        A = np.loadtxt("../result/p" + str(number))
        [XP, YP, ZP] = gnuplot(A)
        [a, b] = np.shape(XP)
        position = b // 2

        P = ZP[:, position]
        time = XP[:, 0]
        index = time > 7
    
        
        ax.plot(time[index], P[index], linewidth=2)
    ax.set_title("Pressure")
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Pressure (mmHg)")
    # ax.set_ylim(Ylim)
    ax.grid(which='both', linestyle='--')
    ax.legend(id)
    # plt.show()
    plt.savefig("./Pressure.png",
                 dpi=300,
                pad_inches=0.0)
