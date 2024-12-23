#include "lab-4/tasks.hpp"
#include "Stopwatch.hpp"
#include "omp-utils.hpp"
#include "Window.hpp"

#include <implot.h>
#include <optional>
#include <random>
#include <format>

L4Task::L4Task() : Task("Task 4.1: Gauss method") {}

void L4Task::renderWindow() {
    if (!m_isOpen) return;
    auto didDataUpdate = false;

    ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.25f);
    setNextWindowPosSize(0, 0, 0.5, 1);
    if (beginWindow()) {
        int count = static_cast<int>(m_dims.size());
        if (ImGui::SliderInt("N. matrices to compare", &count, 1, 10)) {
            m_dims.resize(count, 1);
        }
        ImGui::Spacing();
        for (int i = 0; i < count; ++i) {
            ImGui::SliderInt(std::format("M. #{}", i + 1).c_str(), &m_dims.at(i), 1, 150);
        }
        ImGui::Spacing();
        ImGui::SliderInt("N. runs for averaging", &m_avg, 1, 1000);
        ImGui::SliderInt("Dimension of matrix for thread num tests", &m_dim_thr, 1, 150);
        ImGui::Spacing();
        if (ImGui::Button("Run")) {
            std::sort(m_dims.begin(), m_dims.end());
            m_times.clear();
            m_times_x = {m_dims.begin(), m_dims.end()};
            for (const auto dim : m_dims) {
                double acc = 0;
                for (int i = 0; i < m_avg; ++i)
                    acc += inner(dim);
                m_times.push_back(static_cast<float>(acc / m_avg));
            }
            m_times_thr.clear();
            for (int i = 0, max = 2 * omp_get_num_procs(); i < max; ++i) {
                double acc = 0;
                for (int j = 0; j < m_avg; ++j)
                    acc += inner(m_dim_thr, i + 1);
                m_times_thr.push_back(static_cast<float>(acc / m_avg));
            }
            didDataUpdate = true;
        }
    }
    ImGui::End();
    // Plots
    setNextWindowPosSize(0.5, 0, 0.5, 1);
    if (ImGui::Begin("Task 4.1: Results")) {
        autoFitNextAxesIf(didDataUpdate);
        if (ImPlot::BeginPlot("Time per matrix dim")) {
            if (!m_times.empty()) {
                ImPlot::SetupAxes("dim(M)", "time, s");
                ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 5);
                ImPlot::PlotLine(
                    "##none",
                    m_times_x.data(),
                    m_times.data(),
                    static_cast<int>(m_times.size())
                );
            }
            ImPlot::EndPlot();
        }

        autoFitNextAxesIf(didDataUpdate);
        if (ImPlot::BeginPlot("Time per thread count")) {
            if (!m_times_thr.empty()) {
                ImPlot::SetupAxes("n. threads", "time, s");
                ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 5);
                ImPlot::PlotLine(
                    "##none",
                    m_times_thr.data(),
                    static_cast<int>(m_times_thr.size()),
                    1,
                    1
                );
            }
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
    ImGui::PopItemWidth();
}

std::optional<std::vector<double>> L4Task::gauss(int n, std::vector<double> &A, std::vector<double> &B) {
    // Forward movement
    for (int i = 0; i < n; ++i) {
        auto imax = 0;
        {
            double max = 0;
            for (int j = i; j < n; ++j) {
                auto Aji = A[j * n + i];
                if (auto absAji = std::abs(Aji); absAji > max) {
                    max = absAji;
                    imax = j;
                }
            }
        }
        // Singular matrix
        if (A[imax * n + i] == 0) {
            return std::nullopt;
        }
        // Swap rows to move maximum element to the diagonal
        if (imax != i) {
            std::swap_ranges(A.begin() + i * n, A.begin() + (i + 1) * n, A.begin() + imax * n);
        }
        std::swap_ranges(B.begin() + i, B.begin() + i + 1, B.begin() + imax);

        // Elementary operations cycle producing zeros under main diagonal
#pragma omp parallel for shared(A, B, n, i) default(none)
        for (int j = i + 1; j < n; ++j) {
            auto coef = A[j * n + i] / A[i * (n + 1)];
            for (int k = 0; k < n; ++k) {
                A[j * n + k] -= coef * A[i * n + k];
            }
            B[j] -= coef * B[i];
        }
    }

    // Backward movement
    std::vector<double> x(n);
    for (int i = n - 1; i > -1; --i) {
        auto xi = B[i];
        for (int j = i + 1; j < n; ++j) {
            xi -= A[i * n + j] * x[j];
        }
        x[i] = xi / A[(i + 1) * n];
    }
    return x;
}

double L4Task::inner(const int n, int thread_num) {
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::normal_distribution<double> normalCells(0, 3);

    auto A = std::vector<double>(n * n);
    auto B = std::vector<double>(n);
    for (int i = 0; i < n*n; ++i) {
        A[i] = normalCells(gen);
    }
    for (int i = 0; i < n; ++i) {
        B[i] = i;
    }

    setThreadNum(thread_num);
    Stopwatch sw;
    gauss(n, A, B);
    return sw.reset();
}
