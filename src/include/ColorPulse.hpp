#ifndef PIESHKOV_DCC_COLORPULSE_HPP
#define PIESHKOV_DCC_COLORPULSE_HPP

#include <imgui.h>
#include <omp.h>

class ColorPulse {
    const ImVec4 m_baseCol;
    const ImVec4 m_mixCol;
    const double m_threshold;
    double m_last_wt;

public:
    enum class CurveT {
        Linear, Quadratic, Cubic, Quartic
    };

    ColorPulse(const ImVec4 baseCol, const ImVec4 mixCol, const double threshold);

    void refresh();

    void refresh(double wt);

    template<ColorPulse::CurveT curve>
    ImVec4 getTextColor(double curTime = 0) {
        if (curTime == 0) {
            curTime = omp_get_wtime();
        }
        auto color = m_baseCol;
        if (auto diff = curTime - m_last_wt; diff < m_threshold) {
            auto w = static_cast<float>(1.0 - diff / m_threshold);
            switch (curve) {
                case CurveT::Linear:
                    break;
                case CurveT::Quadratic:
                    w *= w;
                    break;
                case CurveT::Cubic:
                    w *= w * w;
                    break;
                case CurveT::Quartic:
                    w *= w;
                    w *= w;
                    break;
            }
            auto w_sum = w + 1;
            color.x = static_cast<float>((color.x + w * m_mixCol.x) / w_sum);
            color.y = static_cast<float>((color.y + w * m_mixCol.y) / w_sum);
            color.z = static_cast<float>((color.z + w * m_mixCol.z) / w_sum);
            color.w = static_cast<float>((color.w + w * m_mixCol.w) / w_sum);
        }
        return color;
    }
};

#endif //PIESHKOV_DCC_COLORPULSE_HPP
