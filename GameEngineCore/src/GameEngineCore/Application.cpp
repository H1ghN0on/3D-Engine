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
        //setting callback to window


        m_eventDispatcher.addEventListener<WindowResizeEvent>(
            [](WindowResizeEvent& event) {
                LOG_INFO("[EVENT] Set window size to {0}x{1}", event.width, event.height);
            }
        );

        m_eventDispatcher.addEventListener<WindowCloseEvent>(
            [&](WindowCloseEvent& event) {
                this -> m_isWindowClosed = true;
                LOG_INFO("[EVENT] Window Closed");
            }
        );

        m_eventDispatcher.addEventListener<MouseMovedEvent>(
            [&](MouseMovedEvent& event) {
      
            }
        );

        m_pWindow->setEventCallback(
            [&](BaseEvent& event) {
                m_eventDispatcher.dispatch(event);
            }
        );

        this -> initScene();

        while (!m_isWindowClosed) {

            m_pWindow->on_update();
            this -> update();
        }

        return 0;
	}
}