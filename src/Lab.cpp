#include "Lab.hpp"
#include <imgui.h>

Lab::Lab(const char *menuBarName, std::vector<std::shared_ptr<Task>> tasks)
	: m_menuBarName(menuBarName), m_tasks(std::move(tasks)) {}

void Lab::renderMenu() {
	if (ImGui::BeginMenu(m_menuBarName)) {
		for (const auto &task: m_tasks) {
			task->renderMenu();
		}

		ImGui::EndMenu();
	}
}

void Lab::renderWindow() {
	for (const auto &task: m_tasks) {
		task->renderWindow();
	}
}