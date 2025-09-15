#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct WorkspaceProps
{
	std::string workspaceName = "Workspace";
	std::vector<std::string> architecture = {"x32", "x64"};
	std::string startProject = "";
	std::vector<std::string> configuration = {"Debug", "Release"};
	std::unordered_map<std::string, std::string> Dependencies;
	std::vector<std::unordered_map<std::string, std::string>> IncludeDir = {};
	bool DependenciesFolder = true;
};

struct ProjectProps
{
	std::string projectName = "Project";
	std::string location = projectName;
	std::vector<std::string> kind = {"ConsoleApp", "WindowedApp", "SharedLib","StaticLib","Makefile","Utility", "None", "Packaging", "SharedItems"};
	std::vector<std::string> language = {"C", "C++", "C#"};
	std::vector<std::string> cppdialect = { 
		"Default", "C++latest", 
		"C++98", "C++0x", "C++11", "C++1y", "C++14", "C++1z", "C++17", "C++2a","C++20", "C++2b", "C++23", 
		"gnu++98", "gnu++0x", "gnu++11", "gnu++1y", "gnu++14", "gnu++1z", "gnu++17", "gnu++2a", "gnu++20","gnu++2b", "gnu++23" };
	std::vector<std::string> cdialect = {
		"Default",
		"C89","C90","C99","C11","C17","C23",
		"gnu89","gnu90","gnu99","gnu11","gnu17","gnu23" };
	std::vector<std::string> staticRuntime = {"Default", "On", "Off"};
	std::vector<std::string> buildOptions = { };
};

class File
{
public:
	File();
	~File();
	void RenderUI();

	void GenerateFile();
private:
	std::vector<WorkspaceProps> workspaceses;
	std::vector<ProjectProps> projects;
};

