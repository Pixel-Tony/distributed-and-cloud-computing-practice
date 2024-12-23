#include "lab-5/tasks.hpp"
#include "Window.hpp"
#include "Stopwatch.hpp"
#include "omp-utils.hpp"
#include <imgui.h>

#include <random>

L5Task2::L5Task2()
    : Task("Task 5.2: Monte-Carlo method"),
      ColorPulse(ImVec4(1, 1, 1, 1), ImVec4(0, 1, 0, 1), 0.75) {}

void L5Task2::renderWindow() {
    if (!m_isOpen) return;

    setNextWindowPosSize(0.5, 0, 0.5, 1);
    if (beginWindow()) {
        ImGui::SliderInt("Point count", &m_count, 10, 10'000'000);
        ImGui::SliderInt("Thread count", &m_threads, 1, omp_get_num_procs());
        if (ImGui::Button("Run")) {
            auto [result, time] = monteCarlo();
            m_output = std::format(
                fmt,
                PI_String,
                result, std::abs(result - M_PI),
                time
            );
            ColorPulse::refresh();
        }
        ImGui::PushStyleColor(ImGuiCol_Text, getTextColor<ColorPulse::CurveT::Cubic>());
        ImGui::Text("%s", m_output.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

std::pair<double, double> L5Task2::monteCarlo() {
    std::random_device device;
    std::default_random_engine generator(device());
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    int sum = 0;
    setThreadNum(m_threads);
    Stopwatch sw;
#pragma omp parallel for shared(distribution, generator) reduction(+:sum) default(none)
    for (int i = 0; i < m_count; ++i) {
        auto x = distribution(generator);
        auto y = distribution(generator);
        auto dist = x * x + y * y;
        sum += (dist <= 1);
    }
    return {4.0 * sum / m_count, sw.reset()};
}
