#!/usr/bin/env 
# -*- coding: utf-8 -*-

from plot_tools import gnuplot
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os
import re
import sys

mpl.rcParams["font.family"] = "Times New Roman"
mpl.rcParams["font.size"] = 18

def plot_pwave(number):
    '''
    scr: which result, number: artery's order
    '''
    A = np.loadtxt("../result/p" + str(number))
    [XP, YP, ZP] = gnuplot(A)
    [a, b] = np.shape(XP)
    ind = [0*b//4, 1*b//4, 2*b//4, 3*b//4, b-1]
    Color = ["cyan", "blue", "green", "yellow", "red"]
    N = len(ind)
    time = XP[:, 0]
    fig = plt.figure(figsize=(10,8))
    ax = fig.add_subplot()
    ax.set_title("Pressure")
    for i in range(N):
        ax.plot(time, ZP[:, ind[i]], color=Color[i], linewidth=5)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Pressure (mmHg)")
    ax.grid(which='both', linestyle='--')
    plt.savefig("./Pressure"+str(number)+".tif",
                dpi=120,
                pad_inches=0.0)
    plt.close()


def plot_qwave(number):
    '''
    scr: which result, number: artery's order
    '''
    B = np.loadtxt("../result/q" + str(number))
    [XQ, YQ, ZQ] = gnuplot(B)
    [a, b] = np.shape(XQ)
    ind = [0*b//4, 1*b//4, 2*b//4, 3*b//4, b-1]
    Color = ["cyan", "blue", "green", "yellow", "red"]
    N = len(ind)
    time = XQ[:, 0]
    fig = plt.figure(figsize=(10,8))
    ax = fig.add_subplot()
    ax.set_title("Flow")
    for i in range(N):
        ax.plot(time, ZQ[:, ind[i]], color=Color[i], linewidth=5)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Flow (ml/s)")
    ax.grid(which='both', linestyle='--')
    plt.savefig("./Flow"+str(number)+".tif",
                dpi=120,
                pad_inches=0.0)
    plt.close()

if __name__ == "__main__":
    # this section is used to obtain all object diractories in the current dir
 #   print(f"Arguments count: {len(sys.argv)}")
 #   Arteries = []
 #   for i, arg in enumerate(sys.argv):
#        print(f"Argument {i:>6}: {arg}")
#        if i>0:
#            Arteries.append(int(arg))
#    print(Arteries)
    Arteries = [i for i in range(1,72)]
    for i in Arteries:
        plot_pwave(i)
        plot_qwave(i)
    # root = os.getcwd()
    # matchStr = re.compile(r"jcj_58_jcj_inlet.*")
    # dirAll = []
    # for dirpath, dirnames, filenames in os.walk(root):
    #     for _ in dirnames:
    #         dirAll.append(_)
    # obDirList = []
    # for _ in dirAll:
    #     if matchStr.match(_) is None:
    #         pass
    #     else:
    #         obDirList.append(_)
    # M = len(obDirList)
    # # j1 = 25
    # # j2 = 23
    # # PYlim = [55, 110]
    # # QYlim = [0, 20]
    # # j1 = 39
    # # j2 = 36
    # # PYlim = [55, 110]
    # # QYlim = [-30, 90]
    # j1 = 14
    # j2 = 14
    # PYlim = [55, 110]
    # QYlim = [-140, 420]
    # filename1 = "./Geometry/jcj58_1.xlsx"
    # df1 = pd.read_excel(filename1, sheet_name="Sheet1", engine="openpyxl")
    # arterialName1 = df1["Artery"][j1]
    # filename2 = "./Geometry/jcj55.xlsx"
    # df2 = pd.read_excel(filename2, sheet_name="Sheet1", engine="openpyxl")
    # arterialName2 = df2["Artery"][j2]
    # if arterialName1 != arterialName2:
    #     print("different artery's name!")
    # if not os.path.exists(arterialName1):
    #     os.makedirs(arterialName1)

    
    
    # plot_pwave("jcj_55_jcj_inlet", j2, arterialName1, PYlim)
    # plot_qwave("jcj_55_jcj_inlet", j2, arterialName1, QYlim)

