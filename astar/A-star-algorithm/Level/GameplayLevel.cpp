#include "GameplayLevel.h"

GameplayLevel::GameplayLevel()
    : mousePos{ 0, 0 }, consoleRect{ 0, 0, 0, 0 }
{
    AddActor(new Text(puzzleString_01.c_str(), Vector2(2, 1)));
    AddActor(new Text(puzzleString_02.c_str(), Vector2(4, 3)));
    AddActor(new Text(puzzleString_03.c_str(), Vector2(4, 4)));
    AddActor(new Text(puzzleString_04.c_str(), Vector2(4, 5)));
    AddActor(new Text(puzzleString_05.c_str(), Vector2(2, 7)));

    ProcessAddedAndDestroyedActor();
}

GameplayLevel::~GameplayLevel()
{
}

void GameplayLevel::Update(float deltaTime)
{
    Super::Update(deltaTime);

    GetWindowRect(consoleWindow, &consoleRect);

    if (GetAsyncKeyState(VK_LBUTTON) & 1)
    {
        if (GetCursorPos(&mousePos))
        {
            int relativeX = mousePos.x - consoleRect.left;
            int relativeY = mousePos.y - consoleRect.top;
            int titleBarHeight = 30;

            if (relativeX >= 0 && relativeX < (consoleRect.right - consoleRect.left) &&
                relativeY >= titleBarHeight && relativeY < (consoleRect.bottom - consoleRect.top))
            {
                std::cout << relativeX << relativeY;

            
            }
        }
    }
}

void GameplayLevel::Render()
{
    Super::Render();

    // Rendering the board
    Engine::Get().Render(Vector2(0, 20), "_____________________________________________", Color::Green);
    Engine::Get().Render(Vector2(0, 35), "________________|", Color::Green);

    Engine::Get().Render(Vector2(100, 20), "____________________________________________", Color::Green);
    Engine::Get().Render(Vector2(128, 35), "|________________", Color::Green);

    std::string left_slash = "_\\";
    std::string right_slash = "\\_";

    for (int lineNumber = 34; lineNumber > 20; --lineNumber)
    {
        Engine::Get().Render(Vector2(17 + 2 * (34 - lineNumber), lineNumber), left_slash.c_str(), Color::Green);
        Engine::Get().Render(Vector2(100 + 2 * (lineNumber - 21), lineNumber), right_slash.c_str(), Color::Green);
    }
    
    Engine::Get().Render(Vector2(45, 21), "-------------------------------------------------------", Color::Cyan);
    Engine::Get().Render(Vector2(17, 35), "---------------------------------------------------------------------------------------------------------------", Color::Cyan);
}
