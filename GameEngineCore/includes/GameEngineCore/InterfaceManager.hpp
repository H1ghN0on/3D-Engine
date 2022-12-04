#pragma once

#include <vector>
#include <string>
#include <functional>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

namespace GameEngine {

	struct InterfaceCombo {
		std::string name;
		std::vector<std::string> items;
		std::string preview;
		std::function<void(std::string)> onSelect;


		InterfaceCombo() {}

		InterfaceCombo(std::string name, std::vector<std::string> items, std::string preview, std::function<void(std::string)> onSelect) {
			this->name = name;
			this->items = items;
			this->preview = preview;
			this->onSelect = onSelect;
		}
	};

	static class InterfaceManager {

	private:
		static std::vector<InterfaceCombo*> combos;

	public:

		static void addCombo(std::string name, std::vector<std::string> items, std::string preview, std::function<void(std::string)> onSelect);
		static void init(GLFWwindow* window);

		static void render(float width, float height);

	};

	
}