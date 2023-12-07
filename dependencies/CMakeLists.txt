set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

set(IMGUI_DIR ./../imgui)
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

add_subdirectory(sfml)
add_subdirectory(stb-image)