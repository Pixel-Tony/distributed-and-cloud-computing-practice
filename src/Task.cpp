#include "Task.hpp"
#include <implot.h>

bool Task::beginWindow(const char *label) {
    if (label == nullptr)
        label = m_title;

    auto value = ImGui::Begin(label, &m_isOpen);
    return value;
}

Task::Task(const char *title) : m_title(title) {}


void Task::renderMenu() {
    if (ImGui::MenuItem(m_title, nullptr, nullptr, !m_isOpen))
        m_isOpen = true;
}

void Task::autoFitNextAxesIf(bool cond) {
    if (cond) {
        ImPlot::SetNextAxesToFit();
    }
}

void Task::autoFitNextAxesIf(bool *cond) {
    if (*cond) {
        ImPlot::SetNextAxesToFit();
        *cond = false;
    }
}
