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
    "font.size": 24,
    "mathtext.fontset": 'stix',
    # "font.serif": ['Helvetica'],
}
mpl.rcParams.update(config)



def find_extrema(x, prominence):
    width = None
    peak1, _ = signal.find_peaks(x, width=width, distance=None, prominence=prominence)
    peak2, _ = signal.find_peaks(-x, width=width, distance=None,  prominence=prominence)
    return np.concatenate([peak1, peak2])


def round_to_ten(num1, num2):
    x1 = num1 / 10
    y1 = np.ceil(x1) + 1
    x2 = num2 / 10
    y2 = np.floor(x2) - 1
    return [int(y1 * 10), int(y2 * 10)]


def adjust_spines(ax, spines):
    for loc, spine in ax.spines.items():
        if loc in spines:
            spine.set_position(('outward', 10))  # outward by 10 points
            # spine.set_smart_bounds(True)
        else:
            spine.set_color('none')  # don't draw spine

    # turn off ticks where there is no spine
    if 'left' in spines:
        ax.yaxis.set_ticks_position('left')
    else:
        # no yaxis ticks
        ax.yaxis.set_ticks([])

    if 'bottom' in spines:
        ax.xaxis.set_ticks_position('bottom')
    else:
        # no xaxis ticks
        ax.xaxis.set_ticks([])
        
        
def LongPlotPressure(time, ylist, ylim, scr: str, arterialName, show_fit=True, figformat="tif"):
    # how many of arteries plot profile.
    N = len(ylist)
    # start time, and every artery plot profile should be plot by 0.1s distance.
    period = time[0]
    maxPressure = np.zeros(N)
    maxTime = np.zeros(N)
    realmaxTime = np.zeros(N)
    minPressure = np.zeros(N)
    minTime = np.zeros(N)
    realminTime = np.zeros(N)
    mean_pressure = np.zeros(N)
    for i in range(N):
        maxPressure[i] = np.max(ylist[i])
        minPressure[i] = np.min(ylist[i])
        ind_max = np.argmax(ylist[i])
        ind_min = np.argmin(ylist[i])
        maxTime[i] = time[ind_max] + 1.1 * i
        realmaxTime[i] = time[ind_max] - period
        minTime[i] = time[ind_min] + 1.1 * i
        realminTime[i] = time[ind_min] - period
        mean_pressure[i] = np.mean(ylist[i])
    [p_up, p_down] = round_to_ten(np.max(maxPressure), np.min(minPressure))

    x1 = [period + 0.5 + 1.1 * i
          for i in range(N)]  # time set for mean pressure

    fig = plt.figure(figsize=(20, 5))
    ax = fig.add_subplot(1, 1, 1)
    ax.grid(True)
    # ax.set(ylim=(p_down, p_up))
    ax.set_ylim(ylim)
    # ax.set_yticks(np.arange(p_down, p_up + 1, 20))
    adjust_spines(ax, ['left'])
    ax.set_ylabel("Blood pressure (mmHg)")

    for i in range(N):
        ax.plot(time + 1.1 * i, ylist[i], "k", linewidth=4, clip_on=False)
    ax.scatter(x1, mean_pressure, color='red')
    ax.scatter(minTime, minPressure, color='red')
    ax.scatter(maxTime, maxPressure, color='red')
    for i in range(N):
        plt.text(period + .2 + 1.1 * i,
                 p_down - 13,
                 arterialName[i],
                 fontsize=18,
                 ha='left',
                 rotation=20,
                 wrap=True)
        plt.text(x1[i],
                 mean_pressure[i] - 5,
                 str("{:.2f}".format(mean_pressure[i])),
                 fontsize=12,
                 ha='center')
        plt.text(maxTime[i],
                 maxPressure[i] + 5,
                 str("{:.2f}".format(maxPressure[i])),
                 fontsize=12,
                 ha='center')
        plt.text(minTime[i],
                 minPressure[i] - 5,
                 str("{:.2f}".format(minPressure[i])),
                 fontsize=12,
                 ha='center')

    if show_fit:
        x = np.linspace(period, period + 1.1 * N - 0.1, 100)
        coeff1 = polyfit(minTime, minPressure, 1)
        coeff2 = polyfit(maxTime, maxPressure, 1)
        coeff3 = polyfit(x1, mean_pressure, 1)
        ax.plot(x, coeff1[0] * x + coeff1[1], 'k--', linewidth=2)
        ax.plot(x, coeff2[0] * x + coeff2[1], 'k--', linewidth=2)
        ax.plot(x, coeff3[0] * x + coeff3[1], 'k--', linewidth=2)

    plt.savefig("./Pressure_" + scr + "."+figformat,
                dpi=600,
                # format=figformat,
                pad_inches=0.0,
                transparent=True)
    plt.close()


