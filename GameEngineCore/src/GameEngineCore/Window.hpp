#pragma once

#include "GameEngineCore/Event.hpp"

#include <string>
#include <functional>

#include <GLFW/glfw3.h>

class GLFWwindow;

namespace GameEngine {
	class Window {
	public:
		//using <==> typedef (resize callback function type)
		using EventCallbackFn = std::function<void(BaseEvent&)>;


		Window(const unsigned int width, const unsigned int height, std::string title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();

		unsigned int get_width() const { return m_data.width; }
		unsigned int get_height() const { return m_data.height; }

		//set callback from Application, it will be called, when the specific event (f.e. window resize) was triggered
		void set_event_callback(const EventCallbackFn& cb) {
			m_data.eventCallbackFn = cb;
		} 
	private:
		//personal WindowData for the window
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

		GLFWwindow* m_pWindow;
		WindowData m_data;

	};
}

