#ifndef PIESHKOV_DCC_L2_TASKS_HPP
#define PIESHKOV_DCC_L2_TASKS_HPP

#include "Task.hpp"
#include "ColorPulse.hpp"
#include <array>
#include <vector>
#include <string>

class L2Task1 final : public Task, ColorPulse {
	std::string m_content = {};

	void run();

public:
	void renderWindow() override final;

	L2Task1();
};

class L2Task2 final : public Task {
	std::vector<int> m_dims = {100, 200, 300, 400, 500, 600};
	std::array<std::vector<double>, 2> m_dims_times = {{{}, {}}};
	std::vector<double> m_threads;
	int m_rep_count = 10;
	int m_dim_thread = 200;

	double inner(const int n, const int num_threads = 0);

public:
	void renderWindow() override final;

	L2Task2();
};

class L2Task3 final : public Task {
	int m_dim = 1;
	int m_n_threads = 1;
	int m_count = 1;
	std::string m_total;

	double sum(size_t n, std::stringstream &os, bool write);

public:
	void renderWindow() override final;

	L2Task3();
};

#endif //PIESHKOV_DCC_L2_TASKS_HPP
