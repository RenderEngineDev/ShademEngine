#include "GUI/GUI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GUI::GUI() {
	setupMenuProperties();
};

bool GUI::configure(GLFWwindow* window) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;
}

void GUI::setupMenuProperties() {
	showDemoWindow = false;
	listboxItemCurrent = -1;
}

void GUI::startGuiFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

// Metoda do wywalenia, tymczasowo zostawiona na potrzeby ogarniecia GUI
void GUI::drawDemoGuiWindow() {
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);
}

void GUI::draw() {
	drawDemoGuiWindow();
	drawMainMenu();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::drawMainMenu() {
	ImGui::Begin("Shadem menu");
	ImGui::ListBox("Levels\n(single select)", &listboxItemCurrent, levels, IM_ARRAYSIZE(levels), IM_ARRAYSIZE(levels));
	ImGui::Checkbox("Demo Window", &showDemoWindow);
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f, io.Framerate, io.Framerate);
	ImGui::End();
}