import numpy as np 
import matplotlib.pyplot as plt


if __name__ == "__main__":
    A=np.loadtxt("input.dat")
    plt.plot(A)
    plt.show()

