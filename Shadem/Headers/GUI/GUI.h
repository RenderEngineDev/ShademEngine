#pragma once

#include "Window/Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class GUI {
private:
	const char* glsl_version = "#version 130";
	bool showDemoWindow;
	// w przysz�o�ci nazwy poziom�w wraz z ich parametrami do zaci�gania z pliku 
	const char* levels[3] = { "CmLevel", "RmLevel", "CustomLevel" };

public:
	int listboxItemCurrent;

	GUI();

	bool configure(GLFWwindow* window);
	void setupMenuProperties();
	void startGuiFrame();
	
	// Metoda do wywalenia, tymczasowo zostawiona na potrzeby ogarniecia GUI
	void drawDemoGuiWindow();

	void draw();
private:
	void drawMainMenu();
};

