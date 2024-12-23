#include "lab-2/tasks.hpp"
#include "Window.hpp"
#include "Stopwatch.hpp"
#include "implot.h"
#include "omp-utils.hpp"
#include <format>
#include <omp.h>

L2Task2::L2Task2() : Task("Task 2.2: Matrix multiplication") {}

void L2Task2::renderWindow() {
	if (!m_isOpen) return;

	std::array<bool, 2> plotNeedsRebuild = {false, false};

	// Inputs
	setNextWindowPosSize(0, 0, 0.5, 1);
	if (beginWindow()) {
		int count = static_cast<int>(m_dims.size());
		ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.5f);

		if (ImGui::SliderInt("Matrices count", &count, 1, 12)) {
			m_dims.resize(count, 1);
		}

		ImGui::Spacing();
		for (int i = 0; i < count; ++i) {
			ImGui::SliderInt(std::format("Matrix #{} size (1..1500)", i + 1).c_str(), &m_dims.at(i), 1, 1000);
		}
		ImGui::Spacing();
		ImGui::SliderInt("Matrix for (time/n_thread) test", &m_dim_thread, 1, 1000);
		ImGui::SliderInt("Number of repeats for (time/n_thread) test", &m_rep_count, 1, 100);
		ImGui::Spacing();
		if (ImGui::Button("Run for different sizes")) {
			auto &[xs, ys] = m_dims_times;
			std::sort(m_dims.begin(), m_dims.end());
			xs.clear();
			ys.clear();
			for (auto dim: m_dims) {
				xs.push_back(static_cast<double>(dim));
				ys.push_back(inner(dim));
			}
			plotNeedsRebuild[0] = true;
		}
		if (ImGui::Button("Run for different thread counts")) {
			m_threads.clear();
			for (int i = 0, max = 2 * omp_get_num_procs(); i < max; ++i) {
				double average = 0;
				for (int j = 0; j < m_rep_count; ++j) {
					average += inner(m_dim_thread, i + 1);
				}
				m_threads.push_back(average / m_rep_count);
			}
			plotNeedsRebuild[1] = true;
		}
		ImGui::PopItemWidth();
	}
	ImGui::End();

	// Plotting
	setNextWindowPosSize(0.5, 0, 0.5, 1);
	if (ImGui::Begin("Task 2.2: results")) {
		if (ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, -1))) {
			autoFitNextAxesIf(plotNeedsRebuild[0]);
			if (ImPlot::BeginPlot("Time per matrix size")) {
				auto [xs, ys] = m_dims_times;
				ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 5);
				if (!ys.empty()) {
					ImPlot::PlotLine("time/size", xs.data(), ys.data(), static_cast<int>(xs.size()));
				}
				ImPlot::EndPlot();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		if (ImGui::BeginChild("ChildR", ImVec2(0, -1))) {
			autoFitNextAxesIf(plotNeedsRebuild[1]);
			if (ImPlot::BeginPlot("Time per thread count")) {
				ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 5);
				if (!m_threads.empty()) {
					ImPlot::PlotLine("time/num_threads", m_threads.data(), static_cast<int>(m_threads.size()), 1, 1);
				}
				ImPlot::EndPlot();
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}


double L2Task2::inner(const int n, const int num_threads) {
	using matrix = std::vector<double>;

	auto A = matrix(n * n);
	auto B = matrix(n * n);
	auto C = matrix(n * n, 0.0);

	for (int i = 0; i < n * n; ++i) {
		A[i] = i / 2.;
		B[i] = i / 6.;
	}

	setThreadNum(num_threads);
	Stopwatch stopwatch;
	{
#pragma omp parallel for shared(A, B, C, n) default(none)
		for (int i = 0; i < n; ++i) {
			for (int k = 0; k < n; ++k) {
				auto Ci = C.data() + i * n;
				auto Aik = A[i * n + k];
				auto Bk = B.data() + k * n;
				for (int j = 0; j < n; ++j) {
					*(Ci + j) += Aik * (*(Bk + j));
				}
			}
		}
	}
	return stopwatch.reset();
}