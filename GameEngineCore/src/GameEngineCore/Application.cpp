#include <iostream>
#include <memory>

#include "GameEngineCore/Application.hpp"
#include "GameEngineCore/Log.hpp"
#include "GameEngineCore/Window.hpp"

namespace GameEngine {
    Application::Application() {
        LOG_INFO("IT'S HIGH NOON");
    }

	Application::~Application() {
        LOG_INFO("THIS IS THE END");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {
        m_pWindow = std::make_unique<Window>(window_width, window_height, title);

        while (true) {
            m_pWindow->on_update();
            this -> on_update();
        }

        return 0;
	}
}