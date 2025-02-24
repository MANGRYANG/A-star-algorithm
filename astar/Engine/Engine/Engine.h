#pragma once

#include "CoreMinimal.h"
#include "Level/Level.h"
#include "Render/ScreenBuffer.h"
#include "Math/Vector2.h"

struct KeyState
{
	bool isKeyDown = false;
	bool wasKeyDown = false;
};

class Level;
class Actor;
class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	// Functions for managing the engine
	void Run();
	void Shutdown();
	void SetTargetFrameRate(float targetFrameRate);

	void LoadLevel(Level* newLevel);

	void AddActor(Actor* newActor);
	void DestroyActor(Actor* targetActor);

	void Render(const Vector2& position, const char* image, Color color = Color::White);

	void SetCursorType(CursorType cursorType);

	bool GetKey(int key) const;
	bool GetKeyDown(int key) const;
	bool GetKeyUp(int key) const;

	void PresentImmediately();

	static Engine& Get();

protected:
	// Functions for game loop
	void ProcessInput();
	void Update(float deltaTime);
	void Render();

	void Clear();
	void Present();

	void EnableMouseInput();
	void SavePreviouseKeyStates();

	inline ScreenBuffer* GetRenderer() const { return renderTargets[currentRenderTargetIndex]; }
	void ClearImageBuffer();

	
protected:
	// Variables for managing the engine
	bool m_inShutdown = false;

	float m_targetFrameRate = 60.0f;
	float m_targetOneFrameTime = 0.0f;
	float m_deltaTime;

	Level* mainLevel;

	KeyState keyState[255];

	Vector2 screenSize;
	Vector2 mousePosition;

	CHAR_INFO* imageBuffer = nullptr;

	ScreenBuffer* renderTargets[2];
	int currentRenderTargetIndex = 0;

	static Engine* instance;
};