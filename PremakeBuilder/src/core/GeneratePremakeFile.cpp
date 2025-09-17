#include "GeneratePremakeFile.h"
#include <filesystem>
#include "Log.h"

namespace fs = std::filesystem;

File::File()
{
}

File::~File()
{
}

void File::AddWorkspace()
{
	WorkspaceProps newWorkspace;
	m_workspaces.emplace_back(newWorkspace);
}

void File::DeleteWorkspace(const WorkspaceProps& workspace)
{
	auto it = std::find_if(m_workspaces.begin(), m_workspaces.end(),
		[&workspace](const WorkspaceProps& w) {
			return w.workspaceName == workspace.workspaceName; 
		});

	if (it != m_workspaces.end())
		m_workspaces.erase(it);
}

void File::AddProject()
{
	ProjectProps newProjects;
	m_projects.emplace_back(newProjects);
}

void File::DeleteProject(const ProjectProps& project)
{
	auto it = std::find_if(m_projects.begin(), m_projects.end(),
		[&project](const ProjectProps& p) {
			return p.projectName == project.projectName;
		});

	if (it != m_projects.end())
		m_projects.erase(it);
}

void File::GenerateFile()
{
	//Creating file
	std::string outputFolderName = "output";
	fs::create_directory(outputFolderName);
	std::string fileName = outputFolderName + "/" + m_FileName;
	std::ofstream premakeFile(fileName);

	if (!premakeFile){
		LOG_ERROR("Could open/create file");
		exit(-1);
	}


	for (int i = 0; i < m_workspaces.size(); i++)
	{
		auto& currentWorkspace = m_workspaces[i];
		premakeFile << "workspace " << "\"" << currentWorkspace.workspaceName << "\"\n";
		premakeFile << "\tarchitecture " << "\"" << currentWorkspace.architecture[currentWorkspace.current_Architecture_item] << "\"\n";
		if (!currentWorkspace.startProject.empty())
		{
			premakeFile << "\tstartproject" << "\"" << currentWorkspace.startProject << "\"\n";
		}
		premakeFile << "\n";
		premakeFile << "\tconfigurations\n\t{\n";
		for (size_t i = 0; i < currentWorkspace.configurations.size(); i++)
		{
			premakeFile << "\t\t\"" << currentWorkspace.configurations[i] << "\"\n";
		}
		premakeFile << "\t}\n";
		premakeFile << "\n";
		premakeFile << "\n";
		if (currentWorkspace.DependenciesFolder)
		{
			premakeFile << "group \"Dependencies\"\n";
			premakeFile << "group \"\"";
		}
		premakeFile << "\n\n";

		for (auto& project : m_projects)
		{
			if (project.OwnerWorkspace->workspaceName == currentWorkspace.workspaceName)
			{
				premakeFile << "project \"" << project.projectName << "\"\n";
				premakeFile << "\tlocation \"" << project.location << "\"\n";
				premakeFile << "\tkind \"" << project.kind[project.current_kind] << "\"\n";
				premakeFile << "\tlanguage \"" << project.language[project.current_language] << "\"\n";
				if (project.current_language == 0)
				{
					premakeFile << "\tcdialect \"" << project.cdialect[project.current_cdialect] << "\"\n";
				}
				if (project.current_language == 1)
				{
					premakeFile << "\tcppdialect \"" << project.cppdialect[project.current_cppdialect] << "\"\n";
				}
				premakeFile << "\tstaticruntime \"" << project.staticRuntime[project.current_staticRuntime] << "\"\n";
				if (project.buildOptions.size() > 0)
				{
					premakeFile << "\tbuildoptions {";
					for (size_t i = 0; i < project.buildOptions.size(); i++)
					{
						premakeFile << project.buildOptions[i] << ",";
					}
					premakeFile << "}\n";
				}
			}
		}
	}

	premakeFile.close();
	LOG_INFO("File generated Successfully");
}

