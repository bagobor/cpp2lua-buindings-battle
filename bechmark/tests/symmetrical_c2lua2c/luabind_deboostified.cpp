#include <nonius/nonius_single.h++>

#define LUA_COMPAT_APIINTCASTS

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <luabind/luabind.hpp>