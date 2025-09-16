#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include <imgui.h>


struct WorkspaceProps
{
	std::string workspaceName = "WorkspaceName";
	std::vector<std::string> architecture = { "x32", "x64" };
	int current_Architecture_item = 0;
	std::string startProject = "";
	std::vector<std::string> configuration = { "Debug", "Release" };
	std::unordered_map<std::string, std::string> Dependencies;
	std::vector<std::unordered_map<std::string, std::string>> IncludeDir = {};
	bool DependenciesFolder = true;
};

struct ProjectProps
{
	WorkspaceProps* OwnerWorkspace = nullptr;
	std::string projectName = "ProjectName";
	std::string location = projectName;
	std::vector<std::string> kind = {"ConsoleApp","WindowedApp","SharedLib","StaticLib","Makefile","Utility", "None", "Packaging", "SharedItems"};
	int current_kind = 0;
	std::vector<std::string> language = {"C", "C++", "C#"};
	int current_language = 1;
	std::vector<std::string> cppdialect = { 
		"Default", "C++latest", 
		"C++98", "C++0x", "C++11", "C++1y", "C++14", "C++1z", "C++17", "C++2a","C++20", "C++2b", "C++23", 
		"gnu++98", "gnu++0x", "gnu++11", "gnu++1y", "gnu++14", "gnu++1z", "gnu++17", "gnu++2a", "gnu++20","gnu++2b", "gnu++23" };
	int current_cppdialect = 0;
	std::vector<std::string> cdialect = {
		"Default",
		"C89","C90","C99","C11","C17","C23",
		"gnu89","gnu90","gnu99","gnu11","gnu17","gnu23" };
	int current_cdialect = 0;
	std::vector<std::string> staticRuntime = {"Default", "On", "Off"};
	int current_staticRuntime = 0;
	std::vector<std::string> buildOptions = { };
};


class File
{
public:
	File();
	~File();
	void RenderUI(ImGuiID id);

	void AddWorkspace();
	void DeleteWorkspace(const WorkspaceProps& workspace);
	void AddProject();
	void DeleteProject(const ProjectProps& project);

	void GenerateFile();
private:
	std::string m_FileName = "premake5.lua";
	std::vector<WorkspaceProps> m_workspaces;
	std::vector<ProjectProps> m_projects;
};

