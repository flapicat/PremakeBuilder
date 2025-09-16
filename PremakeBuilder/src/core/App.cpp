#include "App.h"
#include <imgui.h>
#include <imgui_internal.h>

App* App::s_Instance = nullptr;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

App::App()
{
}

App::~App()
{
}

void App::Start()
{
    s_Instance = this;
    LOG_INFO("Starting application...");

    Window::WindowData data = { 1024 , 768 , "Premake Builder" };
    m_window = CreateRef<Window>(data);

    m_ImGuiAPI->OnAttach();

	//m_file.GenerateFile();
}

void App::Update()
{
    while (m_Running) {
        processInput(m_window->GetNativeWindow());

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_ImGuiAPI->Begin();

        OnImGuiRender();

        m_ImGuiAPI->End();

        m_window->Update();
    }
}

void App::ShutDown()
{
    m_ImGuiAPI->OnDetach();
    LOG_INFO("Application terminated gracefully");
}

void App::OnImGuiRender()
{
    // Dockspace setup
    static bool* p_open = nullptr;
    static bool opt_fullscreen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("DockSpace", p_open, window_flags);
    ImGui::PopStyleVar(); // Window padding
    if (opt_fullscreen)
        ImGui::PopStyleVar(2); // Rounding + border size

    // Create DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);

    // Menu bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"));
            if (ImGui::MenuItem("Load"));
            if (ImGui::MenuItem("Save Us"));
            if (ImGui::MenuItem("Export"));
            if (ImGui::MenuItem("Exit")) App::Get().Close();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("New Workspace")) m_file.AddWorkspace();
            if (ImGui::MenuItem("New Project")) m_file.AddProject();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    m_file.RenderUI(dockspace_id);
    ImGui::End();
	//DOCKSPACE END
}

void App::Close()
{
	m_Running = false;
}
