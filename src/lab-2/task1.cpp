#include "lab-2/tasks.hpp"
#include "imgui.h"
#include "Stopwatch.hpp"

#include <string>
#include <omp.h>
#include <format>
#include <sstream>

L2Task1::L2Task1()
    : Task("Task 2.1: Looking around"),
      ColorPulse(ImVec4(1, 1, 1, 1), ImVec4(0, 1, 0, 1), 0.75) {
    run();
}

void L2Task1::run() {
    std::stringstream buf;
    buf << std::format("OpenMP v.{} loaded; {} cores available.\n", _OPENMP, omp_get_num_procs());
    {
        Stopwatch sw{};
        buf << std::format("Timestamp obtaining overhead: {}\n", sw.reset());
    }
    buf << std::format("Timer precision: {}\n", omp_get_wtick());

#pragma omp parallel shared(buf) default(none)
    {
#pragma omp critical
        {
            buf << std::format("Thread {} greets the world!\n", omp_get_thread_num());
        }
    }
    m_content = buf.str();
}

void L2Task1::renderWindow() {
    if (!m_isOpen) return;
    if (beginWindow()) {
        if (ImGui::Button("Run")) {
            run();
            ColorPulse::refresh();
        }
        ImGui::PushStyleColor(ImGuiCol_Text, getTextColor<ColorPulse::CurveT::Cubic>());
        ImGui::Text("%s", m_content.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::End();
}