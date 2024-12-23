#ifndef PIESHKOV_DCC_INTEGRATE_HPP
#define PIESHKOV_DCC_INTEGRATE_HPP

#include <utility>
#include <functional>

/// Find numerical approximation of given integral using the mid rectangles method.
/// \param f The function to integrate.
/// \param a The lower limit of integration.
/// \param b The upper limit of integration.
/// \param steps The number of integration steps.
/// \param num_threads The number of threads to use.
/// \return The (integration result, time taken) pair. 
std::pair<double, double>
integrateMidRect(const std::function<double(double)> &f, double a, double b, int steps, int num_threads = 0);

/// Find numerical approximation of given integral using the left rectangles method.
/// \param f The function to integrate.
/// \param a The lower limit of integration.
/// \param b The upper limit of integration.
/// \param steps The number of integration steps.
/// \param num_threads The number of threads to use.
/// \return The (integration result, time taken) pair. 
std::pair<double, double>
integrateLeftRect(const std::function<double(double)> &f, double a, double b, int steps, int num_threads = 0);

/// Find numerical approximation of given integral using the trapezoids method.
/// \param f The function to integrate.
/// \param a The lower limit of integration.
/// \param b The upper limit of integration.
/// \param steps The number of integration steps.
/// \param num_threads The number of threads to use.
/// \return The (integration result, time taken) pair. 
std::pair<double, double>
integrateTrapezoidRect(const std::function<double(double)> &f, double a, double b, int steps, int num_threads = 0);

#endif //PIESHKOV_DCC_INTEGRATE_HPP
