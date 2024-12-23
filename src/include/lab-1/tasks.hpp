#ifndef PIESHKOV_DCC_L1_TASKS_HPP
#define PIESHKOV_DCC_L1_TASKS_HPP

#include "Task.hpp"
#include <imgui.h>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

enum class Command {
    Empty, ClaimFile, WriteToFile, OutputFile, Terminate
};

enum class State {
    Idle, Waiting, ClaimedFile, WritingToFile, OutputtingFile
};

template<class T> 
using SharedResource = std::shared_ptr<std::atomic<T>>;

template<class T>
inline static SharedResource<T> makeResource(T v) { return std::make_shared<std::atomic<T>>(v); }

struct Worker {

    SharedResource<bool> isCompleted;
    SharedResource<Command> command;
    SharedResource<State> state;
    std::thread handle;

    template<class T>
    explicit Worker(T work)
        :isCompleted(makeResource(false)),
         command(makeResource(Command::Empty)),
         state(makeResource(State::Idle)),
         handle(std::thread(work, isCompleted, command, state)) {}

    Worker(Worker &) = delete;
    Worker(Worker &&) = default;
    Worker &operator=(Worker &) = delete;
    Worker &operator=(Worker &&) = default;
    ~Worker() = default;
};

class L1Task final : public Task {
    std::vector<Worker> m_workers{};
    void dropThreadAt(int i);
public:
    L1Task();
    void renderWindow() override final;
    ~L1Task();
};

#endif //PIESHKOV_DCC_L1_TASKS_HPP
