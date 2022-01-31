#pragma once
#include <string>

#include <GLFW/glfw3.h>

class GLFWwindow;

namespace GameEngine {
	class Window {
	public:
		Window(const unsigned int width, const unsigned int height, std::string title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();

		unsigned int get_width() const { return m_width; }
		unsigned int get_height() const { return m_height; }
	private:
		int init();
		void shutdown();

		GLFWwindow* m_pWindow;

		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
	};
}