void File::RenderUI(ImGuiID dockspaceID)
{
	ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);

	// --------------- workspaces -----------------
	{
		for (size_t i = 0; i < m_workspaces.size(); i++)
		{
			auto& currentWorkspace = m_workspaces[i];
			std::string windowID = (currentWorkspace.workspaceName.empty() ? "Workspace" : currentWorkspace.workspaceName) + "###" + std::to_string(i);
			ImGui::Begin(windowID.c_str());
			// --------------- workspace name -----------------
			{
				char buffer[128];
				strncpy(buffer, currentWorkspace.workspaceName.c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';

				ImGui::Text("Workspace Name:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(200);
				if (ImGui::InputText("##workspaceName", buffer, sizeof(buffer)))
					currentWorkspace.workspaceName = buffer;
			}

			// ------------ startup project name --------------
			{
				char buffer[128];
				strncpy(buffer, currentWorkspace.startProject.c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';
				ImGui::Text("Startup Project Name:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(200);
				if (ImGui::InputText("##StartupProject", buffer, sizeof(buffer)))
					currentWorkspace.startProject = buffer;
			}

			// ------------- Architecture combo ---------------
			{
				ImGui::Text("Select Architecture:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);

				if (!currentWorkspace.architecture.empty())
				{
					if (ImGui::BeginCombo("##architecture", currentWorkspace.architecture[currentWorkspace.current_Architecture_item].c_str()))
					{
						for (int n = 0; n < currentWorkspace.architecture.size(); n++)
						{
							bool is_selected = (currentWorkspace.current_Architecture_item == n);
							if (ImGui::Selectable(currentWorkspace.architecture[n].c_str(), is_selected))
								currentWorkspace.current_Architecture_item = n;

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}
			}

			// --------------- Configurations -----------------
			{
				ImGui::Text("Configurations:");
				ImGui::SameLine();
				std::string buttonLabel = "Manage##" + std::to_string(i);
				if (ImGui::Button(buttonLabel.c_str()))
				{
					ImGui::OpenPopup(("Manage Configurations###popup" + std::to_string(i)).c_str());
				}

				// ---------------- Modal Popup -----------------
				std::string ManagepopupID = "Manage Configurations###popup" + std::to_string(i);
				if (ImGui::BeginPopupModal(ManagepopupID.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					for (size_t c = 0; c < currentWorkspace.configurations.size(); c++)
					{
						ImGui::PushID(static_cast<int>(c));
						ImGui::Text("%s", currentWorkspace.configurations[c].c_str());
						ImGui::SameLine();
						if (ImGui::Button("Delete"))
						{
							currentWorkspace.configurations.erase(currentWorkspace.configurations.begin() + c);
							ImGui::PopID();
							break;
						}
						ImGui::PopID();
					}

					// Input to add new configuration
					static char newConfig[128] = "";
					ImGui::InputText("New Config", newConfig, sizeof(newConfig));
					ImGui::SameLine();
					if (ImGui::Button("Add"))
					{
						if (strlen(newConfig) > 0)
						{
							currentWorkspace.configurations.push_back(std::string(newConfig));
							newConfig[0] = '\0';
						}
					}

					ImGui::Separator();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			// ------------ dependencies checkbox -------------
			{
				ImGui::Text("Create dependencies folder:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(200);
				ImGui::Checkbox("##dependenciesFolder", &currentWorkspace.DependenciesFolder);
			}

			// -------------------- Buttons -------------------
			{
				// ---------------- Delete Workspace Button ----------------
				ImVec2 btnSize(100, 30);
				ImVec2 padding(10, 10);

				ImVec2 winPos = ImGui::GetWindowPos();
				ImVec2 winSize = ImGui::GetWindowSize();

				ImVec2 btnPos(winSize.x - btnSize.x - padding.x, winSize.y - btnSize.y - padding.y);

				ImGui::SetCursorPos(btnPos);

				// Red button
				ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.6f, 0.0f, 0.0f, 1.0f });

				if (ImGui::Button("Delete", btnSize))
				{
					ImGui::OpenPopup(("DeleteWorkspace###deletePopup" + std::to_string(i)).c_str());
				}

				ImGui::PopStyleColor(3);

				// Delete Workspace Popup
				std::string deletePopupID = "DeleteWorkspace###deletePopup" + std::to_string(i);
				if (ImGui::BeginPopupModal(deletePopupID.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Are you sure you want to delete this workspace?\nThis action will be permanent!");
					ImGui::Spacing();

					ImVec2 availPopup = ImGui::GetContentRegionAvail();
					ImGui::SetCursorPosX(availPopup.x - 240); // two buttons 120px each
					if (ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
					ImGui::SameLine();
					if (ImGui::Button("Continue", ImVec2(120, 0)))
					{
						DeleteWorkspace(currentWorkspace);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}
	}

	// --------------- Projects -----------------
	{
		for (size_t i = 0; i < m_projects.size(); i++)
		{
			auto& currentProject = m_projects[i];
			std::string windowID = (currentProject.projectName.empty() ? "Project" : currentProject.projectName) + "###" + std::to_string(i + m_workspaces.size());
			ImGui::Begin(windowID.c_str());

			// --------------- workspace owner ----------------
			{
				ImGui::Text("Workspace:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);

				std::string workspaceName = "None";
				if (m_workspaces.size() > 0)
				{
					currentProject.OwnerWorkspace = &m_workspaces[0];
					workspaceName = m_workspaces[0].workspaceName;
				}
				else
				{
					workspaceName = "None";
				}
				if (currentProject.OwnerWorkspace != nullptr)
				{
					if (ImGui::BeginCombo(("##workspace" + std::to_string(i)).c_str(), workspaceName.c_str()))
					{
						for (size_t w = 0; w < m_workspaces.size(); w++)
						{
							WorkspaceProps* ws = &m_workspaces[w];
							bool is_selected = (currentProject.OwnerWorkspace == ws); // compare pointers

							if (ImGui::Selectable(ws->workspaceName.c_str(), is_selected))
								currentProject.OwnerWorkspace = ws;

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}
				}
				else
				{
					if (ImGui::BeginCombo(("##workspace" + std::to_string(i)).c_str(), workspaceName.c_str()))
					{
						ImGui::EndCombo();
					}
				}
			}

			// --------------- workspace name -----------------
			{
				char buffer[128];
				strncpy(buffer, currentProject.projectName.c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';

				ImGui::Text("Project Name:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(200);
				if (ImGui::InputText("##projectName", buffer, sizeof(buffer)))
					currentProject.projectName = buffer;
			}

			// ------------------- Location -------------------
			{
				char buffer[128];
				strncpy(buffer, currentProject.location.c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';
				ImGui::Text("Location:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(200);
				if (ImGui::InputText("##location", buffer, sizeof(buffer)))
					currentProject.location = buffer;
			}

			// ------------------ Kind Combo ------------------
			{
				ImGui::Text("Kind:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				if (ImGui::BeginCombo(("##kind" + std::to_string(i)).c_str(), currentProject.kind[currentProject.current_kind].c_str()))
				{
					for (int i = 0; i < currentProject.kind.size(); i++)
					{
						bool is_selected = (currentProject.current_kind == i);
						if (ImGui::Selectable(currentProject.kind[i].c_str(), is_selected))
							currentProject.current_kind = i;
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			// ---------------- Language Combo ----------------
			{
				ImGui::Text("Language:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::BeginCombo(("##language" + std::to_string(i)).c_str(), currentProject.language[currentProject.current_language].c_str()))
				{
					for (int i = 0; i < currentProject.language.size(); i++)
					{
						bool is_selected = (currentProject.current_language == i);
						if (ImGui::Selectable(currentProject.language[i].c_str(), is_selected))
							currentProject.current_language = i;
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			// -------------- C++ Dialect Combo ---------------
			{
				if (currentProject.current_language == 1)
				{
					ImGui::Text("C++ Dialect:");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(200);
					if (ImGui::BeginCombo(("##cppdialect" + std::to_string(i)).c_str(), currentProject.cppdialect[currentProject.current_cppdialect].c_str()))
					{
						for (int i = 0; i < currentProject.cppdialect.size(); i++)
						{
							bool is_selected = (currentProject.current_cppdialect == i);
							if (ImGui::Selectable(currentProject.cppdialect[i].c_str(), is_selected))
								currentProject.current_cppdialect = i;
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}
			}

			// --------------- C Dialect Combo ----------------
			{
				if (currentProject.current_language == 0)
				{
					ImGui::Text("C Dialect:");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(200);
					if (ImGui::BeginCombo(("##cdialect" + std::to_string(i)).c_str(), currentProject.cdialect[currentProject.current_cdialect].c_str()))
					{
						for (int i = 0; i < currentProject.cdialect.size(); i++)
						{
							bool is_selected = (currentProject.current_cdialect == i);
							if (ImGui::Selectable(currentProject.cdialect[i].c_str(), is_selected))
								currentProject.current_cdialect = i;
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}
			}

			// ---------------- Static Runtime ----------------
			{
				ImGui::Text("Static Runtime:");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::BeginCombo(("##staticRuntime" + std::to_string(i)).c_str(), currentProject.staticRuntime[currentProject.current_staticRuntime].c_str()))
				{
					for (int i = 0; i < currentProject.staticRuntime.size(); i++)
					{
						bool is_selected = (currentProject.current_staticRuntime == i);
						if (ImGui::Selectable(currentProject.staticRuntime[i].c_str(), is_selected))
							currentProject.current_staticRuntime = i;
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			// ----------------- Build Options ----------------
			
			{
				ImGui::Text("Build options:");
				ImGui::SameLine();
				std::string buttonLabel = "Manage##" + std::to_string(i);
				if (ImGui::Button(buttonLabel.c_str()))
				{
					ImGui::OpenPopup(("Manage BuildOptions###popup" + std::to_string(i)).c_str());
				}

				// ---------------- Modal Popup -----------------
				std::string ManagepopupID = "Manage BuildOptions###popup" + std::to_string(i);
				if (ImGui::BeginPopupModal(ManagepopupID.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					for (size_t c = 0; c < currentProject.buildOptions.size(); c++)
					{
						ImGui::PushID(static_cast<int>(c));
						ImGui::Text("%s", currentProject.buildOptions[c].c_str());
						ImGui::SameLine();
						if (ImGui::Button("Delete"))
						{
							currentProject.buildOptions.erase(currentProject.buildOptions.begin() + c);
							ImGui::PopID();
							break;
						}
						ImGui::PopID();
					}

					// Input to add new configuration
					static char newOption[128] = "";
					ImGui::InputText("New Option", newOption, sizeof(newOption));
					ImGui::SameLine();
					if (ImGui::Button("Add"))
					{
						if (strlen(newOption) > 0)
						{
							currentProject.buildOptions.emplace_back(std::string(newOption));
							newOption[0] = '\0';
						}
					}

					ImGui::Separator();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			// -------------------- Buttons -------------------
			{
				// ---------------- Delete Project Button ----------------
				ImVec2 btnSize(100, 30);
				ImVec2 padding(10, 10);

				ImVec2 winPos = ImGui::GetWindowPos();
				ImVec2 winSize = ImGui::GetWindowSize();

				ImVec2 btnPos(winSize.x - btnSize.x - padding.x, winSize.y - btnSize.y - padding.y);

				ImGui::SetCursorPos(btnPos);

				// Red button
				ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 0.0f, 0.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.6f, 0.0f, 0.0f, 1.0f });

				if (ImGui::Button("Delete", btnSize))
				{
					ImGui::OpenPopup(("DeleteProject###deletePopup" + std::to_string(i)).c_str());
				}

				ImGui::PopStyleColor(3);

				// Delete Project Popup
				std::string deletePopupID = "DeleteProject###deletePopup" + std::to_string(i);
				if (ImGui::BeginPopupModal(deletePopupID.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Are you sure you want to delete this project?\nThis action will be permanent!");
					ImGui::Spacing();

					ImVec2 availPopup = ImGui::GetContentRegionAvail();
					ImGui::SetCursorPosX(availPopup.x - 240); // two buttons 120px each
					if (ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
					ImGui::SameLine();
					if (ImGui::Button("Continue", ImVec2(120, 0)))
					{
						DeleteProject(currentProject);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}
	}

}
