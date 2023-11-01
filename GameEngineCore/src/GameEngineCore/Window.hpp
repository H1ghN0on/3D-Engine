#pragma once

#include "GameEngineCore/Event.hpp"

#include <string>
#include <functional>
#include <map>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

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

		static float getDeltaTime() { return deltaTime; }
		static float getLastFrame() { return lastFrame; }

		static void bindKeyPress(int keyCode, std::function<void()>);
		static void bindKeyPressedOnce(int keyCode, std::function<void()>);
		static void bindKeyRelease(int keyCode, std::function<void()>);

		static void clearBinders() {
			pressedKeysForListen.clear();
			pressedOnceKeysForListen.clear();
			releasedKeysForListen.clear();
		}
		
		static glm::vec2 getScreenSize() {
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			return glm::vec2(viewport[2], viewport[3]);
		}

		static void setBackgroundColor(glm::vec4 color) {
			bgColor[0] = color.x;
			bgColor[1] = color.y;
			bgColor[2] = color.z;
			bgColor[3] = color.w;
		}


	private:

		static bool isPressed;

		static void handleKeyPress();

		struct WindowData {
			unsigned int width;
			unsigned int height;
			std::string title;

			double mouseX;
			double mouseY;
			EventCallbackFn eventCallbackFn;
		};

		static GLFWwindow* window;
		WindowData data;

		static void updateDeltaTime();

		static float deltaTime;
		static float lastFrame;

		int init();
		void shutdown();
	
		/*float bgColor[4] = { 0.53, 0.81f, 0.92f, 0.0f };*/
		static float bgColor[4];

		static std::map<int, std::function<void()>> pressedKeysForListen;
		static std::map<int, std::function<void()>> pressedOnceKeysForListen;
		static std::map<int, std::function<void()>> releasedKeysForListen;
	};
}

