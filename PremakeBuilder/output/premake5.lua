workspace "WorkspaceName"
	architecture "x32"

	configurations
	{
		"Debug"
		"Release"
	}


group "Dependencies"
group ""
project "ProjectName"
	location "ProjectName"
	kind "ConsoleApp"
	language "C++"
	cppdialect "Default"
	staticruntime "Default"
