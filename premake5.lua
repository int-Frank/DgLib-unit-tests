workspace "DgLib-tests"
  location ""
  architecture "x64"
  
  startproject "tests"
  
  configurations
  {
    "Debug",
    "Release"
  }
  
projOutput = "%{wks.location}/build/%{prj.name}-%{cfg.buildcfg}"
projOutputInt = "%{wks.location}/build/intermediate/%{prj.name}-%{cfg.buildcfg}"

project "Tests"
  location "Tests"
  kind "ConsoleApp"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  files 
  {
    "Tests/src/**.h",
    "Tests/src/**.cpp",
  }

  includedirs
  {
    "%{wks.location}/DgLib-tests/DgLib/DgLib/src",
    "%{wks.location}/DgLib-tests/cppunitlite/cppunitlite"
  }
  
  links
  {
    "DgLib",
    "cppunitlite"
  }

project "DgLib"
  location "DgLib"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "DgLib/src/**.h",
    "DgLib/src/**.cpp",
  }
  
  include "./DgLib_vpaths.lua"
  
project "cppunitlite"
  location "cppunitlite"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  
  include "cppunitlite/premake5_cppunitlite.lua"