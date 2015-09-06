# cpp2lua-buindings-battle
Testbed for Lua2C++ bindings: Luabind, Luabind-deboostyfied, Selene, luacxx, lua-intf, LuaState, sol, luapath, lua-api-pp

current state:
- sol: latest version use exceptions and don't support Visual Studio 2015
- LuaContext - won't compole. It uses boost and looks like it conflicts with latest version.
- luapp - build failed under Visual Studio 2015
