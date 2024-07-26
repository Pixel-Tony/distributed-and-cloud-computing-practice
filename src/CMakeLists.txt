
set(APP_ICON_ICO logo-128.ico)
set(APP_ICON_PNG logo-128.png)

add_executable(sprite_tool_exe
#   WIN32
  main.cpp
  Frontend/WindowExt.cpp Frontend/Canvas.cpp Frontend/UINode.cpp 
  App.cpp
)
set_property(TARGET sprite_tool_exe PROPERTY CXX_STANDARD 23)

add_definitions(
  -DAPP_ICON_ICO="${APP_ICON_ICO}"
  -DAPP_ICON_PNG="${APP_ICON_PNG}"
  -DDEBUG_ENABLE
)

if(MSVC)
  target_compile_options(sprite_tool_exe PRIVATE /W4 /WX)
else()
  target_compile_options(sprite_tool_exe PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_sources(sprite_tool_exe PRIVATE ../assets/resource.res)

target_link_libraries(sprite_tool_exe PRIVATE stb_image sfml-system sfml-window sfml-graphics)
target_link_libraries(sprite_tool_exe PUBLIC ctre)

if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET sprite_tool_exe POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:sfml-main>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE_DIR:sprite_tool_exe>
    )
  endif()
endif()


add_custom_command(
  TARGET sprite_tool_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/assets/${APP_ICON_ICO}
    $<TARGET_FILE_DIR:sprite_tool_exe>/${APP_ICON_ICO}

  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/assets/${APP_ICON_PNG}
    $<TARGET_FILE_DIR:sprite_tool_exe>/${APP_ICON_PNG}
)