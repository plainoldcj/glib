/*
solution:	glib
project:	prenderer
file:		texallocs.h
author:		cj
*/

/*
by including this file the client registers the builtin
texture allocators automagically. the intention behind
this is to avoid an explicit call to an initialization
function RegisterAllocs(). maybe ill drop it for the
sake of clarity, but i find it very convenient.
*/

#pragma once

#include "texture.h"

R::Texture* AllocTGA(GEN::Pointer<FS::File> file);
static R::RegisterTexAlloc registerTGA("tga", AllocTGA);