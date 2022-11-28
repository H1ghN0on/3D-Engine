#pragma once

#include "GameEngineCore/Event.hpp"

#include <string>
#include <functional>
#include "GameEngineCore/Rendering/OpenGL/Terrain.hpp"
#include <GLFW/glfw3.h>
#include "GameEngineCore/Rendering/OpenGL/ObjectManager.hpp"

class GLFWwindow;

namespace GameEngine {
	class Window {
	public:

		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(const unsigned int width, const unsigned int height, std::string title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();

		unsigned int getWidth() const { return data.width; }
		unsigned int getHeight() const { return data.height; }

		//set callback from Application, it will be called, when the specific event (f.e. window resize) was triggered
		void setEventCallback(const EventCallbackFn& cb) {
			data.eventCallbackFn = cb;
		}
	private:
		struct WindowData {
			unsigned int width;
			unsigned int height;
			std::string title;

			double mouseX;
			double mouseY;
			EventCallbackFn eventCallbackFn;
		};

		int init();
		void shutdown();
		GLFWwindow* window;
		WindowData data;
		float bgColor[4] = { 0.53, 0.81f, 0.92f, 0.0f };

	};
}

