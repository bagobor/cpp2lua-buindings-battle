#pragma once

#include <nonius/nonius_single.h++>

#include <list>
#include <forward_list>

#define LUA_COMPAT_APIINTCASTS

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}