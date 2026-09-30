#pragma once

#include "Engine.hxx"
#include "ur/media.hxx"

namespace ur {
namespace player {

struct Options {
    bool art = true;
    bool seek = true;
    bool transport = true;
    const char* empty = "Nothing playing";
};

void bind( CGraphics* graphics );
void draw_expanded( const Options& options = {} );
void draw_compact( const Options& options = {} );
void draw_chip( const Options& options = {} );

}
}
