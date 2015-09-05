-- premake5.lua

local BUILD_DIR = (".build/" .. _ACTION)

workspace "cpp2lua-bechmarks"
   configurations { "Debug", "Release" }
   location (BUILD_DIR)
   -- platforms { "x32", "x64" }

   -- filter { "platforms:x32" }
   --    architecture "x32"

   -- filter { "platforms:x64" }
   --    architecture "x64"

   filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

-- filter "action:vs2010"
-- filter "action:vs*"
--   defines { "VISUAL_STUDIO_2005" }      

-- filter { "Debug", "kind:SharedLib or StaticLib" }
--   targetsuffix "_d"

-- filter "system:not windows"
--   defines { "NOT_WINDOWS" }

-- filter "files:**.png"
--   buildaction "Embed" -- Embed the file into the target binary as a resource.

project "liblua"
   kind "StaticLib"
   location (BUILD_DIR .. "/lua")
   language "C"
   targetdir (BUILD_DIR .. "/lib/%{cfg.buildcfg}")

   includedirs  {"./external/lua/src/"}
--libdirs { "../lua/libs", "../zlib" }   

   files { "./external/lua/src/**.c", "./external/lua/src/**.h" }
   removefiles {"**luac.c", "**lua.c"}

project "lua"   
   kind "ConsoleApp"
   location (BUILD_DIR .. "/lua")
   language "C"
   targetdir (BUILD_DIR .. "/%{cfg.buildcfg}")
   includedirs  {"./external/lua/src/"}
   files {"./external/lua/src/lua.c"}
   libdirs { BUILD_DIR .. "/lib/%{cfg.buildcfg}" }
   links { "liblua" }

project "luac"   
   kind "ConsoleApp"
   location (BUILD_DIR .. "/lua")
   language "C"
   targetdir (BUILD_DIR .. "/%{cfg.buildcfg}")
   includedirs  {"./external/lua/src/"}
   files {"./external/lua/src/luac.c"}
   libdirs { BUILD_DIR .. "/lib/%{cfg.buildcfg}" }
   links { "liblua" }

project "bechmark"
   kind "ConsoleApp"
   location (BUILD_DIR .. "/bechmark")
   language "C++"
   targetdir (BUILD_DIR .. "/%{cfg.buildcfg}")

   defines { "LUAPP_HEADER_ONLY" }   

   includedirs  {
      "./external/lua-api-pp/",
      "./external/luacxx/src/",
      "./external/lua-intf/LuaIntf/",
      "./external/luapath/include/",
      "./external/LuaState/include/",
      "./external/luawrapper/include/",
      "./external/Selene/include/",
      "./external/sol/",
      "./external/lua/src/",
      "./external/boost/", 
      "./bechmark/nonius/include/"
   }
   
   libdirs { BUILD_DIR .. "/lib/%{cfg.buildcfg}" }   

   links { "liblua" }

   files { "./bechmark/**.h", "./bechmark/**.h++", "./bechmark/**.cpp" }

