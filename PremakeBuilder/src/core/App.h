#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Log.h"
#include "imgui/ImGuiAPI.h"
#include "Window.h"

#include "GeneratePremakeFile.h"

class App
{
public:
	App();
	~App();

	void Start();
	void Update();
	void ShutDown();
	void OnImGuiRender();

	void Close();

	Ref<Window> GetWindow() { return m_window; };

	static App& Get() { return *s_Instance; }
private:
	bool m_Running = true;
	static App* s_Instance;
	Ref<Window> m_window;
	ImGuiAPI* m_ImGuiAPI;
	File m_file;
};

