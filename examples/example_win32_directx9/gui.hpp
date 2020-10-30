#pragma once

#include "../../imgui.h"
#include "../../backends/imgui_impl_dx9.h"
#include "../../backends/imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include "sesame_icons.hpp"
#include "sesame_ui.hpp"

#include <string_view>
#include <vector>

#include "../../imgui_internal.h"

namespace ImGui {
    bool BeginMain( const char* name, bool* p_open );
}