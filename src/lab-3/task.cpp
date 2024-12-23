#include "lab-3/tasks.hpp"
#include "omp-utils.hpp"
#include "integrate.hpp"
#include "Window.hpp"
#include <cmath>
#include <format>
#include <implot.h>

L3Task::L3Task()
    : Task("Task 3.1: Numerical integration"),
      ColorPulse(ImVec4(1, 1, 1, 1), ImVec4(0, 1, 0, 1), 0.75) {}

inline static double f(double x) {
    return std::sin(2 * x) * std::cos(3 * x);
}

inline static double F(double x) {
    return 0.5 * (std::cos(x) - 0.2 * std::cos(5 * x));
}

inline static auto f_spec = "f(x) = sin(2x)cos(3x)";

void L3Task::renderWindow() {
    if (!m_isOpen) return;

    bool plotNeedsRebuild = false;
    ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.25f);
    // Setup
    setNextWindowPosSize(0, 0, 0.5, 1);
    if (beginWindow()) {
        static const float interval_min = 0;
        static const float interval_max = M_PI_2;
        ImGui::Text("%s", f_spec);

        ImGui::SliderFloat("a", &m_a, interval_min, m_b);
        ImGui::SliderFloat("b", &m_b, m_a, interval_max);

        ImGui::SliderInt("N. steps", &m_steps, 1, 1'000'000);
        ImGui::SliderInt("N. threads", &m_threads, 1, omp_get_num_procs());
        ImGui::SliderInt("N. tries (time averaging)", &m_tries, 1, 100);

        if (ImGui::Button("Run")) {
            auto [result, time] = integrateMidRect(f, m_a, m_b, m_steps, m_threads);
            auto result_true = F(m_b) - F(m_a);
            m_output = std::format(fmt, result, time, result_true, std::abs(result - result_true));
            ColorPulse::refresh();
        }
        if (ImGui::Button("Run multiple")) {
            m_graph_threads.clear();
            m_graph_results.clear();
            for (int i = 0, max = 2 * omp_get_num_procs(); i < max; ++i) {
                double time = 0;
                for (int j = 0; j < m_tries; ++j) {
                    time += integrateMidRect(f, m_a, m_b, m_steps, i + 1).second;
                }
                m_graph_threads.push_back(static_cast<float>(time / m_tries));
            }
            for (const auto v: m_graph_results_x) {
                double result = 0;
                for (int j = 0; j < m_tries; ++j) {
                    result += integrateMidRect(f, m_a, m_b, v).first;
                }
                m_graph_results.push_back(static_cast<float>(result / m_tries));
            }
            plotNeedsRebuild = true;
        }
        ImGui::PushStyleColor(ImGuiCol_Text, getTextColor<ColorPulse::CurveT::Cubic>());
        ImGui::Text(m_output.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::End();
    ImGui::PopItemWidth();

    // Plotting
    setNextWindowPosSize(0.5, 0, 0.5, 1);
    if (ImGui::Begin("Task 3.1: Results")) {

        if (plotNeedsRebuild) {
            ImPlot::SetNextAxesToFit();
        }
        if (ImPlot::BeginPlot("Time per thread count")) {
            if (!m_graph_threads.empty()) {
                ImPlot::SetupAxes("thread count", "time, s");
                ImPlot::PlotLine(
                    "time/n_threads",
                    m_graph_threads.data(),
                    static_cast<int>(m_graph_threads.size()),
                    1,
                    1
                );
            }
            ImPlot::EndPlot();
        }
        if (plotNeedsRebuild) {
            ImPlot::SetNextAxesToFit();
        }
        if (ImPlot::BeginPlot("Result per steps count")) {
            if (!m_graph_results.empty()) {
                ImPlot::SetupAxes("step count", "result value");
                ImPlot::PlotLine(
                    "result/n_steps",
                    m_graph_results_xf.data(),
                    m_graph_results.data(),
                    static_cast<int>(m_graph_results.size())
                );
            }
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
}