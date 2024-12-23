#include "Window.hpp"
#include "labs.hpp"
#include <memory>

#ifdef WIN32_EXECUTABLE
#define ENTRY_POINT WinMain
#else
#define ENTRY_POINT main
#endif

int ENTRY_POINT() {
	Window win{L"Лабораторна робота №1", {800, 600}, 60, {
        std::make_shared<Lab1>(),
		std::make_shared<Lab2>(),
		std::make_shared<Lab3>(),
		std::make_shared<Lab4>(),
        std::make_shared<Lab5>(),
		std::make_shared<Lab6>()
	}};
	win.mainLoop();
}