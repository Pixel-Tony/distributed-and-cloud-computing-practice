#include "lab-1/tasks.hpp"
#include "Window.hpp"


static constexpr const char *states[] = {
    "Idling",
    "Waiting for access",
    "Claimed file",
    "Writing to file",
    "Outputting file"
};

static std::mutex m_theFileMutex;
static std::stringstream m_theFile;
static std::mutex m_theOutputMutex;
static std::string m_theOutput;


void workload(const SharedResource<bool> &isDone,
              const SharedResource<Command> &commandSrc,
              const SharedResource<State> &state
) {
    using namespace std::chrono_literals;
    bool isRunning = true;
    while (isRunning) {
        std::this_thread::sleep_for(0.05s);
        auto command = commandSrc->exchange(Command::Empty);
        switch (command) {
            case Command::ClaimFile: {
                state->store(State::Waiting);
                {
                    std::lock_guard guard(m_theFileMutex);
                    state->store(State::ClaimedFile);
                    std::this_thread::sleep_for(4s);
                }
                state->store(State::Idle);
                break;
            }
            case Command::WriteToFile: {
                state->store(State::Waiting);
                {
                    std::lock_guard guard(m_theFileMutex);
                    state->store(State::WritingToFile);
                    std::this_thread::sleep_for(1s);
                    m_theFile << "Last updated by thread " << std::this_thread::get_id() << std::endl;
                }
                state->store(State::Idle);
                break;
            }
            case Command::OutputFile: {
                state->store(State::Waiting);
                {
                    std::lock_guard guard(m_theFileMutex);
                    state->store(State::OutputtingFile);
                    std::this_thread::sleep_for(0.5s);
                    {
                        std::lock_guard guard2(m_theOutputMutex);
                        m_theOutput = m_theFile.str();
                    }
                    m_theFile.str(std::string());
                }
                state->store(State::Idle);
                break;
            }
            case Command::Terminate:
                isRunning = false;
                break;
            case Command::Empty:
            default:
                std::this_thread::yield();
        }
    }
    *isDone = true;
}

L1Task::L1Task() : Task("Task 1.1: Multithreading") {}

void L1Task::dropThreadAt(int i) {
    size_t max = m_workers.size() - 1;
    for (size_t j = i; j < max; ++j) {
        *(m_workers.data() + j) = std::move(*(m_workers.data() + j + 1));
    }
    m_workers.pop_back();
}

void L1Task::renderWindow() {
    using namespace std::chrono_literals;

    if (!m_isOpen) return;

    setNextWindowPosSize(0, 0, 1, 1);
    if (beginWindow()) {
        ImGui::Text("Threads:\n* * *");

        for (int i = 0; i < static_cast<int>(m_workers.size());) {
            auto &worker = m_workers[i];
            if (*worker.isCompleted.get()) {
                if (worker.handle.joinable()) worker.handle.join();
                dropThreadAt(i);
                continue;
            }
            ImGui::PushID(i);
            std::stringstream buf;
            buf << "Thread #" << worker.handle.get_id() << ", " << states[static_cast<int>(worker.state->load())];
            ImGui::Text("%s", buf.str().c_str());
            ImGui::SameLine();

            auto state = worker.state->load();
            auto isThreadBusy = state != State::Idle;

            ImGui::BeginDisabled(isThreadBusy);
            {
                if (ImGui::Button("Claim file")) {
                    worker.command->store(Command::ClaimFile);
                }
                ImGui::SameLine();
                if (ImGui::Button("Write to file")) {
                    worker.command->store(Command::WriteToFile);
                }
                ImGui::SameLine();
                if (ImGui::Button("Output file")) {
                    worker.command->store(Command::OutputFile);
                }
                ImGui::SameLine();
                if (ImGui::Button("Terminate")) {
                    worker.command->store(Command::Terminate);
                }
            }
            ImGui::EndDisabled();
            ImGui::PopID();
            ++i;
        }
        ImGui::Text("* * *");
        if (ImGui::Button("Create thread")) {
            Worker worker{workload};
            m_workers.push_back(std::move(worker));
        }
        {
            std::lock_guard guard(m_theOutputMutex);
            ImGui::Text("File output:\n%s", m_theOutput.c_str());
        }
    }
    ImGui::End();
}

L1Task::~L1Task() {
    for (auto &worker: m_workers) {
        worker.command->store(Command::Terminate);
        worker.handle.join();
    }
}