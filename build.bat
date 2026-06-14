@echo off
setlocal

set CC=clang++
set CFLAGS=-std=c++17 -O2 -fms-extensions -DUNICODE -D_UNICODE -DWIN32 -D_WINDOWS -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 -D_CRT_SECURE_NO_WARNINGS -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_SILENCE_LOCALE_EMPTY_DEPRECATION_WARNING -w
set INCLUDES=-I third_party/imgui -I third_party/imgui/backends -I third_party/rcedit -I src
set LIBS=-ld3d11 -ldxgi -ld3dcompiler -ldwmapi -lversion -luxtheme -lcomdlg32 -lshell32 -lole32 -lgdi32 -luser32 -lkernel32

set IMGUI_SRC=third_party/imgui/imgui.cpp third_party/imgui/imgui_draw.cpp third_party/imgui/imgui_tables.cpp third_party/imgui/imgui_widgets.cpp third_party/imgui/imgui_demo.cpp
set IMGUI_BACKEND=third_party/imgui/backends/imgui_impl_win32.cpp third_party/imgui/backends/imgui_impl_dx11.cpp
set RCEDIT_SRC=third_party/rcedit/rescle.cc
set APP_SRC=src/main.cpp

echo Building RceditGUI...
%CC% %CFLAGS% %INCLUDES% -o RceditGUI.exe %APP_SRC% %IMGUI_SRC% %IMGUI_BACKEND% %RCEDIT_SRC% %LIBS% -Wl,/SUBSYSTEM:WINDOWS

if %ERRORLEVEL% EQU 0 (
    echo Build successful: RceditGUI.exe
) else (
    echo Build failed!
    exit /b 1
)
