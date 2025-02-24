#include "PCH.h"
#include "Engine.h"
#include "Actor/Actor.h"

#include <conio.h>

static BOOL WINAPI MessageProcessor(DWORD message)
{
	switch (message)
	{
	case CTRL_CLOSE_EVENT:
		Engine::Get().Shutdown();
		return true;

	default:
		return false;
	}
}

Engine* Engine::instance = nullptr;

Engine::Engine()
	: m_inShutdown(false), mainLevel(nullptr), screenSize(220, 55)
{
	instance = this;

	SetTargetFrameRate(60.0f);
	
	imageBuffer = new CHAR_INFO[(screenSize.xpos + 1) * screenSize.ypos + 1];
	ClearImageBuffer();

	COORD size = { (short)(screenSize.xpos), (short)(screenSize.ypos) };
	renderTargets[0] = new ScreenBuffer(size);
	renderTargets[1] = new ScreenBuffer(size);

	Present();

	SetConsoleCtrlHandler(MessageProcessor, true);

	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(inputHandle, ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_EXTENDED_FLAGS);
}

Engine::~Engine()
{
	if (mainLevel != nullptr)
	{
		delete mainLevel;
	}

	delete[] imageBuffer;

	delete renderTargets[0];
	delete renderTargets[1];
}

void Engine::Run()
{
	LARGE_INTEGER frequency, time;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&time);

	int64_t currentTime = time.QuadPart;
	int64_t previousTime = currentTime;

	EnableMouseInput();

	while (true)
	{
		if (m_inShutdown)
		{
			break;
		}

		QueryPerformanceCounter(&time);
		currentTime = time.QuadPart;

		m_deltaTime = static_cast<float>(currentTime - previousTime) /
			static_cast<float>(frequency.QuadPart);

		if (m_deltaTime >= m_targetOneFrameTime)
		{
			ProcessInput();
			Update(m_deltaTime);
			Render();

			previousTime = currentTime;
		}
	}
}

void Engine::Shutdown()
{
	m_inShutdown = true;
}

void Engine::SetTargetFrameRate(float targetFrameRate)
{
	m_targetFrameRate = targetFrameRate;
	m_targetOneFrameTime = 1.0f / targetFrameRate;
}

void Engine::LoadLevel(Level* newLevel)
{
	mainLevel = newLevel;
}

void Engine::AddActor(Actor* newActor)
{
	if (mainLevel == nullptr)
	{
		return;
	}

	mainLevel->AddActor(newActor);
}

void Engine::DestroyActor(Actor* targetActor)
{
	if (mainLevel == nullptr)
	{
		return;
	}

	targetActor->Destroy();
}

Engine& Engine::Get()
{
	return *instance;
}

void Engine::ProcessInput()
{
	static HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD record;
	DWORD events;
	if (PeekConsoleInput(inputHandle, &record, 1, &events) && events > 0)
	{
		if (ReadConsoleInput(inputHandle, &record, 1, &events))
		{
			switch (record.EventType)
			{
			case KEY_EVENT:
			{
				if (record.Event.KeyEvent.bKeyDown)
				{
					keyState[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = true;
				}
				else
				{
					keyState[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = false;
				}
			}
			break;

			case MOUSE_EVENT:
			{
				mousePosition.xpos = record.Event.MouseEvent.dwMousePosition.X;
				mousePosition.ypos = record.Event.MouseEvent.dwMousePosition.Y;

				keyState[VK_LBUTTON].isKeyDown
					= (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
			}
			break;
			}
		}
	}
}

void Engine::Update(float deltaTime)
{
	if (mainLevel != nullptr)
	{
		mainLevel->Update(deltaTime);
	}
}

void Engine::Render()
{
	Clear();

	if (mainLevel != nullptr)
	{
		mainLevel->Render();
	}

	Present();
}

void Engine::Render(const Vector2& position, const char* image, Color color)
{
	for (int ix = 0; ix < (int)strlen(image); ++ix)
	{
		int index = (position.ypos * (screenSize.xpos)) + position.xpos + ix;
		imageBuffer[index].Char.AsciiChar = image[ix];
		imageBuffer[index].Attributes = (unsigned long)color;
	}
}

void Engine::SetCursorType(CursorType cursorType)
{
	GetRenderer()->SetCursorType(cursorType);
}

bool Engine::GetKey(int key) const
{
	return keyState[key].isKeyDown;
}

bool Engine::GetKeyDown(int key) const
{
	return keyState[key].isKeyDown && !keyState[key].wasKeyDown;
}

bool Engine::GetKeyUp(int key) const
{
	return !keyState[key].isKeyDown && keyState[key].wasKeyDown;
}

void Engine::PresentImmediately()
{
	Present();
}

void Engine::Clear()
{
	ClearImageBuffer();
	GetRenderer()->Clear();
}

void Engine::Present()
{
	GetRenderer()->Draw(imageBuffer);

	// Swap Buffer.
	SetConsoleActiveScreenBuffer(GetRenderer()->buffer);
	currentRenderTargetIndex = 1 - currentRenderTargetIndex;
}

void Engine::EnableMouseInput()
{
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

	DWORD mode;
	GetConsoleMode(inputHandle, &mode);
	SetConsoleMode(inputHandle, mode | ENABLE_MOUSE_INPUT);
}

void Engine::SavePreviouseKeyStates()
{
	for (int ix = 0; ix < 255; ++ix)
	{
		keyState[ix].wasKeyDown = keyState[ix].isKeyDown;
	}
}

void Engine::ClearImageBuffer()
{
	for (int y = 0; y < screenSize.ypos; ++y)
	{
		for (int x = 0; x < screenSize.xpos + 1; ++x)
		{
			auto& buffer = imageBuffer[(y * (screenSize.xpos + 1)) + x];
			buffer.Char.AsciiChar = ' ';
			buffer.Attributes = 0;
		}

		auto& buffer = imageBuffer[(y * (screenSize.xpos + 1)) + screenSize.xpos];
		buffer.Char.AsciiChar = '\n';
		buffer.Attributes = 0;
	}

	auto& buffer = imageBuffer[(screenSize.xpos + 1) * screenSize.ypos];
	buffer.Char.AsciiChar = '\0';
	buffer.Attributes = 0;
}
