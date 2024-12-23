#ifndef PIESHKOV_DCC_L5_TASKS_HPP
#define PIESHKOV_DCC_L5_TASKS_HPP

#include "Task.hpp"
#include "pie.hpp"
#include "ColorPulse.hpp"
#include <vector>
#include <format>
#include <limits>
#include <cmath>



class L5Task1 final : public Task, ColorPulse {
    static constexpr const auto fmt =
        "True value:              {}\n"
        "Calculated (left rects): {:.20f}, err: {:.3e}\n"
        "Calculated (mid rects):  {:.20f}, err: {:.3e}\n"
        "Calculated (trapezoids): {:.20f}, err: {:.3e}\n\n"
        "Time taken (left rects): {:.3f} sec.\n"
        "Time taken (mid rects):  {:.3f} sec.\n"
        "Time taken (trapezoids): {:.3f} sec.";
    int m_steps = 10;
    int m_threads = 1;
    std::string m_output = std::format(fmt, PI_String, NAN, 0.0, NAN, 0.0, NAN, 0.0, NAN, NAN, NAN);

public:
    void renderWindow() override final;

    L5Task1();
};


class L5Task2 final : public Task, ColorPulse {
    static constexpr const auto fmt =
        "True value: {}\n"
        "Estimated:  {:.20f}, err = {:.3e}\n"
        "Time taken: {:.3f}";

    int m_count = 100;
    int m_threads = 1;
    std::string m_output;

    std::pair<double, double> monteCarlo();

public:
    void renderWindow() override final;

    L5Task2();

};


#endif //PIESHKOV_DCC_L5_TASKS_HPP
