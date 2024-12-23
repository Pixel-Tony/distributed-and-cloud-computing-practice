#ifndef PIESHKOV_DCC_TASK_HPP
#define PIESHKOV_DCC_TASK_HPP

#include "IRenderable.hpp"

class Task : public IRenderable {
	const char *m_title;
protected:
	bool m_isOpen = false;
	bool beginWindow(const char *label = nullptr);
	explicit Task(const char *title);
	static void autoFitNextAxesIf(bool cond);
	void autoFitNextAxesIf(bool *cond);

public:
	void renderMenu() override final;
};

#endif //PIESHKOV_DCC_TASK_HPP
