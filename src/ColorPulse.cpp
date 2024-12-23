#include "ColorPulse.hpp"

ColorPulse::ColorPulse(const ImVec4 baseCol, const ImVec4 mixCol, const double threshold)
    : m_baseCol(baseCol), m_mixCol(mixCol), m_threshold(threshold), m_last_wt(omp_get_wtime()) {}

void ColorPulse::refresh() {
    refresh(omp_get_wtime());
}

void ColorPulse::refresh(double wt) {
    m_last_wt = wt;
}
