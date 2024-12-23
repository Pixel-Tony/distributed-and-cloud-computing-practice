#include "lab-6/tasks.hpp"

L6Task::L6Task() : Task("Task 6.1: A Song of Bush and Fire") {}

bool L6Task::isOnFire(L6Task::CellType cell) { return (1 << 8) & static_cast<uint16_t>(cell); }

void L6Task::renderWindow() {
    if (!m_isOpen) return;

    setNextWindowPosSize(0, 0, 0.5, 1);
    if (beginWindow()) {
        ImGui::Text("%s", "- Setup -");
        ImGui::SliderInt("Field size", &m_newSize, 10, 50);
        ImGui::SliderFloat("Dirt frequency", &m_dirtFreq, 5.0f, 95.0f);
        if (ImGui::Button("Generate")) {
            generateField();
        }
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("%s", "- Replay -");
        ImGui::SliderInt("Speed", &m_speed, 1, 16);
        if (ImGui::Button(m_isRunning ? "Pause" : "Resume")) {
            m_isRunning = !m_isRunning;
        }
        ImGui::SameLine();
        if (ImGui::Button("Put random fire") && !m_field.empty()) {
            putFire();
        }
        ImGui::Text("%s", m_duration.c_str());
    }
    ImGui::End();

    step();

    setNextWindowPosSize(0.5, 0, 0.5, 1);
    if (ImGui::Begin("The Field", nullptr)) {
        outputField();
    }
    ImGui::End();
}

void L6Task::putFire() {
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<int> distribution(0, m_size * m_size - 1);
    auto index = distribution(gen);
    while (true) {
        for (auto &cell: m_field) {
            if (cell == CellType::Bush || cell == CellType::Tree) {
                --index;
                if (index == 0) {
                    cell = cell == CellType::Bush
                           ? CellType::BushOnFire
                           : CellType::TreeOnFire;
                    return;
                }
            }
        }
    }
}

void L6Task::step() {
    if (m_field.empty()) {
        return;
    }
    auto currentWt = omp_get_wtime();
    if (!m_isRunning) {
        m_lastWt = currentWt;
        return;
    }
    if ((currentWt - m_lastWt) < delay / m_speed) {
        return;
    }
    auto newField = std::vector<CellType>(m_size * m_size, CellType::Dirt);
    m_lastWt = currentWt;

    setThreadNum(m_threads);
    Stopwatch sw;
#pragma omp parallel for shared(m_field, newField) default(none)
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            const auto *cell = m_field.data() + i * m_size + j;
            auto &newCell = newField[i * m_size + j];
            switch (*cell) {
                case CellType::Bush:
                    [[fallthrough]];
                case CellType::Tree: {
                    auto xFirst = j == 0;
                    auto xLast = j == m_size - 1;
                    auto yFirst = i == 0;
                    auto yLast = i == m_size - 1;
                    bool willBurn = willCellBurn(cell, xFirst, xLast, yFirst, yLast);
                    auto isTree = *cell == CellType::Tree;
                    if (isTree) {
                        newCell = willBurn ? CellType::TreeOnFire : CellType::Tree;
                    } else {
                        newCell = willBurn ? CellType::BushOnFire : CellType::Bush;
                    }
                }
                    break;

                case CellType::BushOnFire:
                    [[fallthrough]];
                case CellType::TreeOnFire2:
                    newCell = CellType::Dirt;
                    break;
                case CellType::TreeOnFire:
                    newCell = CellType::TreeOnFire2;
                    break;
                case CellType::Dirt:
                    newCell = CellType::Dirt;
                    break;
            }
        }
    }
    auto elapsed = sw.reset();
    m_duration = std::format(fmt, elapsed, m_threads, m_threads > 1 ? "s" : "");
    m_field = newField;
}

bool L6Task::willCellBurn(const L6Task::CellType *cell, bool xFirst, bool xLast, bool yFirst, bool yLast) {
    auto willBurn = (
        (!xFirst && isOnFire(*(cell - 1)))
        || (!xLast && isOnFire(*(cell + 1)))
        || (!yFirst && isOnFire(*(cell - m_size)))
        || (!yLast && isOnFire(*(cell + m_size)))
    );
    return willBurn || 2 < (
        // upper diagonals
        (!xFirst && !yFirst && isOnFire(*(cell - m_size - 1)))
        + (!xFirst && !yLast && isOnFire(*(cell - m_size + 1)))
        // lower diagonals
        + (!xLast && !yFirst && isOnFire(*(cell + m_size - 1)))
        + (!xLast && !yLast && isOnFire(*(cell + m_size + 1)))
    );
}

void L6Task::generateField() {
    m_size = m_newSize;

    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_real_distribution<float> distribution(0, 100.0);

    m_field.clear();
    m_field.reserve(m_size * m_size);
    const auto secondThreshold = (100.0 + m_dirtFreq) / 2;
    for (int i = 0; i < m_size * m_size; ++i) {
        auto prob = distribution(gen);

        CellType cell;
        if (prob >= secondThreshold) {
            cell = CellType::Bush;
        } else if (prob >= m_dirtFreq) {
            cell = CellType::Tree;
        } else {
            cell = CellType::Dirt;
        }
        m_field.push_back(cell);
    }
}

void L6Task::outputField() {
    if (m_field.empty()) {
        ImGui::Text("(Generate one, dumbass)");
        return;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetFontSize() / 4, 0));
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            auto cell = m_field[i * m_size + j];
            char chr = static_cast<char>(cell);
            auto color = getColorForCell(cell);
            if (j) ImGui::SameLine();
            ImGui::TextColored(color, "%c", chr);
        }
    }
    ImGui::PopStyleVar();
}

const ImVec4 &L6Task::getColorForCell(L6Task::CellType cell) {
    auto arr = m_isRunning ? colors : pausedColors;
    switch (cell) {
        case CellType::Bush:
            return arr[1];
        case CellType::Tree:
            return arr[2];
        case CellType::BushOnFire:
            [[fallthrough]];
        case CellType::TreeOnFire2:
            [[fallthrough]];
        case CellType::TreeOnFire:
            return arr[3];
        case CellType::Dirt:
            return arr[0];
        default:
            return arr[0];
    }
}
