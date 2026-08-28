#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "ur/overlay.hpp"

#include <Windows.h>
#include <dwmapi.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

namespace ur {
namespace overlay {

static Options Last{ };
static HWND LastHandle = nullptr;
static bool HaveLast = false;

void apply( void* Window, const Options& Options ) {
    HWND Handle = ( HWND )Window;
    if ( !Handle )
        return;

    if ( HaveLast && LastHandle == Handle
        && Last.topmost == Options.topmost
        && Last.click_through == Options.click_through
        && Last.layered == Options.layered
        && Last.alpha == Options.alpha )
        return;

    Last = Options;
    LastHandle = Handle;
    HaveLast = true;

    LONG Extra = GetWindowLongW( Handle, GWL_EXSTYLE );

    if ( Options.layered || Options.click_through || Options.alpha < 255 )
        Extra |= WS_EX_LAYERED;
    else
        Extra &= ~WS_EX_LAYERED;

    if ( Options.click_through )
        Extra |= WS_EX_TRANSPARENT;
    else
        Extra &= ~WS_EX_TRANSPARENT;

    SetWindowLongW( Handle, GWL_EXSTYLE, Extra );

    if ( Extra & WS_EX_LAYERED ) {
        int Alpha = Options.alpha;
        if ( Alpha < 0 )
            Alpha = 0;
        if ( Alpha > 255 )
            Alpha = 255;
        SetLayeredWindowAttributes( Handle, 0, ( BYTE )Alpha, LWA_ALPHA );
    }

    SetWindowPos( Handle, Options.topmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED );
}

void attach( void* Window ) {
    HWND Handle = ( HWND )Window;
    if ( !Handle )
        return;

    HaveLast = false;
    LastHandle = nullptr;

    BOOL Dark = TRUE;
    if ( FAILED( DwmSetWindowAttribute( Handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &Dark, sizeof( Dark ) ) ) )
        DwmSetWindowAttribute( Handle, 19, &Dark, sizeof( Dark ) );
}

}
}
