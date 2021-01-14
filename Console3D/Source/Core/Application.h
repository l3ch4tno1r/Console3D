#pragma once

#include <mutex>
#include <condition_variable>

#include <Utilities/Source/DesignPatterns/SignalSlot.h>
#include <Console3D/Source/Core/ConsoleInput.h>

int main(int argc, char** argv);

namespace LCN
{
	template<class _Derived>
	class Application
	{
	public:
		template<class F>
		using SignalApplication = Signal<Application, F>;

		template<class F>
		using SlotApplication = Slot<Application, F>;

		static Application& Get() noexcept
		{
			static _Derived instance;
			return instance;
		}

		void Quit()
		{
			std::lock_guard<std::mutex> lock(m_RunMut);

			m_Run = false;

			m_RunCond.notify_one();
		}

	public: // Signals
		SignalApplication<void(KeyPressedEvent&)>  SignalKeyPressed;
		SignalApplication<void(KeyReleasedEvent&)> SignalKeyReleased;

	private: // Slots
		void KeyPressed(KeyPressedEvent& keypressedevent) { this->SignalKeyPressed.Emmit(keypressedevent); }
		void KeyReleased(KeyReleasedEvent& keyreleasedevent) { this->SignalKeyReleased.Emmit(keyreleasedevent); }

		SlotApplication<void(KeyPressedEvent&)>  SlotOnKeyPressed;
		SlotApplication<void(KeyReleasedEvent&)> SlotOnKeyReleased;

	protected:
		inline _Derived& Derived() { return static_cast<_Derived&>(*this); }
		inline const _Derived& Derived() const { return static_cast<_Derived&>(*this); }

		Application() :
			SlotOnKeyPressed( SLOT_INIT(Application::KeyPressed)),
			SlotOnKeyReleased(SLOT_INIT(Application::KeyReleased))
		{
			if (m_App)
				throw std::exception("Application is already running.");

			m_App = this;

			ConsoleInput& eventhandler = ConsoleInput::Get();

			Bind(eventhandler.SignalKeyPressed,  this->SlotOnKeyPressed);
			Bind(eventhandler.SignalKeyReleased, this->SlotOnKeyReleased);

			eventhandler.Start();
		}
	
		virtual ~Application() = default;

		void WaitQuit()
		{
			std::unique_lock<std::mutex> lock(m_RunMut);

			while (m_Run)
				m_RunCond.wait(lock);
		}

	private:
		void Run() { this->Derived().Run(); }

		friend int ::main(int argc, char** argv);

	private:
		inline static Application* m_App = nullptr;

		bool                    m_Run = true;
		std::mutex              m_RunMut;
		std::condition_variable m_RunCond;
	};
}