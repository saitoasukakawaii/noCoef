import numpy as np
import matplotlib.pyplot as plt


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