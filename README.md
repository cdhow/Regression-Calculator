# Regression-Calculator

Useful regression parameters and statistics for provided data.

## Description

This program takes data points as input and outputs to file the regression parameters for either Exponential, Power and Linear Regression.
The program also calculates the R-Squared statistic for each regression type.

### Input data format

* Text file.
* x and y values are separated by spaces.
* Each data point (x, y) is separated by a new line. 
* There is a sample data.txt file include in this repository.

### Executing program

* Command line arguments are: input file and regression type flag.
* flag -e: exponential regression.
* flag -p: power regression.
* flag -l: linear regression.
* Example: data.txt -e
* A plot.py python script is included for plotting the regression function.


### Output

The output is the two regression parameters used for each regression type and the corresponding R-Squared statistic.
The plot.py python script can use the parameters to plot the regression function.
