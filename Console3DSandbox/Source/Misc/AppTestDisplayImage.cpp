#include <iostream>

#include "AppTestDisplayImage.h"

#include <Console3D/Source/Core/Console.h>
#include <Console3D/Source/Scene/StdComponent.h>

namespace LCN
{
	/////////////////////////////
	//-- AppTestDisplayImage --//
	/////////////////////////////

	AppTestDisplayImage::AppTestDisplayImage() :
		SLOT_INIT(SlotStartup, AppTestDisplayImage::Startup)
	{
		Connect(this->SignalStartup, this->SlotStartup);
	}

	void AppTestDisplayImage::Run()
	{
		this->SignalStartup.Trigger();

		this->WaitQuit();

		this->SignalQuit.Trigger();
	}

	void AppTestDisplayImage::Startup()
	{
		m_Camera = m_Scene.CreateEntity();
		m_Image  = m_Scene.CreateEntity();

		TextureComponent& textureCmp = m_Image.Add<TextureComponent>();

		textureCmp.Texture.Load("Ressource/Le_Chat_Noir_Photo_Medium.png", STBI_grey);

		size_t texturew = textureCmp.Texture.Width();
		size_t textureh = textureCmp.Texture.Height();

		Sprite2DComponent& sprite = m_Image.Add<Sprite2DComponent>();

		sprite.Sprite.Vertices[0] = { (float)(-(int)texturew / 2), (float)( (int)textureh / 2), 1.0f };
		sprite.Sprite.Vertices[1] = { (float)( (int)texturew / 2), (float)( (int)textureh / 2), 1.0f };
		sprite.Sprite.Vertices[2] = { (float)( (int)texturew / 2), (float)(-(int)textureh / 2), 1.0f };
		sprite.Sprite.Vertices[3] = { (float)(-(int)texturew / 2), (float)(-(int)textureh / 2), 1.0f };

		sprite.Sprite.TectureCoords[0] = { 0, 0 };
		sprite.Sprite.TectureCoords[1] = { 1, 0 };
		sprite.Sprite.TectureCoords[2] = { 1, 1 };
		sprite.Sprite.TectureCoords[3] = { 0, 1 };

		sprite.Sprite.Faces[0] = { 0, 1, 3 };
		sprite.Sprite.Faces[1] = { 1, 2, 3 };

		Console& console = Console::Get();

		console.ConstructConsole(300, 200, 4, 4);

		m_Camera.Add<Camera2DComponent>(console.Width(), console.Height());

		m_Scene.Render(m_Camera);
	}
}