workspace "PremakeBuilder"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "PremakeBuilder/vendor/GLFW/include"
IncludeDir["glad"] = "PremakeBuilder/vendor/glad/include"
IncludeDir["ImGui"] = "PremakeBuilder/vendor/imgui"
IncludeDir["glm"] = "PremakeBuilder/vendor/glm"

group "Dependencies"
	include "PremakeBuilder/vendor/GLFW"
	include "PremakeBuilder/vendor/glad"
	include "PremakeBuilder/vendor/imgui"
group ""

project "PremakeBuilder"
	location "PremakeBuilder"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	buildoptions "/utf-8"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui}/backends",
		"%{IncludeDir.glm}"
	}
	
	links 
	{ 
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
