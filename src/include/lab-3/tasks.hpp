#ifndef PIESHKOV_DCC_L3_TASKS_HPP
#define PIESHKOV_DCC_L3_TASKS_HPP

#include "Task.hpp"
#include "ColorPulse.hpp"
#include <functional>
#include <cmath>
#include <string>
#include <format>

class L3Task final : public Task, ColorPulse {
	static constexpr auto fmt = 
		"Result:                            {:.5f}\n"
		"Time taken:                        {:.3f} sec.\n"
		"Result by Newton-Leibniz formula:  {:.5f}\n"
        "Error:                             {:.5f}";
	
	const std::vector<int> m_graph_results_x = {2, 10, 20, 50, 100, 150};
	const std::vector<float> m_graph_results_xf = {m_graph_results_x.begin(), m_graph_results_x.end()};
	
	std::vector<float> m_graph_threads;
	std::vector<float> m_graph_results;
	
	float m_a = 0;
	float m_b = M_PI_2;
	int m_steps = 10;
	int m_threads = 1;
    int m_tries = 10;
	std::string m_output = std::format(fmt, NAN, NAN, NAN, NAN);
	
public:
	void renderWindow() override final;

	L3Task();
};

#endif //PIESHKOV_DCC_L3_TASKS_HPP
