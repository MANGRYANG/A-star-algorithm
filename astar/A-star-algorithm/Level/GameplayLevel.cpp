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

    AddActor(new Wolf(Vector2(3, 23)));
    AddActor(new Wolf(Vector2(11, 23)));
    AddActor(new Wolf(Vector2(19, 23)));

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
                    if(!isRaftFull())
                    {
                        for (int idx = 0; idx < 3; ++idx)
                        {
                            // Wolf selected
                            if (leftAnimals[idx] &&
                                relativeX / 8 >= 3 + (idx * 8) && relativeX / 8 <= 9 + (idx * 8) &&
                                relativeY / 17 >= 22 && relativeY / 17 <= 23)
                            {
                                Wolf* tempWolfActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempWolfActor = dynamic_cast<Wolf*>(actor);
                                    if (tempWolfActor != nullptr &&
                                        actor->GetPosition() == Vector2(3 + (8 * idx), 23))
                                    {
                                        break;
                                    }
                                }

                                leftAnimals[idx] = false;

                                if (leftRaftAnimal[0] == -1)
                                {
                                    leftRaftAnimal[0] = 0;

                                    if (tempWolfActor != nullptr)
                                    {
                                        tempWolfActor->SetPosition(Vector2(41, 26));
                                    }
                                }

                                else
                                {
                                    leftRaftAnimal[1] = 0;

                                    if (tempWolfActor != nullptr)
                                    {
                                        tempWolfActor->SetPosition(Vector2(54, 26));
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

                                if (leftRaftAnimal[0] == -1)
                                {
                                    leftRaftAnimal[0] = 0;

                                    if (tempChickActor != nullptr)
                                    {
                                        tempChickActor->SetPosition(Vector2(41, 26));
                                    }
                                }

                                else
                                {
                                    leftRaftAnimal[1] = 0;

                                    if (tempChickActor != nullptr)
                                    {
                                        tempChickActor->SetPosition(Vector2(54, 26));
                                    }
                                }
                            }
                        }
                    }

                    // Select animal on raft
                    if (leftRaftAnimal[0] != -1 &&
                        relativeX / 8 >= 41 && relativeX / 8 <= 48 &&
                        relativeY / 17 >= 25 && relativeY / 17 <= 26)
                    {
                        leftRaftAnimal[0] = -1;

                        Animal* tempAnimalActor = nullptr;

                        for (Actor* actor : actors)
                        {
                            tempAnimalActor = dynamic_cast<Animal*>(actor);

                            if (tempAnimalActor != nullptr &&
                                actor->GetPosition() == Vector2(41, 26))
                            {
                                for (int idx = 0; idx < 3; ++idx)
                                {
                                    if (dynamic_cast<Wolf*>(tempAnimalActor) != nullptr)
                                    {
                                        if (!leftAnimals[idx])
                                        {
                                            leftAnimals[idx] = true;
                                            actor->SetPosition(Vector2(3 + (idx * 8), 23));
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        if (!leftAnimals[idx + 3])
                                        {
                                            leftAnimals[idx + 3] = true;
                                            actor->SetPosition(Vector2(3 + (idx * 8), 26));
                                            break;
                                        }
                                    }
                                    
                                }
                                break;
                            }
                        }
                    }

                    else if (leftRaftAnimal[1] != -1 &&
                        relativeX / 8 >= 54 && relativeX / 8 <= 62 &&
                        relativeY / 17 >= 25 && relativeY / 17 <= 26)
                    {
                        leftRaftAnimal[1] = -1;

                        Animal* tempAnimalActor = nullptr;

                        for (Actor* actor : actors)
                        {
                            tempAnimalActor = dynamic_cast<Animal*>(actor);

                            if (tempAnimalActor != nullptr &&
                                actor->GetPosition() == Vector2(54, 26))
                            {
                                for (int idx = 0; idx < 3; ++idx)
                                {
                                    if (dynamic_cast<Wolf*>(tempAnimalActor) != nullptr)
                                    {
                                        if (!leftAnimals[idx])
                                        {
                                            leftAnimals[idx] = true;
                                            actor->SetPosition(Vector2(3 + (idx * 8), 23));
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        if (!leftAnimals[idx + 3])
                                        {
                                            leftAnimals[idx + 3] = true;
                                            actor->SetPosition(Vector2(3 + (idx * 8), 26));
                                            break;
                                        }
                                    } 
                                }
                                break;
                            }
                        }
                    }

                } 
                ProcessAddedAndDestroyedActor();
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
        std::string raft = "**************************";
        for (int idx = 0; idx < 9; ++idx)
        {
            if (idx == 4)
            {
                if (!isRaftEmpty())
                {
                    if (isRaftLeft)
                    {
                        if (isRaftFull())
                        {
                            Engine::Get().Render(Vector2(36, 26), "*****", Color::Yellow);
                            Engine::Get().Render(Vector2(48, 26), "******", Color::Yellow);
                            Engine::Get().Render(Vector2(61, 26), "*****", Color::Yellow);
                            raft += '*';
                            continue;
                        }
                        else if (leftRaftAnimal[0] != -1)
                        {
                            Engine::Get().Render(Vector2(36, 26), "*****", Color::Yellow);
                            Engine::Get().Render(Vector2(48, 26), "******************", Color::Yellow);
                            raft += '*';
                            continue;
                        }
                        else
                        {
                            Engine::Get().Render(Vector2(36, 26), "******************", Color::Yellow);
                            Engine::Get().Render(Vector2(61, 26), "*****", Color::Yellow);
                            raft += '*';
                            continue;
                        }
                    }
                    
                }
            }
            Engine::Get().Render(Vector2(44 - (2 * idx), 22 + idx), raft.c_str(), Color::Yellow);
            raft += '*';
        }
    }
}

bool GameplayLevel::isRaftEmpty() const
{
    if (isRaftLeft &&
        leftRaftAnimal[0] == -1 && leftRaftAnimal[1] == -1)
    {
        return true;
    }
    else if (!isRaftLeft &&
        rightRaftAnimal[0] == -1 && rightRaftAnimal[1] == -1)
    {
        return true;
    }

    return false;
}

bool GameplayLevel::isRaftFull() const
{
    if (isRaftLeft &&
        leftRaftAnimal[0] != -1 && leftRaftAnimal[1] != -1)
    {
        return true;
    }

    else if (!isRaftLeft &&
        rightRaftAnimal[0] != -1 && rightRaftAnimal[1] != -1)
    {
        return true;
    }

    return false;
}
