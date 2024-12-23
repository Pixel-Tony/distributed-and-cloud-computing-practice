cmake_minimum_required(VERSION 3.12)
project(pieshkov-dcc LANGUAGES CXX VERSION 0.1.0 
  DESCRIPTION "Лабораторні роботи №1-6 з дисципліни 'Розподілені та хмарні обчислення'")

set(IMGUI_SFML_IMGUI_DEMO ON)
add_subdirectory(dependencies)
add_subdirectory(src)