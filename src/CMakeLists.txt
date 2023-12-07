
set(FILE_ICON logo-128.ico)
set(FILE_ICON_PNG logo-128.png)

add_executable(sprite_tool_exe
  # WIN32
  main.cpp
)
  # WindowExt.cpp Canvas.cpp App.cpp UI.cpp)

add_definitions(
  -DFILE_ICON="${FILE_ICON}"
  -DFILE_ICON_PNG="${FILE_ICON_PNG}"
  -DDEBUG_ENABLE)

if(MSVC)
  target_compile_options(sprite_tool_exe PRIVATE /W4 /WX)
else()
  target_compile_options(sprite_tool_exe PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_sources(sprite_tool_exe PRIVATE ../assets/resource.rc)

set_property(TARGET sprite_tool_exe PROPERTY CXX_STANDARD 23)

target_link_libraries(sprite_tool_exe PRIVATE stb_image ImGui-SFML::ImGui-SFML)
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET sprite_tool_exe POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-main>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE_DIR:sprite_tool_exe>)
  endif()
endif()


add_custom_command(
    TARGET sprite_tool_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/${FILE_ICON}
        $<TARGET_FILE_DIR:sprite_tool_exe>/${FILE_ICON}

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/${FILE_ICON_PNG}
        $<TARGET_FILE_DIR:sprite_tool_exe>/${FILE_ICON_PNG}
        )
