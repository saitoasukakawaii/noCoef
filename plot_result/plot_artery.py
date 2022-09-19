from plot_tools import gnuplot
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os
import re


def plot_pwave(scr, number, arterialName, Ylim=None):
    '''
    scr: which result, number: artery's order
    '''
    A = np.loadtxt("./" + scr + "/p" + str(number+1))
    [XP, YP, ZP] = gnuplot(A)
    [a, b] = np.shape(XP)
    # position = b // 2
    position = 0
    P = ZP[:, position]
    time = XP[:, 0]
    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot()
    ax.set_title("Pressure")
    ax.plot(time, P, linewidth=5)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Pressure (mmHg)")
    ax.set_ylim(Ylim)
    ax.grid(which='both', linestyle='--')
    plt.savefig("./" + arterialName + "/" + scr + "_pressure.tif",
                dpi=100,
                pad_inches=0.0)
    plt.close()


def plot_qwave(scr, number, arterialName, Ylim=None):
    '''
    scr: which result, number: artery's order
    '''
    B = np.loadtxt("./" + scr + "/q" + str(number+1))
    [XQ, YQ, ZQ] = gnuplot(B)
    [a, b] = np.shape(XQ)
    # position = b // 2
    position = 0
    Q = ZQ[:, position]
    time = XQ[:, 0]
    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot()
    ax.set_title("Flow")
    ax.plot(time, Q, linewidth=5)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Flow (ml/s)")
    ax.set_ylim(Ylim)
    ax.grid(which='both', linestyle='--')
    plt.savefig("./" + arterialName + "/" + scr + "_flow.tif",
                dpi=100,
                pad_inches=0.0)
    plt.close()

if __name__ == "__main__":
    # this section is used to obtain all object diractories in the current dir
    obDirList = "../result"
   
    j = 14
    PYlim = [55, 110]
    QYlim = [-140, 420]
    filename = "./Geometry/jcj55.xlsx"
    df = pd.read_excel(filename, sheet_name="Sheet1", engine="openpyxl")
    arterialName = df["Artery"][j]
  

    plot_pwave(obDirList, j, arterialName, PYlim)

  


