#include <GameEngineCore/InterfaceManager.hpp>



namespace GameEngine {

	std::vector<InterfaceCombo*> InterfaceManager::combos = std::vector<InterfaceCombo*>();

	void InterfaceManager::init(GLFWwindow * window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
	}

	void InterfaceManager::addCombo(std::string name, std::vector<std::string> items, std::string preview, std::function<void(std::string)> onSelect) {
		combos.push_back(new InterfaceCombo(name, items, "", onSelect));
	}

	void InterfaceManager::render(float width, float height) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(width);
		io.DisplaySize.y = static_cast<float>(height);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		ImGui::Begin("Objects transform");
		

		for (auto combo : combos) {
			
			if (ImGui::BeginCombo(combo->name.c_str(), combo->preview.c_str()))
			{
				for (auto item : combo->items) {
					bool isSelected = combo->preview == item;

					if (ImGui::Selectable(item.c_str(), isSelected)) {
						combo->onSelect(item);
						combo->preview = item;
					}
					if (isSelected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			
		}

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

}