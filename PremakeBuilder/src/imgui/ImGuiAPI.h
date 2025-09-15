#pragma once

class ImGuiAPI
{
public:
	ImGuiAPI();
	~ImGuiAPI();

	void OnAttach();
	void OnDetach();
	void OnImGuiRender();

	void Begin();
	void End();
};