def LongPlotFlow(time, ylist, ylim, scr: str, arterialName, figformat="tif"):
    N = len(ylist)
    period = time[0]
    fig = plt.figure(figsize=(20, 5))
    ax = fig.add_subplot(1, 1, 1)
    [q_up, q_down] = round_to_ten(np.max(ylist), np.min(ylist))
    ax.set(ylim=(-50, 500))
    ax.set_yticks(np.arange(-50, 500 + 1, 50))
    ax.set_ylabel("Blood flow (mL/s)")
    ax.spines['bottom'].set_position(('data', 0))
    ax.axes.xaxis.set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    peakW = []
    prominence = 5
    for i in range(N):
        peakW.append(find_extrema(ylist[i], prominence))
    for i in range(N):
        ax.plot(time + 1.1 * i, ylist[i], "k", linewidth=4, clip_on=False)
        plt.text(period + .05 + 1.1 * i,
                 -60,
                 arterialName[i],
                 fontsize=18,
                 ha='left',
                 wrap=True)
        for j in peakW[i]:
            plt.text(time[j]+1.1*i,
                     ylist[i][j],
                     str("({:.4f},{:.2f})".format(time[j],ylist[i][j])),
                     fontsize=18,
                     ha='center')

    plt.savefig("./Flow_" + scr + "."+figformat,
                dpi=600,
                # format=figformat,
                pad_inches=0.0,
                transparent=True)
    plt.close()
    
    
# this section is used to obtain all object diractories in the current dir

obDirList = ["../result"]
dirAll = []
M = len(obDirList)

number = [0,14,30,34,38]
N = len(number)

arterialName = []
filename = "../Geometry/jcj55.xlsx"
df = pd.read_excel(filename, sheet_name="Sheet1")
for i in number:
    arterialName.append(df["Artery"][i])
    
    
for i in range(M):
    # 对case循环，单独处理case
    P = []
    Q = []
    # 对血管循环，有的血管可能需要组合
    for j in range(N):
        # 需要处理的血管名称
        A = np.loadtxt("./" + obDirList[i] + "/q" + str(number[j]+1))
        B = np.loadtxt("./" + obDirList[i] + "/p" + str(number[j]+1))
        [XQ, YQ, ZQ] = gnuplot(A)
        [XP, YP, ZP] = gnuplot(B)
        time = XQ[:, 0]
    
        # b is the monitor point of artery, here wo monitor middle point
        [a, b] = np.shape(ZQ)
        pos = b//2
        # here Q list is flow at specific point in each case.
        Q.append(ZQ[:, pos])
        P.append(ZP[:, pos])
#    scr = obDirList[i].replace("../", "")
    scr = "result1"
    LongPlotPressure(time, P, [50, 130], scr, arterialName, figformat="svg")
    LongPlotFlow(time, Q, [-50, 500], scr, arterialName, figformat="svg")
    
    
    
    
    
    
"""
如果需要输出到.csv可以参考如下代码：

        maxPressure.append(np.max(Z[:,position]))
        minPressure.append(np.min(Z[:,position]))
        maxTime.append(X[np.argmax(Z[:,position]), position]+1.1*i)
        realmaxTime.append(X[np.argmax(Z[:,position]), position]-6.0)
        minTime.append(X[np.argmin(Z[:,position]), position]+1.1*i)
        realminTime.append(X[np.argmin(Z[:,position]), position]-6.0)



       
    pulse_pressure = np.array(maxPressure)-np.array(minPressure)
    newdf = pd.DataFrame(data={"artery": arterialName, "P_max": maxPressure, "Times_pmax": realmaxTime, "P_min": minPressure, "Times_pmin": realminTime, "pulse pressure": pulse_pressure})
    newdf.to_csv(".\\pressure_csv\\Pressure_"+scr+".csv")
  
        flow.append(np.max(Z[:,position]))
        speed.append(np.mean(Zc[:, position]))
        times.append(X[np.argmax(Z[:,position]), position]-6.0)

    plt.savefig(".\\flow\\Flow_"+scr+".tif",dpi=144,format="tif")
    plt.close()
    newdf = pd.DataFrame(data={"artery": arterialName, "Q_max": flow, "Times_qmax": times, "c": speed})
    newdf.to_csv(".\\flow_csv\\Flow_"+scr+".csv")
# plt.show()

#def longplot():
"""
 
