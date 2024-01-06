#pragma once

#include "Scene/Scene.h"
#include "CubeMarching/CmObject.h"
#include "Window/Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class GUI {
private:
	const char* glsl_version = "#version 130";
	bool showDemoWindow;
	bool showSceneMenu;
	// w przysz³oœci nazwy poziomów wraz z ich parametrami do zaci¹gania z pliku 
	const char* levels[3] = { "CmLevel", "RmLevel", "CustomLevel" };

public:
	int listboxItemCurrent;
	// TODO: selectedObjectIndex - do zmiany na listê obiektów, kiedy zmieni siê formê zaznaczania obiektów do edycji
	int selectedObjectIndex;

	GUI();

	bool configure(GLFWwindow* window);
	void setupStartUpProperties();
	void startGuiFrame();

	void draw(Scene* scene);
	void drawObjectListWindow(Scene* scene);
	// Metoda do wywalenia, tymczasowo zostawiona na potrzeby ogarniecia GUI
	void drawDemoGuiWindow();

private:
	void drawMainMenu();
	
	template <class T>
	void handleObjectSelection(T* selectedObject);
	void handleOtherAttributesSelection(CubeMarching::Noise* noise);
	void handleOtherAttributesSelection(CubeMarching::Sphere* sphere);
	
};
