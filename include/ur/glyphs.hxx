#pragma once

#include "Engine.hxx"
#include "ur/icons.hxx"

namespace ur {
namespace glyphs {

enum class Weight {
    Solid,
    Regular,
    Light
};

void bind( CGraphics* graphics );
unsigned long long image( icons::Icon icon, int size, Weight weight = Weight::Solid );
void sweep( );

}
}
