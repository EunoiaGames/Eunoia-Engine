#pragma once
#include "../Core/Common.h"

#ifdef _WIN32

#include "../Platform/Windows/Win32Display.h"
namespace Eunoia { namespace Rendering { typedef Win32Display Display; } }

#elif defined __APPLE__ //TODO

#elif defined __linux__ // TODO

#else
#define Display void*
#endif


