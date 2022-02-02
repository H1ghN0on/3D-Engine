#pragma once

#include <array>
#include <functional>

namespace GameEngine {
	enum class EventType {
		WindowResize,
		WindowClose,
		
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventCount
	};

	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};

	class EventDispatcher {
	public:
		template <class EventType>
		void addEventListener(std::function<void(EventType&)> callback) {
			//transform EventType to BaseEvent
			auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
				if (e.get_type() == EventType::type) {
					func(static_cast<EventType&>(e));
				}
			};
			m_EventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}
	
		void dispatch(BaseEvent& event) {
			auto& callback = m_EventCallbacks[static_cast<size_t>(event.get_type())];
			if (callback) {
				callback(event);
			}
		}
	private:
		//all event callbacks
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventCount)>  m_EventCallbacks;
	};


	struct WindowResizeEvent : public BaseEvent {
		WindowResizeEvent(unsigned int width, unsigned int height)
			: width(width)
			, height(height)
		{}

		EventType get_type() const override {
			return EventType::WindowResize;
		}
		unsigned int width;
		unsigned int height;

		static const EventType type = EventType::WindowResize;
		
	};


	struct WindowCloseEvent : public BaseEvent {
		EventType get_type() const override {
			return EventType::WindowClose;
		}
		static const EventType type = EventType::WindowClose;
	};

	struct MouseMovedEvent : public BaseEvent {
		MouseMovedEvent(double new_x, double new_y)
			: x(new_x)
			, y(new_y) 
		{}
		EventType get_type() const override {
			return EventType::MouseMoved;
		}

		double x, y;

		static const EventType type = EventType::MouseMoved;
	};
}

