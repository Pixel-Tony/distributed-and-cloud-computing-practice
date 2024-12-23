#include "integrate.hpp"
#include <omp-utils.hpp>
#include "Stopwatch.hpp"

std::pair<double, double>
integrateMidRect(const std::function<double(double)> &f, double a, double b, int steps, int num_threads) {
    double sum = 0;
    const auto dx = (b - a) / steps;
    const auto dx_2 = dx / 2.f;

    setThreadNum(num_threads);
    Stopwatch sw;
#pragma omp parallel for shared(steps, f, dx, a, dx_2) reduction(+:sum) default(none)
    for (int i = 0; i < steps; ++i) {
        auto x = a + dx * i + dx_2;
        sum += f(x);
    }
    auto dur = sw.reset();
    return {sum * dx, dur};
}


std::pair<double, double>
integrateLeftRect(const std::function<double(double)> &f, double a, double b, int steps, int num_threads) {
    double sum = 0;
    const auto dx = (b - a) / steps;

    setThreadNum(num_threads);
    Stopwatch sw;
#pragma omp parallel for shared(steps) firstprivate(dx, f, a) reduction(+:sum) default(none)
    for (int i = 0; i < steps; ++i) {
        auto x = a + dx * i;
        sum += f(x);
    }
    auto dur = sw.reset();
    return {sum * dx, dur};
}

std::pair<double, double>
integrateTrapezoidRect(const std::function<double(double)> &f, double a, double b, int steps, int num_threads) {
    double sum = 0;
    const auto dx = (b - a) / steps;

    setThreadNum(num_threads);
    Stopwatch sw;
#pragma omp parallel for shared(steps) firstprivate(dx, f, a) reduction(+:sum) default(none)
    for (int i = 0; i < steps; ++i) {
        auto x1 = a + dx * i;
        auto x2 = x1 + dx;
        sum += (f(x1) + f(x2));
    }
    auto dur = sw.reset();
    return {sum * dx / 2.0, dur};
}



