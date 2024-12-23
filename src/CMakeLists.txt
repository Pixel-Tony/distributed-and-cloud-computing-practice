find_package(OpenMP REQUIRED)

if (MSVC)
  add_compile_options(/W4 /WX)
else ()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

# Add main executable
add_executable(my-laba-collection 
  main.cpp 
  Stopwatch.cpp ColorPulse.cpp
  IRenderable.cpp Task.cpp Lab.cpp Window.cpp 
  integrate.cpp omp-utils.cpp
  labs.cpp
  lab-1/task.cpp
  lab-2/task1.cpp lab-2/task2.cpp lab-2/task3.cpp
  lab-3/task.cpp
  lab-4/task.cpp
  lab-5/task1.cpp lab-5/task2.cpp 
  lab-6/task.cpp
)
set_property(TARGET my-laba-collection PROPERTY CXX_STANDARD 23)
target_include_directories(my-laba-collection PRIVATE include)
target_compile_definitions(my-laba-collection PRIVATE -DDEBUG_ENABLE)
target_link_libraries(my-laba-collection 
  PRIVATE sfml-system sfml-window sfml-graphics ImGui-SFML::ImGui-SFML OpenMP::OpenMP_CXX implot)

# Copy shared libs to executable dir
if (WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET my-laba-collection POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:sfml-main>
      $<TARGET_FILE:sfml-system>
      $<TARGET_FILE:sfml-window>
      $<TARGET_FILE:sfml-graphics>
      $<TARGET_FILE:ImGui-SFML>
      $<TARGET_FILE_DIR:my-laba-collection>
    )
  endif ()
endif ()