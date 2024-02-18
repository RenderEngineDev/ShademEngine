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
	// w przysz�o�ci nazwy poziom�w wraz z ich parametrami do zaci�gania z pliku 
	const char* levels[3] = { "CmLevel", "RmLevel", "CustomLevel" };

public:
	int listboxItemCurrent;
	// TODO: selectedObjectIndex - do zmiany na list� obiekt�w, kiedy zmieni si� form� zaznaczania obiekt�w do edycji
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
