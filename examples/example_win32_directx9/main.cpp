﻿// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "gui.hpp"

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D( HWND hWnd );
void CleanupDeviceD3D( );
void ResetDevice( );
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

// Main code
int main( int, char** ) {
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, _T( "ImGui Example" ), NULL };
    ::RegisterClassEx( &wc );
    HWND hwnd = ::CreateWindow( wc.lpszClassName, _T( "Dear ImGui DirectX9 Example" ), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if ( !CreateDeviceD3D( hwnd ) ) {
        CleanupDeviceD3D( );
        ::UnregisterClass( wc.lpszClassName, wc.hInstance );
        return 1;
    }

    // Show the window
    ::ShowWindow( hwnd, SW_SHOWDEFAULT );
    ::UpdateWindow( hwnd );

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); ( void )io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsSesame( );
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX9_Init( g_pd3dDevice );

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    const ImWchar custom_font_ranges [ ] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0x4e00, 0x9FAF, // CJK Ideograms
        0x3131, 0x3163, // Korean alphabets
        0xAC00, 0xD7A3, // Korean characters
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0x2010, 0x205E, // Punctuations
        0x0E00, 0x0E7F, // Thai
        // Vietnamese
        0x0102, 0x0103,
        0x0110, 0x0111,
        0x0128, 0x0129,
        0x0168, 0x0169,
        0x01A0, 0x01A1,
        0x01AF, 0x01B0,
        0x1EA0, 0x1EF9,
        0
    };

    const auto ui_font = io.Fonts->AddFontFromMemoryTTF( ( void* )sesame_ui_data, sesame_ui_size, 15.0f, nullptr, /*custom_font_ranges*/io.Fonts->GetGlyphRangesDefault( ) );
    const auto ui_small_font = io.Fonts->AddFontFromMemoryTTF( ( void* )sesame_ui_data, sesame_ui_size, 12.0f, nullptr, /*custom_font_ranges*/io.Fonts->GetGlyphRangesDefault( ) );
    const auto ui_icons_font = io.Fonts->AddFontFromMemoryTTF( ( void* )sesame_icons_data, sesame_icons_size, 28.0f, nullptr, io.Fonts->GetGlyphRangesDefault( ) );

    // Our state
    ImVec4 clear_color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );

    // Main loop
    MSG msg;
    ZeroMemory( &msg, sizeof( msg ) );

    //ImGui::GetStyle ( ).ScaleAllSizes ( 2.0f );

    while ( msg.message != WM_QUIT ) {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        static bool main_open = true;

        ImGui::PushFont( ui_font );

        /* use imgui to draw on screen directly */ {
            /* begin scene */
            ImGuiIO& io = ImGui::GetIO( );

            ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f } );
            ImGui::PushStyleColor( ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f } );
            ImGui::Begin( "##Backbuffer", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs );

            ImGui::SetWindowPos( ImVec2( 0, 0 ), ImGuiCond_Always );
            ImGui::SetWindowSize( ImVec2( io.DisplaySize.x, io.DisplaySize.y ), ImGuiCond_Always );

            const auto draw_list = ImGui::GetCurrentWindow( )->DrawList;

            /* render */
            draw_list->AddRect( ImVec2( 20.0f, 20.0f ), ImVec2( 200.0f, 200.0f ), ImGui::GetColorU32( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
            draw_list->AddText( ImVec2( 40.0f, 40.0f ), ImGui::GetColorU32( ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) ), "Test" );


            /* end scene */
            draw_list->PushClipRectFullScreen( );

            ImGui::End( );
            ImGui::PopStyleColor( );
            ImGui::PopStyleVar( 2 );
        }

        if ( ImGui::custom::Begin( "Sesame v3.2.0", &main_open, ui_small_font ) ) {
            static int cur_tab = 0;

            if ( ImGui::custom::BeginTabs( &cur_tab, ui_icons_font ) ) {
                ImGui::custom::AddTab( "A" );
                ImGui::custom::AddTab( "B" );
                ImGui::custom::AddTab( "C" );
                ImGui::custom::AddTab( "D" );
                ImGui::custom::AddTab( "E" );
                ImGui::custom::AddTab( "F" );

                ImGui::custom::EndTabs( );
            }

            ImGui::SetNextWindowPos( ImVec2( ImGui::GetWindowPos( ).x + ImGui::GetWindowSize( ).x * 0.5f, ImGui::GetWindowPos( ).y + ImGui::GetWindowSize( ).y * 0.5f ), ImGuiCond_Always, ImVec2( 0.5f, 0.5f ) );

            if ( ImGui::BeginPopupModal( "Save Config##popup", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings ) ) {
                ImGui::TextColored( ImVec4( 1.0f, 0.1f, 0.1f, 1.0f ), "There already is a config with the same name in this location.\nAre you sure you want to overwrite the config?" );

                if ( ImGui::Button( "Confirm", ImVec2( ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::GetStyle( ).FramePadding.x, 0.0f ) ) ) {
                    ImGui::CloseCurrentPopup( );
                }

                ImGui::SameLine( );

                if ( ImGui::Button( "Cancel", ImVec2( ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::GetStyle( ).FramePadding.x, 0.0f ) ) ) {
                    ImGui::CloseCurrentPopup( );
                }

                ImGui::EndPopup( );
            }

            bool open_save_modal = false;

            switch ( cur_tab ) {
                case 0: break;
                case 1: {
                    static int sub_tabs = 0;

                    if ( ImGui::custom::BeginSubtabs( &sub_tabs ) ) {
                        ImGui::custom::AddSubtab( "General", "General ragebot and accuracy settings", [ & ] ( ) {
                            static bool test_checkbox_0 = false;
                            static bool test_checkbox_1 = false;
                            static bool test_checkbox_2 = false;

                            static float test_slider_float = 0.0f;
                            static int test_slider_int = 0;

                            static int test_combobox = 0;

                            static std::vector<const char*> test_combobox_options { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten" };

                            static float test_color [ ] { 0.8f, 0.2f, 0.95f, 1.0f };

                            static int test_listbox = 0;
                            static std::vector<const char*> test_listbox_options { "One", "Two", "Three", "Four", "Five" };
                            static std::vector<const char*> hitboxes { "head", "neck","upper chest","chest", "pelvis","arms","legs", "feet" };
                            static std::vector<int> bool_hitboxes { 0, 0, 0, 0, 0, 0, 0, 0 };

                            static int test_keybind_key = 0;
                            static int test_keybind_key_mode = 0;

                            static char test_textbox [ 256 ] { '\0' };

                            /* Weapon Settings */
                            ImGui::BeginChildFrame( ImGui::GetID( "Weapon Settings" ), ImVec2( ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::GetStyle( ).FramePadding.x, 0.0f ), ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ); {
                                ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::CalcTextSize( "Weapon Settings" ).x * 0.5f );
                                ImGui::Text( "Weapon Settings" );
                                ImGui::Separator( );

                                ImGui::Checkbox( "Checkbox 1", &test_checkbox_0 );
                                ImGui::Checkbox( "Checkbox 2", &test_checkbox_1 );
                                ImGui::Checkbox( "Checkbox 3", &test_checkbox_2 );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker1", test_color );

                                ImGui::SliderFloat( "Float Slider", &test_slider_float, 0.0f, 100.0f );
                                ImGui::SliderInt( "Int Slider", &test_slider_int, 0, 20 );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker2", test_color );

                                if ( ImGui::Button( "Save Config" ) )
                                    open_save_modal = true;

                                ImGui::Button( "Load Config" );
                                ImGui::Button( "Refresh Config List" );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker3", test_color );

                                ImGui::EndChildFrame( );
                            }

                            ImGui::SameLine( );

                            /* Hitscan */
                            ImGui::BeginChildFrame( ImGui::GetID( "Hitscan" ), ImVec2( ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::GetStyle( ).FramePadding.x, 0.0f ), ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ); {
                                ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::CalcTextSize( "Hitscan" ).x * 0.5f );
                                ImGui::Text( "Hitscan" );
                                ImGui::Separator( );

                                ImGui::Combo( "Combobox", &test_combobox, test_combobox_options.data( ), test_combobox_options.size( ) );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker4", test_color );

                                ImGui::ListBox( "Listbox", &test_listbox, test_listbox_options.data( ), test_listbox_options.size( ) );

                                ImGui::MultiCombo( "Multicombo", ( bool* )bool_hitboxes.data( ), hitboxes.data( ), hitboxes.size( ) );

                                ImGui::Keybind( "Keybind", &test_keybind_key, &test_keybind_key_mode );

                                ImGui::InputText( "Textbox", test_textbox, sizeof( test_textbox ) );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker5", test_color );

                                ImGui::EndChildFrame( );
                            }
                            } );

                        ImGui::custom::AddSubtab( "Default", "Default settings used for unconfigured weapons", [ & ] ( ) {
                            static bool test_checkbox_0 = false;
                            static bool test_checkbox_1 = false;
                            static bool test_checkbox_2 = false;

                            static float test_slider_float = 0.0f;
                            static int test_slider_int = 0;

                            static int test_combobox = 0;

                            static std::vector<const char*> test_combobox_options { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten" };

                            static float test_color [ ] { 0.8f, 0.2f, 0.95f, 1.0f };

                            static int test_listbox = 0;
                            static std::vector<const char*> test_listbox_options { "One", "Two", "Three", "Four", "Five" };
                            static std::vector<const char*> hitboxes { "head", "neck","upper chest","chest", "pelvis","arms","legs", "feet" };
                            static std::vector<int> bool_hitboxes { 0, 0, 0, 0, 0, 0, 0, 0 };

                            static int test_keybind_key = 0;
                            static int test_keybind_key_mode = 0;

                            static char test_textbox [ 256 ] { '\0' };

                            /* Weapon Settings */
                            ImGui::BeginChildFrame( ImGui::GetID( "Weapon Settings" ), ImVec2( ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::GetStyle( ).FramePadding.x, 0.0f ), ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ); {
                                ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::CalcTextSize( "Weapon Settings" ).x * 0.5f );
                                ImGui::Text( "Weapon Settings" );
                                ImGui::Separator( );

                                ImGui::Checkbox( "Checkbox 1", &test_checkbox_0 );
                                ImGui::Checkbox( "Checkbox 2", &test_checkbox_1 );
                                ImGui::Checkbox( "Checkbox 3", &test_checkbox_2 );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker1", test_color );

                                ImGui::SliderFloat( "Float Slider", &test_slider_float, 0.0f, 100.0f );
                                ImGui::SliderInt( "Int Slider", &test_slider_int, 0, 20 );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker2", test_color );

                                if ( ImGui::Button( "Save Config" ) )
                                    open_save_modal = true;

                                ImGui::Button( "Load Config" );
                                ImGui::Button( "Refresh Config List" );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker3", test_color );

                                ImGui::EndChildFrame( );
                            }

                            ImGui::SameLine( );

                            /* Hitscan */
                            ImGui::BeginChildFrame( ImGui::GetID( "Hitscan" ), ImVec2( ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::GetStyle( ).FramePadding.x, 0.0f ), ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ); {
                                ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + ImGui::GetWindowContentRegionWidth( ) * 0.5f - ImGui::CalcTextSize( "Hitscan" ).x * 0.5f );
                                ImGui::Text( "Hitscan" );
                                ImGui::Separator( );

                                ImGui::Combo( "Combobox", &test_combobox, test_combobox_options.data( ), test_combobox_options.size( ) );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker4", test_color );

                                ImGui::ListBox( "Listbox", &test_listbox, test_listbox_options.data( ), test_listbox_options.size( ) );

                                ImGui::MultiCombo( "Multicombo", ( bool* )bool_hitboxes.data( ), hitboxes.data( ), hitboxes.size( ) );

                                ImGui::Keybind( "Keybind", &test_keybind_key, &test_keybind_key_mode );

                                ImGui::InputText( "Textbox", test_textbox, sizeof( test_textbox ) );

                                ImGui::SameLine( );
                                ImGui::ColorEdit4( "##Color Picker5", test_color );

                                ImGui::EndChildFrame( );
                            }
                            } );
                        ImGui::custom::AddSubtab( "Pistol", "Pistol class configuration", [ ] ( ) { } );
                        ImGui::custom::AddSubtab( "Revolver", "Revolver class configuration", [ ] ( ) { } );
                        ImGui::custom::AddSubtab( "Rifle", "Rifle, SMG, and shotgun class configuration", [ ] ( ) { } );
                        ImGui::custom::AddSubtab( "AWP", "AWP class configuration", [ ] ( ) { } );
                        ImGui::custom::AddSubtab( "Auto", "Autosniper class configuration", [ ] ( ) { } );
                        ImGui::custom::AddSubtab( "Scout", "Scout class configuration", [ ] ( ) { } );

                        ImGui::custom::EndSubtabs( );
                    }
                } break;
                case 2: break;
                case 3: break;
                case 4: break;
                case 5: break;
            }

            if ( open_save_modal )
                ImGui::OpenPopup( "Save Config##popup" );

            ImGui::custom::End( );
        }

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        if ( false && ImGui::Begin( "Sesame", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar ) ) {


            ImGui::End( );
        }

        ImGui::PopFont( );

        // Rendering
        ImGui::EndFrame( );
        g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
        g_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( ( int )( clear_color.x * 255.0f ), ( int )( clear_color.y * 255.0f ), ( int )( clear_color.z * 255.0f ), ( int )( clear_color.w * 255.0f ) );
        g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );

        if ( g_pd3dDevice->BeginScene( ) >= 0 ) {
            ImGui::Render( );
            ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
            g_pd3dDevice->EndScene( );
        }

        HRESULT result = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

        // Handle loss of D3D9 device
        if ( result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
            ResetDevice( );
    }

    ImGui_ImplDX9_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    CleanupDeviceD3D( );
    ::DestroyWindow( hwnd );
    ::UnregisterClass( wc.lpszClassName, wc.hInstance );

    return 0;
}

// Helper functions

bool CreateDeviceD3D( HWND hWnd ) {
    if ( ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
        return false;

    // Create the D3DDevice
    ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if ( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice ) < 0 )
        return false;

    return true;
}

void CleanupDeviceD3D( ) {
    if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = NULL; }
    if ( g_pD3D ) { g_pD3D->Release( ); g_pD3D = NULL; }
}

void ResetDevice( ) {
    ImGui_ImplDX9_InvalidateDeviceObjects( );
    HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
    if ( hr == D3DERR_INVALIDCALL )
        IM_ASSERT( 0 );
    ImGui_ImplDX9_CreateDeviceObjects( );
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

// Win32 message handler
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
        return true;

    switch ( msg ) {
        case WM_SIZE:
            if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED ) {
                g_d3dpp.BackBufferWidth = LOWORD( lParam );
                g_d3dpp.BackBufferHeight = HIWORD( lParam );
                ResetDevice( );
            }
            return 0;
        case WM_SYSCOMMAND:
            if ( ( wParam & 0xfff0 ) == SC_KEYMENU ) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage( 0 );
            return 0;
    }
    return ::DefWindowProc( hWnd, msg, wParam, lParam );
}
