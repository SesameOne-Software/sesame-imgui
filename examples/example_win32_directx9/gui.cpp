#include "gui.hpp"

float g_window_fade = 0.0f;
ImVec2 g_window_dim = ImVec2( 800, 600 );

template <typename type>
constexpr auto animate( float& t, float change_dir, const type& min, const type& max ) {
    constexpr auto animation_time = 0.200f;

    t = ImClamp( t + ImGui::GetIO( ).DeltaTime * (change_dir * (1.0f / animation_time)), 0.0f, 1.0f );
    return ImLerp( min, max, t );
}

bool ImGui::BeginMain( const char* name, bool* p_open ) {
    if ( IsKeyPressed( ImGuiKey_Insert, false ) )
        *p_open = !*p_open;

    const auto window_alpha = animate( g_window_fade, *p_open ? 1.0f : -1.0f, 0.0f, 1.0f );

    if ( !window_alpha )
        return false;

    SetNextWindowBgAlpha( window_alpha );

    const auto ret = Begin(
        name,
        p_open,
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar );



    return ret;
}