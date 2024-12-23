#include "Lab.hpp"
#include "Window.hpp"
#include "implot.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <format>
#include <utility>

Window::Window(const sf::String &title, sf::Vector2u minSize, uint32_t fps, std::vector<std::shared_ptr<Lab>> labs)
	: m_rw(sf::VideoMode(1280u, 720u), title), m_minSize(minSize), m_labs(std::move(labs)) {
	if (!ImGui::SFML::Init(m_rw))
		throw;

	ImPlot::CreateContext();

	ImGui::GetIO().IniFilename = nullptr;

	m_rw.setFramerateLimit(fps);
}

Window::~Window() {
	ImPlot::DestroyContext();
	ImGui::SFML::Shutdown(m_rw);
}


void Window::mainLoop() {
	sf::Clock dt;

	while (m_rw.isOpen()) {
		for (sf::Event ev{}; m_rw.pollEvent(ev);) {
			ImGui::SFML::ProcessEvent(m_rw, ev);

			if (ev.type == sf::Event::Closed) {
				m_rw.close();
			} else if (ev.type == sf::Event::Resized) {
				auto w = std::max(ev.size.width, m_minSize.x);
				auto h = std::max(ev.size.height, m_minSize.y);

				auto wf = static_cast<float>(w);
				auto hf = static_cast<float>(h);

				m_rw.setView(sf::View{{0, 0, wf, hf}});
				m_rw.setSize({w, h});
			}
		}
		ImGui::SFML::Update(m_rw, dt.restart());
		render();
		m_rw.clear(m_bg_color);
		ImGui::SFML::Render(m_rw);
		m_rw.display();
	}
}

void Window::render() {
	const auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
					   | ImGuiWindowFlags_NoBringToFrontOnFocus;

	if (ImGui::BeginMainMenuBar()) {
		for (auto &lab: m_labs)
			lab->renderMenu();

		ImGui::EndMainMenuBar();
	}

	setNextWindowPosSize(1, 1, 1, 1);
	if (ImGui::Begin("#Fullscreen beginWindow", nullptr, flags)) {
		for (auto &x: m_labs)
			x->renderWindow();
	}
	ImGui::End();
}

void setNextWindowPosSize(float rx, float ry, float rw, float rh) {
	auto vp = ImGui::GetMainViewport();
	auto [w, h] = vp->WorkSize;
	auto [x, y] = vp->WorkPos;
	ImGui::SetNextWindowSize(ImVec2(w * rw, h * rh));
	ImGui::SetNextWindowPos(ImVec2(x + w * rx, y + h * ry));
}
