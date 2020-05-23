workspace "Breakout"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Breakout"
    location "Breakout"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c"
    }

    includedirs
    {
        "vendor/glad/include",
        "vendor/GLFW/include",
        "vendor/glm/include",
        "vendor/stb_image/include"
    }

    libdirs
    {
        "vendor/GLFW/lib"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib"
    }

    filter "configurations:Debug"
		defines "BO_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BO_RELEASE"
		optimize "On"