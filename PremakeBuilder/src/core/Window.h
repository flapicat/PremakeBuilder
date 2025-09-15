#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	struct WindowData
	{
		uint32_t Width = 1024;
		uint32_t Height = 768;
		std::string Title = "Premake Builder";
		WindowData(uint32_t width, uint32_t height, const std::string& title)
			:Width(width), Height(height), Title(title)
		{
		}
	};
	Window(const WindowData& windowdata);
	~Window();
	
	void CreateNativeWindow();
	void Update();
	inline uint32_t GetWidth() const { return m_Data.Width; }
	inline uint32_t GetHeight() const { return m_Data.Height; }
	GLFWwindow* GetNativeWindow() const { return m_window; }
private:
	GLFWwindow* m_window;

	WindowData m_Data;
};

