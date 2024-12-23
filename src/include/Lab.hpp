#ifndef PIESHKOV_DCC_LAB_HPP
#define PIESHKOV_DCC_LAB_HPP

#include "Task.hpp"
#include <memory>
#include <vector>

class Lab: public IRenderable {
	const char *m_menuBarName;
	std::vector<std::shared_ptr<Task>> m_tasks;

protected:
	Lab(const char *menuBarName, std::vector<std::shared_ptr<Task>> tasks);

public:
	void renderMenu() override final;
	void renderWindow() override final;
};




#endif //PIESHKOV_DCC_LAB_HPP
