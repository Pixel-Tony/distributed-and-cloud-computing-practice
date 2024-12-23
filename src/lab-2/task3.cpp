#include "lab-2/tasks.hpp"
#include "Window.hpp"
#include "Stopwatch.hpp"
#include "omp-utils.hpp"
#include <imgui.h>
#include <omp.h>
#include <format>
#include <cmath>
#include <sstream>

L2Task3::L2Task3() : Task("Task 2.3: Matrix row sums") {}


void L2Task3::renderWindow() {
	if (!m_isOpen) return;

	setNextWindowPosSize(0, 0, 1, 1);
	if (beginWindow()) {
		ImGui::SliderInt("Matrix size", &m_dim, 1, 5000);
		ImGui::SliderInt("Thread num", &m_n_threads, 1, omp_get_num_procs());
		ImGui::SliderInt("Reps count", &m_count, 1, 60);

		if (ImGui::Button("Run")) {
			std::stringstream os;

			double total, dur;
			{
				Stopwatch sw{};
				for (int i = 0; i < m_count; ++i) {
					total = this->sum(m_dim, os, false);
				}
				dur = sw.reset() / m_count;
				total = this->sum(m_dim, os, true);
			}
			m_total = std::format("{} @ {}ms, {} thread(s)\n{}", total, std::round(dur * 1e3), m_n_threads, os.str());
		}
		ImGui::Spacing();
		ImGui::Text("%s", m_total.c_str());
		ImGui::End();
	}
}

double L2Task3::sum(size_t n, std::stringstream &os, bool write) {
	auto A = std::vector<double>(n * n, 0.0);
	for (size_t i = 0; i < n * n; ++i)
		A[i] = static_cast<double>(i);
	double total = 0;

	setThreadNum(m_n_threads);
#pragma omp parallel for shared(A, n, os, write) reduction(+:total) default(none)
	for (size_t i = 0; i < n; ++i) {
		double sum = 0;
		auto i0 = i * n;
		const auto ij = i0 + n;
		for (; i0 < ij; ++i0) {
			sum += A[i0];
		}
		total += sum;

#pragma omp critical
		if (write && i < 25) {
			os << std::format("sum(A[{}]) = {}\n", i, sum);
		}
	}
	return total;
}
