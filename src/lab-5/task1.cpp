#include "lab-5/tasks.hpp"
#include "Window.hpp"
#include "integrate.hpp"

#include <implot.h>
#include <format>
#include <omp.h>

inline static double f(double x) {
    return 4.0 / (x * x + 1.0);
}

L5Task1::L5Task1()
    : Task("Task 5.1: Integration method"),
      ColorPulse(ImVec4(1, 1, 1, 1), ImVec4(0, 1, 0, 1), 0.75) {}

void L5Task1::renderWindow() {
    if (!m_isOpen) return;

    setNextWindowPosSize(0, 0, 0.5, 1);
    if (beginWindow("Task 5.1: Calculating PI via integration")) {
        ImGui::SliderInt("Steps count", &m_steps, 1, 10'000'000);
        ImGui::SliderInt("Threads count", &m_threads, 1, omp_get_num_procs());
        ImGui::Spacing();
        if (ImGui::Button("Run")) {
            auto [result1, time1] = integrateLeftRect(f, 0, 1, m_steps, m_threads);
            auto [result2, time2] = integrateMidRect(f, 0, 1, m_steps, m_threads);
            auto [result3, time3] = integrateTrapezoidRect(f, 0, 1, m_steps, m_threads);

            m_output = std::format(
                fmt,
                PI_String,
                result1, std::abs(result1 - M_PI),
                result2, std::abs(result2 - M_PI),
                result3, std::abs(result3 - M_PI),
                time1,
                time2,
                time3
            );
            ColorPulse::refresh();
        }
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, getTextColor<ColorPulse::CurveT::Cubic>());
        ImGui::Text("%s", m_output.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

