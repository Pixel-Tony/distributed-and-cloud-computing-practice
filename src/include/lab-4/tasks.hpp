#ifndef PIESHKOV_DCC_L4_TASK_HPP
#define PIESHKOV_DCC_L4_TASK_HPP

#include "Task.hpp"
#include <vector>
#include <optional>

class L4Task final : public Task {
    std::optional<std::vector<double>> gauss(int n, std::vector<double> &A, std::vector<double> &B);

    double inner(const int n, int thread_num = 0);

    std::vector<int> m_dims = {50};
    std::vector<float> m_times_x;
    std::vector<float> m_times;
    int m_dim_thr = 100;
    int m_avg = 20;
    std::vector<float> m_times_thr;
    
public:
    void renderWindow() override final;
    
    L4Task();
};

#endif //PIESHKOV_DCC_L4_TASK_HPP
