workspace "cpplox"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "build"
    startproject "cpplox"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "cpplox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}"
    location "%{wks.location}/build/%{prj.name}"

    files {"%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp"}

    includedirs {"%{prj.name}/src"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


-- ExprNode(  TermNode(    FactorNode(Number: NUMBER 5 5.000000)     TermPrimeNode(STAR *      FactorNode(Number: NUMBER 4 4.000000)     TermPrimeNode(STAR *      FactorNode(Number: NUMBER 3 3.000000) ))) )