cmake_minimum_required(VERSION 3.15)
project(RceditGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows subsystem (no console window)
set(CMAKE_WIN32_EXECUTABLE ON)

# ImGui sources
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

# rcedit core sources
set(RCEDIT_DIR ${CMAKE_SOURCE_DIR}/third_party/rcedit)
set(RCEDIT_SOURCES
    ${RCEDIT_DIR}/rescle.cc
)

# Application sources
set(APP_SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} WIN32
    ${APP_SOURCES}
    ${IMGUI_SOURCES}
    ${RCEDIT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${RCEDIT_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d11
    dxgi
    d3dcompiler
    dwmapi
    version
    uxtheme
)

# MSVC specific flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W3 /utf-8 /Zc:__cplusplus
    )
    # Enable DPI awareness
    target_link_options(${PROJECT_NAME} PRIVATE
        /MANIFEST:EMBED
        /MANIFESTINPUT:${CMAKE_SOURCE_DIR}/src/app.manifest
    )
endif()
