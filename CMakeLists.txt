cmake_minimum_required( VERSION 3.20 )

project( CustomFramework LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

if( NOT WIN32 )
    message( FATAL_ERROR "Custom Framework builds on Windows." )
endif( )

option( UR_DIRECTX11 "DirectX 11 backend" ON )
option( UR_DIRECTX12 "DirectX 12 backend" ON )
option( UR_OPENGL "OpenGL backend" ON )
option( UR_VULKAN "Vulkan backend" OFF )
option( UR_BUILD_DEMO "Build hello and showcase" ON )
option( UR_DISCORD "Discord Rich Presence" ON )
option( UR_SPOTIFY_API "Spotify Web API path" ON )

set( Engine ${CMAKE_CURRENT_SOURCE_DIR}/src/engine )

set( UrSources
    ${Engine}/Core/Arena/Arena.cxx
    ${Engine}/Core/Context/Context.cxx
    ${Engine}/Core/Input/Input.cxx
    ${Engine}/Text/Font/Font.cxx
    ${Engine}/Text/Format/Format.cxx
    ${Engine}/Text/Sheet/Sheet.cxx
    ${Engine}/Interface/Layout/Layout.cxx
    ${Engine}/Interface/Style/Style.cxx
    ${Engine}/Interface/Widgets/Widgets.cxx
    ${Engine}/Interface/Desktop/Docking/Docking.cxx
    ${Engine}/Interface/Desktop/Frames/Frames.cxx
    ${Engine}/Ports/Canvas/Canvas.cxx
    ${Engine}/Ports/Shaders/Shaders.cxx
    ${Engine}/Ports/Platform/Native/Native.cxx
    ${Engine}/Ports/Platform/Pictures/Pictures.cxx
    src/app/app.cxx
    src/app/settings.cxx
    src/app/theme.cxx
    src/audio/hear.cxx
    src/config/config.cxx
    src/discord/discord.cxx
    src/effects/effects.cxx
    src/host/ElevenHost.cxx
    src/host/TwelveHost.cxx
    src/host/OpenGLHost.cxx
    src/host/VulkanHost.cxx
    src/icons/glyphs.cxx
    src/media/media.cxx
    src/overlay/overlay.cxx
    src/ui/bind.cxx
    src/ui/debug.cxx
    src/ui/image.cxx
    src/ui/motion.cxx
    src/ui/palette.cxx
    src/ui/toast.cxx
    src/ui/view.cxx
    src/ui/widget.cxx
    src/widgets/desk.cxx
    src/widgets/orbit.cxx
    src/widgets/player.cxx
)

if( UR_DIRECTX11 )
    list( APPEND UrSources ${Engine}/Ports/Graphics/DirectX11/DirectX11.cxx )
endif( )

if( UR_DIRECTX12 )
    list( APPEND UrSources ${Engine}/Ports/Graphics/DirectX12/DirectX12.cxx )
endif( )

if( UR_OPENGL )
    list( APPEND UrSources ${Engine}/Ports/Graphics/OpenGL/OpenGL.cxx )
endif( )

if( UR_VULKAN )
    list( APPEND UrSources ${Engine}/Ports/Graphics/Vulkan/Vulkan.cxx )
endif( )

add_library( ur STATIC ${UrSources} )
add_library( UR::UR ALIAS ur )

set( UrIncludes
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/host
    ${Engine}
    ${Engine}/Core
    ${Engine}/Core/Arena
    ${Engine}/Core/Context
    ${Engine}/Core/Input
    ${Engine}/Text/Font
    ${Engine}/Text/Format
    ${Engine}/Text/Sheet
    ${Engine}/Interface/Layout
    ${Engine}/Interface/Style
    ${Engine}/Interface/Widgets
    ${Engine}/Interface/Desktop/Docking
    ${Engine}/Interface/Desktop/Frames
    ${Engine}/Ports/Canvas
    ${Engine}/Ports/Shaders
    ${Engine}/Ports/Platform/Native
    ${Engine}/Ports/Platform/Pictures
    ${Engine}/Ports/Graphics/DirectX11
    ${Engine}/Ports/Graphics/DirectX12
    ${Engine}/Ports/Graphics/OpenGL
    ${Engine}/Ports/Graphics/Vulkan
)

target_include_directories( ur PUBLIC ${UrIncludes} )

target_compile_definitions( ur PUBLIC
    UR_DISCORD=$<BOOL:${UR_DISCORD}>
    UR_SPOTIFY_API=$<BOOL:${UR_SPOTIFY_API}>
    UR_VULKAN=$<BOOL:${UR_VULKAN}>
)

if( MSVC )
    target_compile_options( ur PRIVATE /W4 /utf-8 /EHsc )
endif( )

target_link_libraries( ur PUBLIC
    user32 gdi32 ole32 uuid windowscodecs d2d1 dwrite dwmapi winmm
    runtimeobject windowsapp winhttp bcrypt ws2_32 shell32
)

if( UR_DIRECTX11 )
    target_link_libraries( ur PUBLIC d3d11 d3dcompiler dxgi )
endif( )

if( UR_DIRECTX12 )
    target_link_libraries( ur PUBLIC d3d12 d3dcompiler dxgi )
endif( )

if( UR_OPENGL )
    target_link_libraries( ur PUBLIC opengl32 )
endif( )

if( UR_VULKAN )
    find_package( Vulkan REQUIRED )
    target_link_libraries( ur PUBLIC Vulkan::Vulkan )
endif( )

function( ur_add_app Target )
    add_executable( ${Target} WIN32 ${ARGN} )
    target_link_libraries( ${Target} PRIVATE ur )
    if( MSVC )
        target_compile_options( ${Target} PRIVATE /W4 /utf-8 /EHsc )
    endif( )
    set_target_properties( ${Target} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_command( TARGET ${Target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${Target}>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/.env.example
            $<TARGET_FILE_DIR:${Target}>/.env.example
    )
endfunction( )

if( UR_BUILD_DEMO )
    ur_add_app( ur_showcase demos/showcase/Main.cxx )
    set_target_properties( ur_showcase PROPERTIES OUTPUT_NAME Showcase )

    ur_add_app( ur_hello demos/hello/Main.cxx )
    set_target_properties( ur_hello PROPERTIES OUTPUT_NAME Hello )
endif( )
