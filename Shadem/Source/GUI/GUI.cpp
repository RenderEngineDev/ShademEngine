#include "GUI/GUI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GUI::GUI() {
	setupStartUpProperties();
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
	glfwMakeContextCurrent(window); 
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;
}

void GUI::setupStartUpProperties() {
	showDemoWindow = false;
	listboxItemCurrent = -1;
	selectedObjectIndex = -1;
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

void GUI::drawObjectListWindow(Scene* scene) {
	
	//ImGui::BeginMenu("Scene objects")
	if (showSceneMenu && scene != nullptr) {
		ImGui::Begin("Scene Menu", &showSceneMenu);
		ImGui::BeginListBox("Objects", ImVec2(300, 100));
		std::vector<Object*> &objects = scene->getObjects();
		for (int i = 0; i < objects.size(); ++i) {
			std::string& item_name = std::to_string(i) + " " + objects[i]->getAttributes()->name;
			if (ImGui::Selectable(item_name.c_str(), selectedObjectIndex)) {
				// Zapisujemy indeks wybranego obiektu
				selectedObjectIndex = i;
				// handle selection, na przyk³ad mo¿esz wywo³aæ funkcjê obs³uguj¹c¹ wybór obiektu
			}
		}
		ImGui::EndListBox();
		if (selectedObjectIndex != -1) {
			handleObjectSelection(objects[selectedObjectIndex]);
		}
		ImGui::End();
	}
}


// Dodatkowa funkcja do obs³ugi wyboru obiektu
template <class T>
void GUI::handleObjectSelection(T* selectedObject) {
	ImGui::BeginChild((char*)(&selectedObject->getAttributes()->name), ImVec2(300, 200), 1, 1);
	ImGui::InputFloat3("Position", &selectedObject->getAttributes()->position.x);
	ImGui::InputFloat3("Scale", &selectedObject->getAttributes()->scale.x);
	ImGui::InputFloat3("Rotation", &selectedObject->getAttributes()->rotation.x);
	
	// SprawdŸ typ obiektu i wykonaj dynamiczne rzutowanie
	if (typeid(*selectedObject) == typeid(CubeMarching::Noise)) {
		handleOtherAttributesSelection(dynamic_cast<CubeMarching::Noise*>(selectedObject));
	}
	else if (typeid(*selectedObject) == typeid(CubeMarching::Sphere)) {
		handleOtherAttributesSelection(dynamic_cast<CubeMarching::Sphere*>(selectedObject));
	}

	ImGui::EndChild();
}

// TODO: Przekazywaæ atrybuty, ale dopiero po agarniêciu struktur i ich elementów?
// Do póki nie robimy generycznych klas to chyba trzeba na sztywno zrobiæ wszystkie przypadki
void GUI::handleOtherAttributesSelection(CubeMarching::Noise* noise) {
	auto attributes = noise->getAttributes();
	if (ImGui::SliderFloat("Iso value", &attributes->isoValue, -1.0f, 1.0f)) { noise->setUpdated(true); }
	if (ImGui::InputFloat("Frequency", &attributes->frequency, 0.1f, 1.0f)) { noise->setUpdated(true); }
	if (ImGui::InputFloat("Amplitude", &attributes->amplitude, 0.1f, 1.0f)) { noise->setUpdated(true); }
	if (ImGui::InputFloat("Lacunarity", &attributes->lacunarity, 0.1f, 1.0f)) { noise->setUpdated(true); }
	if (ImGui::InputFloat("Persistence", &attributes->persistence, 0.1f, 1.0f)) { noise->setUpdated(true); }
	if (ImGui::InputFloat("Offset X", &noise->getOffset().x, 5.0f, 10.0f)) { noise->setUpdated(true); }
	if (ImGui::InputFloat("Offset Y", &noise->getOffset().y, 5.0f, 10.0f)) { noise->setUpdated(true); }
	if (ImGui::InputFloat("Offset Z", &noise->getOffset().z, 5.0f, 10.0f)) { noise->setUpdated(true); }
}

void GUI::handleOtherAttributesSelection(CubeMarching::Sphere* sphere) {
	ImGui::SliderFloat("Iso value", &sphere->getAttributes()->isoValue, -1.0f, 1.0f);
	ImGui::InputFloat("Radius", &sphere->getAttributes()->radius, 1.0f, 5.0f);
}

void GUI::draw(Scene *scene) {
	drawDemoGuiWindow();
	drawMainMenu();
	drawObjectListWindow(scene);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::drawMainMenu() {
	ImGuiIO io = ImGui::GetIO();
	ImGui::Begin("Shadem menu");
	ImGui::ListBox("Levels\n(single select)", &listboxItemCurrent, levels, IM_ARRAYSIZE(levels), IM_ARRAYSIZE(levels));
	ImGui::Checkbox("Demo Window", &showDemoWindow);
	ImGui::Checkbox("Scene Menu Window", &showSceneMenu);
	ImGui::Text("(%.1f FPS)", io.Framerate);
	ImGui::End();
}