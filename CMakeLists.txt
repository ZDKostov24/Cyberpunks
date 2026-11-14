cmake_minimum_required(VERSION 3.14)
project(EscapeTheMainframe LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
#  raylib: downloaded automatically from GitHub the first time you build
# -----------------------------------------------------------------------------
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

# -----------------------------------------------------------------------------
#  Game logic (no input/output) - shared by both versions and the tests
# -----------------------------------------------------------------------------
add_library(maze_core STATIC src/core/maze.cpp src/core/game.cpp)
target_include_directories(maze_core PUBLIC src/core)

# Graphical version (raylib) - the main game
add_executable(escape_the_mainframe
    src/graphics/main.cpp
    src/graphics/menu_screen.cpp
    src/graphics/size_screen.cpp
    src/graphics/info_screens.cpp
    src/graphics/game_screen.cpp
    src/graphics/maze_renderer.cpp
    src/graphics/widgets.cpp
    src/graphics/fonts.cpp
    src/graphics/particles.cpp
    src/graphics/code_rain.cpp
)
target_include_directories(escape_the_mainframe PRIVATE src/graphics)
target_link_libraries(escape_the_mainframe PRIVATE maze_core raylib)

# Copy the fonts (assets folder) next to the .exe after every build
add_custom_command(TARGET escape_the_mainframe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:escape_the_mainframe>/assets
)

# Text version (console) - draws the maze with characters
add_executable(escape_the_mainframe_console
    src/console/console_main.cpp
    src/console/console_ui.cpp
)
target_link_libraries(escape_the_mainframe_console PRIVATE maze_core)

# QA tests
add_executable(maze_tests test/tests.cpp)
target_link_libraries(maze_tests PRIVATE maze_core)

# Strict warnings for our own code - the project must compile without any
foreach(target maze_core escape_the_mainframe escape_the_mainframe_console maze_tests)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
    endif()
endforeach()

# On Windows, the graphical game opens without an extra console window
if(WIN32 AND MSVC)
    set_target_properties(escape_the_mainframe PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()
