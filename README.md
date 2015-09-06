#Benchmarks for Lua_to_c++ bindings:

Notes:
- Selene, https://github.com/jeremyong/Selene, header oonly
- lua-intf, https://github.com/SteveKChiu/lua-intf header only is an option
- Luabind, 
- Luabind-deboostyfied, 
- luacxx, https://github.com/dafrito/luacxx
- LuaState, https://github.com/AdUki/LuaState
- luapath, https://github.com/guwere/luapath
- luawrapper, https://github.com/tomaka/luawrapper
- ~~sol, https://github.com/Rapptz/sol build failed under Visual Studio 2015 (not supported?)~~
- ~~luapp, https://github.com/OldFisher/lua-api-pp - build failed under Visual Studio 2015~~
- ~~LuaContext - build failed under Visual Studio 2015 It uses boost and looks like it conflicts with latest version.~~
 
##[**Results:(draft)**](https://raw.githubusercontent.com/bagobor/cpp2lua-buindings-battle/master/results/cfunction_from_lua.html)
![c_function_from_lua](https://raw.githubusercontent.com/bagobor/cpp2lua-buindings-battle/master/results/cfunction_from_lua.png)

Status:
- [x] Initial project structure and build setup
- [x] Create simple benchmark
- [x] Obtain fist result
- [ ] Do the rest :)




