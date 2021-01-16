#include "Application.h"

namespace LCN
{
	Application::Application() :
		SLOT_INIT(SlotDispatchKeyPressedEvent,          Application::DispatchKeyPressedEvent),
		SLOT_INIT(SlotDispatchKeyReleasedEvent,         Application::DispatchKeyReleasedEvent),
		SLOT_INIT(SlotDispatchMouseMoveEvent,           Application::DispatchMouseMoveEvent),
		SLOT_INIT(SlotDispatchMouseButtonPressedEvent,  Application::DispatchMouseButtonPressedEvent),
		SLOT_INIT(SlotDispatchMouseButtonReleasedEvent, Application::DispatchMouseButtonReleasedEvent),
		SLOT_INIT(SlotDispatchMouseScrolledEvent,       Application::DispatchMouseScrolledEvent)
	{
		if (m_App)
			throw std::exception("Application is already running.");

		m_App = this;

		ConsoleInput& consoleinput = ConsoleInput::Get();

		Connect(consoleinput.SignalKeyPressed,          this->SlotDispatchKeyPressedEvent);
		Connect(consoleinput.SignalKeyReleased,         this->SlotDispatchKeyReleasedEvent);
		Connect(consoleinput.SignalMouseMove,           this->SlotDispatchMouseMoveEvent);
		Connect(consoleinput.SignalMouseButtonPressed,  this->SlotDispatchMouseButtonPressedEvent);
		Connect(consoleinput.SignalMouseButtonReleased, this->SlotDispatchMouseButtonReleasedEvent);
		Connect(consoleinput.SignalMouseScroll,         this->SlotDispatchMouseScrolledEvent);

		consoleinput.Start();
	}

	void Application::Quit()
	{
		std::lock_guard<std::mutex> lock(m_RunMut);

		m_Run = false;

		m_RunCond.notify_one();
	}

	void Application::DispatchKeyPressedEvent(KeyPressedEvent& keypressedevent)    { this->SignalKeyPressed.Emmit(keypressedevent); }
	void Application::DispatchKeyReleasedEvent(KeyReleasedEvent& keyreleasedevent) { this->SignalKeyReleased.Emmit(keyreleasedevent); }

	void Application::DispatchMouseMoveEvent(MouseMovedEvent& mousemovedevent)                             { this->SignalMouseMoved.Emmit(mousemovedevent); }
	void Application::DispatchMouseButtonPressedEvent(MouseButtonPressedEvent& mousebuttonpressedevent)    { this->SignalMouseButtonPressed.Emmit(mousebuttonpressedevent); }
	void Application::DispatchMouseButtonReleasedEvent(MouseButtonReleasedEvent& mousebuttonreleasedevent) { this->SignalMouseButtonReleased.Emmit(mousebuttonreleasedevent); }
	void Application::DispatchMouseScrolledEvent(MouseScrollEvent& mousescrollevent)                       { this->SignalMouseScrolled.Emmit(mousescrollevent); }

	void Application::WaitQuit()
	{
		std::unique_lock<std::mutex> lock(m_RunMut);

		while (m_Run)
			m_RunCond.wait(lock);
	}
}