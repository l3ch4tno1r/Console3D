#include <iostream>

#include "Console3D/Source/Core/EventApplication.h"
#include "Console3D/Source/Core/Console.h"

#include "Console3D/Source/Rendering/Renderer2D.h"

#include "Console3D/Source/Scene/Scene.h"
#include "Console3D/Source/Scene/Entity.h"
#include "Console3D/Source/Scene/StdComponent.h"

#include "Utilities/Source/Instrumentor.h"

using namespace std::chrono_literals;

namespace LCN
{
	class BenchMarkApp : public Core::EventApplication
	{
	public:
		BenchMarkApp() :
			SLOT_INIT(SlotOnStartup, BenchMarkApp::OnStartup)
		{
			Connect(this->SignalStartup, this->SlotOnStartup);
		}

	private: // Slots
		SLOT(BenchMarkApp, OnStartup);

	private:
		Scene  m_Scene;
		Entity m_Camera;
		Entity m_Sprite1;
		Entity m_Sprite2;
	};

	void BenchMarkApp::OnStartup()
	{
		m_Camera = m_Scene.CreateEntity();
		m_Sprite1 = m_Scene.CreateEntity();
		m_Sprite2 = m_Scene.CreateEntity();

		// Sprite 1
		Component::TextureCmp& textureCmp1 = m_Sprite1.Add<Component::TextureCmp>();

		textureCmp1.Texture.Load("../Console3DSandbox/Ressource/Le_Chat_Noir_Photo_Alpha.png");

		ASSERT(textureCmp1.Texture);

		size_t texturew1 = textureCmp1.Texture.Width();
		size_t textureh1 = textureCmp1.Texture.Height();

		m_Sprite1.Add<Component::Sprite2DCmp>(texturew1, textureh1);

		// Sprite 2
		Component::TextureCmp& textureCmp2 = m_Sprite2.Add<Component::TextureCmp>();

		textureCmp2.Texture.Load("../Console3DSandbox/Ressource/Wooden_Medium.jpg");

		ASSERT(textureCmp2.Texture);

		size_t texturew2 = textureCmp2.Texture.Width();
		size_t textureh2 = textureCmp2.Texture.Height();

		m_Sprite2.Add<Component::Sprite2DCmp>(texturew2, textureh2);

		Core::Console& console = Core::Console::Get();

		/*
		console.ConstructConsole(150, 100, 8, 8);
		*/
		console.ConstructConsole(300, 200, 4, 4);

		m_Camera.Add<Component::Camera2DCmp>(console.Width(), console.Height());

		for (size_t i = 0; i < 5; ++i)
		{
			// Timing tests
			Instrumentor::Get().BeginSession("Parallelization Test");
		
			// Render
			{
				PROFILE_SCOPE("Renderer2D::Render");

				Render::Renderer2D::RenderSequential(m_Scene, m_Camera);

				console.Render();
			}

			// RenderParallel
			{
				PROFILE_SCOPE("Renderer2D::RenderParallel");

				Render::Renderer2D::RenderParallel(m_Scene, m_Camera);
			}
		}
	}

	Core::Application::AppPointer Core::Application::CreateApplication()
	{
		return std::make_unique<BenchMarkApp>();
	}
}

#include "Console3D/Source/Core/EntryPoint.h"