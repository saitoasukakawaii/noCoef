import numpy as np
import matplotlib.pyplot as plt
import os,re


def gnuplot(A: np.array):
    """

    @rtype: object
    """
    [height, width] = np.shape(A)
    if width != 3:
        raise "Input matrix must have three columns\n"
    
    # three data refer to time, x-coordinate and result respectively.
    x = A[:,0]
    y = A[:,1]
    z = A[:,2]

    # Determine length of y-axis
    xx = x[0]
    ylength: int = 2
    while xx == x[ylength-1]:
        ylength += 1
    ylength -= 1

    xlength: int = int(height/ylength);

    X = np.zeros((xlength, ylength))
    Y = np.zeros((xlength, ylength))
    Z = np.zeros((xlength, ylength))

    for i in range(xlength):
        a = i*ylength
        b = (i+1)*ylength
        X[i, :] = x[a:b]
        Y[i, :] = y[a:b]
        Z[i, :] = z[a:b]
    return [X, Y, Z]


def PlotSurf3d(X,Y,Z,Title=None):
    from mpl_toolkits import mplot3d
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.plot_surface(X, Y, Z,cmap='viridis', edgecolor='none')
    ax.set_title(Title)
    plt.show()


def get_deal_file(re_name):
    root = os.getcwd()
    matchStr = re.compile(re_name)
    dirAll = []
    for dirpath, dirnames, filenames in os.walk(root):
        for _ in dirnames:
            dirAll.append(_)
    obDirList = []
    for _ in dirAll:
        if matchStr.match(_) is None:
            pass
        else:
            obDirList.append(_)

    return obDirList


def getBarSerial(w, N):
    return w*np.arange(-N+1, N, 2)

def combineData(scr, number):
    '''
    to combine a serial data together.
    @param scr: the dir of data.
    @param number: a list of serial number of artery, start from 1 so do not need +1
    @return: combine time, x_axis, P and Q, where time and x_axis is list, and PQ are matrix in time and x dimension
    '''
    # read data
    A = np.loadtxt("./" + scr + "/p" + str(number[0]))
    B = np.loadtxt("./" + scr + "/q" + str(number[0]))
    # reshape data to x: time, y: length, z: data.
    [XP, YP, ZP] = gnuplot(A)
    [XQ, YQ, ZQ] = gnuplot(B)
    time = XP[:, 0]
    x_axis = YP[0, :]
    # \Delta t, time interval
    dt = time[1]-time[0]
    # \Delta x: h, x axis interval
    h = x_axis[1]-x_axis[0]
    EndPL = YP[-1, -1]
    for i in range(1, len(number)):
        A = np.loadtxt("./" + scr + "/p" + str(number[i]))
        B = np.loadtxt("./" + scr + "/q" + str(number[i]))
        # reshape data to x: time, y: length, z: data.
        [XP1, YP1, ZP1] = gnuplot(A)
        [XQ1, YQ1, ZQ1] = gnuplot(B)
        YP1 = YP1 + EndPL + h
        EndPL = YP1[-1,-1]
        YP = np.hstack((YP, YP1))
        ZP = np.hstack((ZP, ZP1))
        ZQ = np.hstack((ZQ, ZQ1))
    return [time, YP[0, :], ZP, ZQ]