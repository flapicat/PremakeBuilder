#include "Window.h"
#include "App.h"

#include "Log.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void WindowCloseCallback(GLFWwindow* window) {
    App::Get().Close();
}

Window::Window(const WindowData& windowdata)
	:m_Data(windowdata)
{
	CreateNativeWindow();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::CreateNativeWindow()
{
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    m_window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str() , nullptr, nullptr);
    if (!m_window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetWindowCloseCallback(m_window, WindowCloseCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
        exit(-1);
    }

    LOG_INFO("GLFW and GLAD initialized successfully");
}

void Window::Update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
