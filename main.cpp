/*
 * This program reads in data points from file and
 * outputs "a" and "b" parameters for a regression line.
 * Also outputs R-Squared.
 * Command line flags are -l -p -e for linear, power and exponential
 * respectively.
 */


#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <numeric>

/*
 * Takes regression parameters and x and y data
 * and returns the R-squared statistic.
 */
double RSquared(std::pair<double, double>& params,
                std::vector<double>& x_data,
                std::vector<double>& y_data)
{
    double n = y_data.size();

    // First calculate the y mean.
    double y_mean = std::accumulate(y_data.begin(), y_data.end(), 0.0) / n;

    // Calculate the difference between each y and the y mean.
    std::vector<double> delta_mean_squared(y_data.size());

    for (double y : y_data) {
        double delta_squared = std::pow(y - y_mean, 2.0);
        delta_mean_squared.push_back(delta_squared);
    }

    // Calculate the distance from the estimated values to the mean.
    auto f = [params](double x) { return params.first  + params.second * x; }; // equation.
    std::vector<double> delta_estimated_squared(x_data.size());

    for (double x : x_data) {
        double delta_squared = std::pow(f(x) - y_mean, 2.0);
        delta_estimated_squared.push_back(delta_squared);
    }

    // R-squared is the mean of delta_estimated_squared / delta_mean_squared.
    double dms_mean = std::accumulate(delta_mean_squared.begin(), delta_mean_squared.end(), 0.0) / n;
    double des_mean = std::accumulate(delta_estimated_squared.begin(), delta_estimated_squared.end(), 0.0) / n;

    return des_mean / dms_mean;
}


/*
 * Returns the parameters for a Linear Regression
 * line given x and y data.
 */
std::pair<double, double> linearRegressionParams(std::vector<double>& x,
                                                 std::vector<double>& y,
                                                 double& r_squared)
{
    double n = x.size();
    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_x2 = 0.0;  // sum of x^2.
    double sum_xy = 0.0; // sum of x * y.

    // Calculate the sum terms of the formula.
    for (int i = 0; i < n; i++) {
        sum_x2 += x[i] * x[i];
        sum_xy += x[i] * y[i];
    }

    // Calculate a and b terms of the equation.
    double numerator = n * sum_xy - sum_x * sum_y;
    double denominator = n * sum_x2 - sum_x * sum_x;
    double slope = numerator / denominator;
    double intercept = (sum_y - slope * sum_x) / n;

    auto params = std::make_pair(intercept, slope);

    // Calculate R-squared.
    r_squared= RSquared(params, x, y);

    return params;
}


/*
 * Returns the parameters for a Power Regression
 * line given x and y data. Also updates the R-squared parameter.
 */
std::pair<double, double> powerRegressionParams(std::vector<double>& x,
                                                std::vector<double>& y,
                                                double& r_squared)
{
    // For Power Regression we take the log of
    // all x and y data points and perform
    // linear regression on them.

    std::for_each(x.begin(), x.end(),
                  [](double& x){ x = log(x); });
    std::for_each(y.begin(), y.end(),
                  [](double& y){ y = log(y); });

    // Linear regression on log of data.
    auto params = linearRegressionParams(x, y, r_squared);

    // Convert log(a) back to a.
    params.first = std::exp(params.first);

    return params;
}


/*
 * Returns the parameters for a Exponential Regression
 * line given x and y data. Also updates the R-squared parameter.
 */
std::pair<double, double> exponentialRegressionParams(std::vector<double>& x,
                                                      std::vector<double>& y,
                                                      double& r_squared)
{
    // For Exponential Regression we take the log of
    // all y data and perform linear regression.
    std::for_each(y.begin(), y.end(),
                  [](double& y){ y = log(y); });

    // Linear regression on log of data.
    auto params = linearRegressionParams(x, y, r_squared);

    // Convert log(a) and log(b) back to a and b.
    params.first = std::exp(params.first);
    params.second = std::exp(params.second);

    return params;
}


/*
 * Reads <u, v> data points from file where each
 * data point is on a new line.
 */
void parseData(const std::string& filepath,
               std::vector<double>& x_data,
               std::vector<double>& y_data)
{
    std::ifstream infile(filepath);

    if (!infile.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::cout << "Reading data from file..." << std::endl;

    double x, y;
    while (infile >> x >> y) {
        x_data.push_back(x);
        y_data.push_back(y);
    }
}


/*
 * Outputs regression parameters and R-squared to file.
 */
void outputParams(const std::string& filepath,
                  std::pair<double, double>& params,
                  double r_squared,
                  const std::string& regression_type)
{
    std::ofstream outfile(filepath);

    if (!outfile.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::cout << "Printing data to file..." << std::endl;

    outfile << regression_type << '\n'
            << params.first << ' '
            << params.second << '\n'
            << "R-Squared: " << r_squared << '\n';

}


int main(int argc, char **argv)
{
    if (argc < 3) {
        const std::string err = "usage: <input_file> <regression_type_flag>.";
        throw std::runtime_error(err);
    }

    const std::string input_file = argv[1];
    const char type = argv[2][1];

    // Get the data points from file.
    std::vector<double> x, y;
    parseData(input_file, x, y);

    std::pair<double, double> params;

    double r_squared;
    std::string reg_type;
    switch (type) {
        case 'l' :
            params = linearRegressionParams(x, y, r_squared);
            reg_type = "Linear";
            break;
        case 'p' :
            params = powerRegressionParams(x, y, r_squared);
            reg_type = "Power";
            break;
        case 'e' :
            params = exponentialRegressionParams(x, y, r_squared);
            reg_type = "Exponential";
            break;
        default :
            throw std::runtime_error("Invalid regression_type_flag."
                                     "\nFlags are:\n-l:linear\n-p:power\n-e:exponential");

    }

    const std::string output_filepath = "output/" + reg_type + "_params.txt";

    outputParams(output_filepath, params, r_squared, reg_type);

    return 0;
}
