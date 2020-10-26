@echo off
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
) else (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    ) else (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
    )
)

set include_dir="%DXSDK_DIR%Include"
set compilerflags=/Od /Zi /EHsc /std:c++latest /I %include_dir% /OPT:REF /INCREMENTAL
set libs=kernel32.lib user32.lib d3d9.lib d3dx9.lib
set library_dir="%DXSDK_DIR%Lib/x64"
set linkerflags=/OUT:examples/example_win32_directx9/bin/main.exe /LIBPATH:%library_dir% /OPT:REF /INCREMENTAL
set to_compile=imgui_widgets.cpp imgui_draw.cpp imgui.cpp backends/imgui_impl_dx9.cpp backends/imgui_impl_win32.cpp examples/example_win32_directx9/*.cpp

set out_str=%compilerflags% %to_compile% /link %libs% %linkerflags%

cl.exe %out_str%