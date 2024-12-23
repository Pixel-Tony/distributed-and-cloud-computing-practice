#include "labs.hpp"
#include "tasks.hpp"

Lab1::Lab1() : Lab("Lab 1", {std::make_shared<L1Task>()}){}

Lab2::Lab2() : Lab("Lab 2", {std::make_shared<L2Task1>(), std::make_shared<L2Task2>(), std::make_shared<L2Task3>()}) {}

Lab3::Lab3() : Lab("Lab 3", {std::make_shared<L3Task>()}) {}

Lab4::Lab4() : Lab("Lab 4", {std::make_shared<L4Task>()}) {}

Lab5::Lab5() : Lab("Lab 5", {std::make_shared<L5Task1>(), std::make_shared<L5Task2>()}) {}

Lab6::Lab6() : Lab("Lab 6", {std::make_shared<L6Task>()}) {}