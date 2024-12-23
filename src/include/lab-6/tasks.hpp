#ifndef PIESHKOV_DCC_L6_TASKS_HPP
#define PIESHKOV_DCC_L6_TASKS_HPP

#include "Task.hpp"
#include "Window.hpp"
#include "omp-utils.hpp"
#include "Stopwatch.hpp"
#include <omp.h>

#include <imgui.h>
#include <imgui-SFML.h>
#include <implot.h>
#include <vector>
#include <iostream>
#include <random>
#include <format>

// Google: grayscale from rgb
inline static constexpr ImVec4 grayscale(const ImVec4 color) {
    auto s = 0.299f * color.x + 0.587f * color.y + 0.114f * color.z;
    return ImVec4(s, s, s, color.w);
}

class L6Task final : public Task {
    enum class CellType : uint16_t {
        Bush = 'B',
        Tree = 'T',
        Dirt = '.',
        BushOnFire = 'b' + (1 << 8),
        TreeOnFire = 't' + (1 << 8),
        TreeOnFire2 = 'r' + (1 << 8)
    };

    static constexpr const ImVec4 colors[4] = {
        ImVec4(0.4, 0.5, 0.2, 1),
        ImVec4(0.3, 0.8, 0.2, 1),
        ImVec4(0, 0.6, 0.1, 1),
        ImVec4(1, 0.317, 0, 1)
    };
    static constexpr const ImVec4 pausedColors[4] = {
        grayscale(colors[0]),
        grayscale(colors[1]),
        grayscale(colors[2]),
        grayscale(colors[3])
    };

    static constexpr double delay = 2.0;
    static constexpr const auto fmt = "Last iteration took {:.3f} sec. ({} thread{})";
    double m_lastWt = 0;
    int m_newSize = 30;
    int m_size = 10;
    bool m_isRunning = false;
    int m_speed = 1;
    float m_dirtFreq = 20.0f;
    std::vector<CellType> m_field;
    std::string m_duration = std::format(fmt, 0.0, 0.0, 's');
    int m_threads = 1;

    bool isOnFire(CellType cell);
    const ImVec4 &getColorForCell(CellType cell);

    void outputField();

    void generateField();

    bool willCellBurn(const CellType *cell, bool xFirst, bool xLast, bool yFirst, bool yLast);

    void step();

    void putFire();

public:
    void renderWindow() override final;
    
    L6Task();
};


#endif //PIESHKOV_DCC_L6_TASKS_HPP
