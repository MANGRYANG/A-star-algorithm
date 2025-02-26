#include "GameplayLevel.h"

GameplayLevel::GameplayLevel()
    : mousePos{ 0, 0 }, consoleRect{ 0, 0, 0, 0 },
    leftAnimals{ true, true, true, true, true, true }, rightAnimals{ false, false, false, false, false, false },
    leftRaftAnimal{ -1, -1 }, rightRaftAnimal{ -1, -1 }
{
    AddActor(new Text(puzzleString_01.c_str(), Vector2(2, 1)));
    AddActor(new Text(puzzleString_02.c_str(), Vector2(4, 3)));
    AddActor(new Text(puzzleString_03.c_str(), Vector2(4, 4)));
    AddActor(new Text(puzzleString_04.c_str(), Vector2(4, 5)));
    AddActor(new Text(puzzleString_05.c_str(), Vector2(2, 7)));

    AddActor(new Wolf(Vector2(4, 23)));
    AddActor(new Wolf(Vector2(12, 23)));
    AddActor(new Wolf(Vector2(20, 23)));

    AddActor(new Chick(Vector2(3, 26)));
    AddActor(new Chick(Vector2(11, 26)));
    AddActor(new Chick(Vector2(19, 26)));

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
            int titleBarHeight = 30;
            int consoleSideWidth = 7;
            int relativeX = mousePos.x - consoleRect.left - consoleSideWidth;
            int relativeY = mousePos.y - consoleRect.top - titleBarHeight;
            

            if (relativeX >= 0 && relativeX < (consoleRect.right - consoleRect.left) &&
                relativeY >= 0 && relativeY < (consoleRect.bottom - consoleRect.top - titleBarHeight))
            {
                // Raft : Left side
                if (isRaftLeft)
                {
                    if (isRaftFull())
                    {

                    }
                    else
                    {
                        for (int idx = 0; idx < 3; ++idx)
                        {
                            // Wolf selected
                            if (leftAnimals[idx] &&
                                relativeX / 8 >= 4 + (idx * 8) && relativeX / 8 <= 9 + (idx * 8) &&
                                relativeY / 17 >= 22 && relativeY / 17 <= 23)
                            {
                                Wolf* tempWolfActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempWolfActor = dynamic_cast<Wolf*>(actor);
                                    if (tempWolfActor != nullptr &&
                                        actor->GetPosition() == Vector2(4 + (8 * idx), 23))
                                    {
                                        break;
                                    }
                                }

                                leftAnimals[idx] = false;

                                // Raft is empty
                                if (leftRaftAnimal[0] == -1)
                                {
                                    leftRaftAnimal[0] = 0;
                                    
                                    if (tempWolfActor != nullptr)
                                    {
                                        tempWolfActor->SetPosition(Vector2(42, 26));
                                    }
                                }
                                // One animal exists on Raft
                                else
                                {
                                    leftRaftAnimal[1] = 0;

                                    if (tempWolfActor != nullptr)
                                    {
                                        tempWolfActor->SetPosition(Vector2(55, 26));
                                    }
                                }
                            }

                            // Chick selected
                            else if (leftAnimals[idx + 3] &&
                                relativeX / 8 >= 3 + (idx * 8) && relativeX / 8 <= 9 + (idx * 8) &&
                                relativeY / 17 >= 25 && relativeY / 17 <= 26)
                            {
                                Chick* tempChickActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempChickActor = dynamic_cast<Chick*>(actor);
                                    if (tempChickActor != nullptr &&
                                        actor->GetPosition() == Vector2(3 + (8 * idx), 26))
                                    {
                                        break;
                                    }
                                }

                                leftAnimals[idx + 3] = false;

                                // Raft is empty
                                if (leftRaftAnimal[0] == -1)
                                {
                                    leftRaftAnimal[0] = 1;

                                    if (tempChickActor != nullptr)
                                    {
                                        tempChickActor->SetPosition(Vector2(41, 26));
                                    }
                                }
                                // One animal exists on Raft
                                else
                                {
                                    leftRaftAnimal[1] = 1;

                                    if (tempChickActor != nullptr)
                                    {
                                        tempChickActor->SetPosition(Vector2(54, 26));
                                    }
                                }
                            }
                        }
                        ProcessAddedAndDestroyedActor();
                    }
                }        
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

    std::string left_slash = "_Y";
    std::string right_slash = "Y_";

    for (int lineNumber = 34; lineNumber > 20; --lineNumber)
    {
        Engine::Get().Render(Vector2(17 + 2 * (34 - lineNumber), lineNumber), left_slash.c_str(), Color::Green);
        Engine::Get().Render(Vector2(100 + 2 * (lineNumber - 21), lineNumber), right_slash.c_str(), Color::Green);
    }
    
    // Draw river-line
    Engine::Get().Render(Vector2(45, 21), "-------------------------------------------------------", Color::Cyan);
    Engine::Get().Render(Vector2(17, 35), "---------------------------------------------------------------------------------------------------------------", Color::Cyan);

    if (isRaftLeft)
    {
        // Draw raft in left side
        std::string raft = "***************************";
        for (int idx = 0; idx < 9; ++idx)
        {
            if (idx == 4)
            {
                if (leftRaftAnimal[0] != -1)
                {
                    if (leftRaftAnimal[1] != -1)
                    {
                        Engine::Get().Render(Vector2(36, 26), "*****", Color::Yellow);
                        Engine::Get().Render(Vector2(48, 26), "******", Color::Yellow);
                        Engine::Get().Render(Vector2(62, 26), "*****", Color::Yellow);
                        raft += '*';
                        continue;
                    }
                    else
                    {
                        Engine::Get().Render(Vector2(36, 26), "*****", Color::Yellow);
                        Engine::Get().Render(Vector2(48, 26), "*******************", Color::Yellow);
                        raft += '*';
                        continue;
                    }
                }
            }
            Engine::Get().Render(Vector2(44 - (2 * idx), 22 + idx), raft.c_str(), Color::Yellow);
            raft += '*';
        }
    }
}

bool GameplayLevel::isRaftFull()
{
    if (isRaftLeft && leftRaftAnimal[1] != -1)
    {
        return true;
        
    }

    else if (!isRaftLeft && rightRaftAnimal[1] != -1)
    {
        return true;
    }

    return false;
}
