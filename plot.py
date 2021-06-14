import sys
import matplotlib.pyplot as plt
import math
import numpy as np

def plot(filepath, regression_type, a, b, x, y):
    fig = plt.figure(figsize=(10,5))
    
    # Scatter the true data
    plt.scatter(x, y, c = "red", label = "True Data")
    
    print(a)
    print(b)
    
    # Plot the regression line.
    f = None
    if regression_type == "Exponential":
        f = lambda v : a * b ** v
    elif regression_type == "Power":
        f = lambda v : a * v ** b
    elif regression_type == "Linear":
        f = lambda v : a + b * v
    else:
        assert("invalid regression type: " + regression_type)
    
    regression_x = np.linspace(x[0], round(x[-1]), 100).tolist()
    regression_y = list(map(f, regression_x))
    
    plt.plot(regression_x, regression_y, label = regression_type + " Regression Line")

    plt.ylabel("Distinct Minima")
    plt.xlabel("Atom Cluster Size")
    plt.title("Distinct Minima Vs. Atom Cluster Size ("+regression_type+" Regression).")
    plt.legend()

    # Saving the plot as an image
    print("Saving plot please wait...")
    fig.savefig(filepath, bbox_inches='tight', dpi=150)
    print("Plot save to: " + filepath)


def parse (s):
    arr = s.split()
    a = float(arr[0])
    b = float(arr[1])
    return a, b


def main():
    if len(sys.argv) < 3 :
        assert("parameters are <input params filepath> <input data points filepath> <plot save filepath>")
        
    a, b = [None, None]
    header = None
    with open(sys.argv[1]) as file:
        header = file.readline().replace("\n", "")
        a, b = parse(file.readline())
        
    
    x, y = [], []
    with open(sys.argv[2]) as file:
        for l in file:
            row = l.split()
            x.append(float(row[0]))
            y.append(float(row[1]))


    plot(sys.argv[3], header, a, b, x, y)



if __name__ == "__main__":
    main()
