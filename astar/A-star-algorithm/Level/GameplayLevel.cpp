#include "GameplayLevel.h"

GameplayLevel::GameplayLevel()
    : mousePos{ 0, 0 }, consoleRect{ 0, 0, 0, 0 },
    leftAnimals{ true, true, true, true, true, true }, rightAnimals{ false, false, false, false, false, false },
    RaftAnimal{ -1, -1 }, isRaftLeft(true)
{
    AddActor(new Text(puzzleString_01.c_str(), Vector2(2, 1)));
    AddActor(new Text(puzzleString_02.c_str(), Vector2(4, 3)));
    AddActor(new Text(puzzleString_03.c_str(), Vector2(4, 4)));
    AddActor(new Text(puzzleString_04.c_str(), Vector2(4, 5)));
    AddActor(new Text(puzzleString_05.c_str(), Vector2(2, 7)));

    A_Star_Solution(3, 3, true);

    puzzle_least_count = least_count;
    last_least_count = least_count;

    if (puzzle_least_count == 0)
    {
        AddActor(new Text(std::to_string(puzzle_least_count).c_str(), Vector2(35, 7)));
        AddActor(new Text("회", Vector2(36, 7)));
    }
    else if (puzzle_least_count == -1)
    {
        AddActor(new Text("무한대", Vector2(35, 7)));
    }
    else
    {
        AddActor(new Text(std::to_string(puzzle_least_count).c_str(), Vector2(35, 7)));
        AddActor(new Text("회", Vector2(36 + (int)log10(puzzle_least_count), 7)));
    }

    AddActor(new Text("COUNT:", Vector2(2, 12), Color::Red));
    countText = new Text("0", Vector2(9, 12), Color::Red);
    AddActor(countText);

    AddActor(new ResetButton(Vector2(136, 1)));

    AddActor(new Wolf(Vector2(3, 23)));
    AddActor(new Wolf(Vector2(11, 23)));
    AddActor(new Wolf(Vector2(19, 23)));

    AddActor(new Chick(Vector2(3, 26)));
    AddActor(new Chick(Vector2(11, 26)));
    AddActor(new Chick(Vector2(19, 26)));

    // Add raft actor in left side
    int raft_length = 26;
    for (int idx = 0; idx < 9; ++idx)
    {
        if (idx == 4)
        {
            AddActor(new Raft(5, Vector2(36, 26)));
            AddActor(new Raft(6, Vector2(48, 26)));
            AddActor(new Raft(5, Vector2(61, 26)));

            AddActor(new Raft(7, Vector2(41, 26)));
            AddActor(new Raft(7, Vector2(54, 26)));

            ++raft_length;
            continue;
        }
        AddActor(new Raft(raft_length, Vector2(44 - (2 * idx), 22 + idx)));
        ++raft_length;
    }

    ProcessAddedAndDestroyedActor();
}

GameplayLevel::~GameplayLevel()
{
}

void GameplayLevel::Update(float deltaTime)
{
    Super::Update(deltaTime);

    GetWindowRect(consoleWindow, &consoleRect);

    if (isWin || isGameOver)
    {
        if (isWin)
        {
            AddActor(new Text("YOU WIN!!", Vector2(80, 12), Color::Green));
        }
        else
        {
            // Game Over logic
            if (!isRenderedGameOverMessage)
            {
                AddActor(new Text("GAME OVER", Vector2(80, 12), Color::Red));
            }
        }

        isRenderedGameOverMessage = true;

        ProcessAddedAndDestroyedActor();

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
                    if (relativeX / 8 >= 136 && relativeX / 8 <= 141 &&
                        relativeY / 17 >= 0 && relativeY / 17 <= 1)
                    {
                        ResetGame();
                    }
                }
            }
        }
    }

    else
    {
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
                    // Clicked RESET Button
                    if (relativeX / 8 >= 136 && relativeX / 8 <= 141 &&
                        relativeY / 17 >= 0 && relativeY / 17 <= 1)
                    {
                        ResetGame();
                    }
                    
                    // Raft : Left side
                    if (isRaftLeft)
                    {
                        // Move Raft (Left -->> Right) Animation
                        for (int idx = 0; idx < 9; ++idx)
                        {
                            if (relativeX / 8 >= 43 - (idx * 2) && relativeX / 8 <= 70 - idx &&
                                relativeY / 17 >= 21 + idx && relativeY / 17 < 22 + idx
                                && !isRaftEmpty())
                            {
                                if (!(RaftAnimal[0] != -1 &&
                                    relativeX / 8 >= 41 && relativeX / 8 <= 48 &&
                                    relativeY / 17 >= 25 && relativeY / 17 <= 26)
                                    && !(RaftAnimal[1] != -1 &&
                                        relativeX / 8 >= 54 && relativeX / 8 <= 62 &&
                                        relativeY / 17 >= 25 && relativeY / 17 <= 26))
                                {
                                    isRaftMoving = true;
                                    ++count;
                                    countText->SetImage(std::to_string(count).c_str());
                                }
                            }
                        }
                    }
                    // Raft : Right side
                    else
                    {
                        // Move Raft (Left <<-- Right) Animation
                        for (int idx = 0; idx < 9; ++idx)
                        {
                            if (relativeX / 8 >= 74 + (idx * 2) && relativeX / 8 <= 101 + idx &&
                                relativeY / 17 >= 21 + idx && relativeY / 17 < 22 + idx
                                && !isRaftEmpty())
                            {
                                if (!(RaftAnimal[0] != -1 &&
                                    relativeX / 8 >= 84 && relativeX / 8 <= 91 &&
                                    relativeY / 17 >= 25 && relativeY / 17 <= 26)
                                    && !(RaftAnimal[1] != -1 &&
                                        relativeX / 8 >= 97 && relativeX / 8 <= 105 &&
                                        relativeY / 17 >= 25 && relativeY / 17 <= 26))
                                {
                                    isRaftMoving = true;
                                    ++count;
                                    countText->SetImage(std::to_string(count).c_str());
                                }
                            }
                        }
                    }

                    // Raft : Left side
                    if (isRaftLeft)
                    {
                        if (!isRaftFull())
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

                                    if (RaftAnimal[0] == -1)
                                    {
                                        RaftAnimal[0] = 0;

                                        if (tempWolfActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(41, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

                                            tempWolfActor->SetPosition(Vector2(41, 26));
                                        }
                                    }

                                    else
                                    {
                                        RaftAnimal[1] = 0;

                                        if (tempWolfActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(54, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

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

                                    if (RaftAnimal[0] == -1)
                                    {
                                        RaftAnimal[0] = 1;

                                        if (tempChickActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(41, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

                                            tempChickActor->SetPosition(Vector2(41, 26));
                                        }
                                    }

                                    else
                                    {
                                        RaftAnimal[1] = 1;

                                        if (tempChickActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(54, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

                                            tempChickActor->SetPosition(Vector2(54, 26));
                                        }
                                    }
                                }
                            }
                        }

                        // Select animal on raft
                        if (RaftAnimal[0] != -1 &&
                            relativeX / 8 >= 41 && relativeX / 8 <= 48 &&
                            relativeY / 17 >= 25 && relativeY / 17 <= 26)
                        {
                            RaftAnimal[0] = -1;

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
                                                AddActor(new Raft(7, Vector2(41, 26)));
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            if (!leftAnimals[idx + 3])
                                            {
                                                leftAnimals[idx + 3] = true;
                                                actor->SetPosition(Vector2(3 + (idx * 8), 26));
                                                AddActor(new Raft(7, Vector2(41, 26)));
                                                break;
                                            }
                                        }

                                    }
                                    break;
                                }
                            }
                        }

                        else if (RaftAnimal[1] != -1 &&
                            relativeX / 8 >= 54 && relativeX / 8 <= 62 &&
                            relativeY / 17 >= 25 && relativeY / 17 <= 26)
                        {
                            RaftAnimal[1] = -1;

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
                                                AddActor(new Raft(7, Vector2(54, 26)));
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            if (!leftAnimals[idx + 3])
                                            {
                                                leftAnimals[idx + 3] = true;
                                                actor->SetPosition(Vector2(3 + (idx * 8), 26));
                                                AddActor(new Raft(7, Vector2(54, 26)));
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }

                    // Raft : Right side
                    else
                    {
                        if (!isRaftFull())
                        {
                            for (int idx = 0; idx < 3; ++idx)
                            {
                                // Wolf selected
                                if (rightAnimals[idx] &&
                                    relativeX / 8 >= 115 + (idx * 8) && relativeX / 8 <= 121 + (idx * 8) &&
                                    relativeY / 17 >= 22 && relativeY / 17 <= 23)
                                {
                                    Wolf* tempWolfActor = nullptr;

                                    for (Actor* actor : actors)
                                    {
                                        tempWolfActor = dynamic_cast<Wolf*>(actor);
                                        if (tempWolfActor != nullptr &&
                                            actor->GetPosition() == Vector2(115 + (8 * idx), 23))
                                        {
                                            break;
                                        }
                                    }

                                    rightAnimals[idx] = false;

                                    if (RaftAnimal[0] == -1)
                                    {
                                        RaftAnimal[0] = 0;

                                        if (tempWolfActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(84, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

                                            tempWolfActor->SetPosition(Vector2(84, 26));
                                        }
                                    }

                                    else
                                    {
                                        RaftAnimal[1] = 0;

                                        if (tempWolfActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(97, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

                                            tempWolfActor->SetPosition(Vector2(97, 26));
                                        }
                                    }
                                }

                                // Chick selected
                                else if (rightAnimals[idx + 3] &&
                                    relativeX / 8 >= 115 + (idx * 8) && relativeX / 8 <= 121 + (idx * 8) &&
                                    relativeY / 17 >= 25 && relativeY / 17 <= 26)
                                {
                                    Chick* tempChickActor = nullptr;

                                    for (Actor* actor : actors)
                                    {
                                        tempChickActor = dynamic_cast<Chick*>(actor);
                                        if (tempChickActor != nullptr &&
                                            actor->GetPosition() == Vector2(115 + (8 * idx), 26))
                                        {
                                            break;
                                        }
                                    }

                                    rightAnimals[idx + 3] = false;

                                    if (RaftAnimal[0] == -1)
                                    {
                                        RaftAnimal[0] = 1;

                                        if (tempChickActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(84, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

                                            tempChickActor->SetPosition(Vector2(84, 26));
                                        }
                                    }

                                    else
                                    {
                                        RaftAnimal[1] = 1;

                                        if (tempChickActor != nullptr)
                                        {
                                            Raft* tempRaftActor = nullptr;

                                            for (Actor* actor : actors)
                                            {
                                                tempRaftActor = dynamic_cast<Raft*>(actor);
                                                if (tempRaftActor != nullptr &&
                                                    tempRaftActor->GetPosition() == Vector2(97, 26))
                                                {
                                                    tempRaftActor->Destroy();
                                                }
                                            }

                                            tempChickActor->SetPosition(Vector2(97, 26));
                                        }
                                    }
                                }
                            }
                        }

                        // Select animal on raft
                        if (RaftAnimal[0] != -1 &&
                            relativeX / 8 >= 84 && relativeX / 8 <= 91 &&
                            relativeY / 17 >= 25 && relativeY / 17 <= 26)
                        {
                            RaftAnimal[0] = -1;

                            Animal* tempAnimalActor = nullptr;

                            for (Actor* actor : actors)
                            {
                                tempAnimalActor = dynamic_cast<Animal*>(actor);

                                if (tempAnimalActor != nullptr &&
                                    actor->GetPosition() == Vector2(84, 26))
                                {
                                    for (int idx = 0; idx < 3; ++idx)
                                    {
                                        if (dynamic_cast<Wolf*>(tempAnimalActor) != nullptr)
                                        {
                                            if (!rightAnimals[idx])
                                            {
                                                rightAnimals[idx] = true;
                                                actor->SetPosition(Vector2(115 + (idx * 8), 23));
                                                AddActor(new Raft(7, Vector2(84, 26)));
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            if (!rightAnimals[idx + 3])
                                            {
                                                rightAnimals[idx + 3] = true;
                                                actor->SetPosition(Vector2(115 + (idx * 8), 26));
                                                AddActor(new Raft(7, Vector2(84, 26)));
                                                break;
                                            }
                                        }

                                    }
                                    break;
                                }
                            }
                        }

                        else if (RaftAnimal[1] != -1 &&
                            relativeX / 8 >= 97 && relativeX / 8 <= 105 &&
                            relativeY / 17 >= 25 && relativeY / 17 <= 26)
                        {
                            RaftAnimal[1] = -1;

                            Animal* tempAnimalActor = nullptr;

                            for (Actor* actor : actors)
                            {
                                tempAnimalActor = dynamic_cast<Animal*>(actor);

                                if (tempAnimalActor != nullptr &&
                                    actor->GetPosition() == Vector2(97, 26))
                                {
                                    for (int idx = 0; idx < 3; ++idx)
                                    {
                                        if (dynamic_cast<Wolf*>(tempAnimalActor) != nullptr)
                                        {
                                            if (!rightAnimals[idx])
                                            {
                                                rightAnimals[idx] = true;
                                                actor->SetPosition(Vector2(115 + (idx * 8), 23));
                                                AddActor(new Raft(7, Vector2(97, 26)));
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            if (!rightAnimals[idx + 3])
                                            {
                                                rightAnimals[idx + 3] = true;
                                                actor->SetPosition(Vector2(115 + (idx * 8), 26));
                                                AddActor(new Raft(7, Vector2(97, 26)));
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (isRaftMoving)
        {
            Raft* tempRaftActor = nullptr;
            Animal* tempAnimalActor = nullptr;

            if (movingIndex <= 15 ||
                (movingIndex >= 40 && movingIndex < 55))
            {
                Sleep(4);
                for (Actor* actor : actors)
                {
                    tempRaftActor = dynamic_cast<Raft*>(actor);
                    tempAnimalActor = dynamic_cast<Animal*>(actor);

                    if (tempRaftActor != nullptr)
                    {
                        if (tempRaftActor->GetPosition().xpos >= 28 && tempRaftActor->GetPosition().xpos < 105)
                        {
                            if (isRaftLeft)
                            {
                                tempRaftActor->SetPosition(
                                    Vector2(tempRaftActor->GetPosition().xpos + 1,
                                        tempRaftActor->GetPosition().ypos));
                            }
                            else
                            {
                                tempRaftActor->SetPosition(
                                    Vector2(tempRaftActor->GetPosition().xpos - 1,
                                        tempRaftActor->GetPosition().ypos));
                            }
                        }
                    }
                    else if (tempAnimalActor != nullptr)
                    {
                        if (tempAnimalActor->GetPosition().xpos >= 28 && tempAnimalActor->GetPosition().xpos < 105)
                        {
                            if (isRaftLeft)
                            {
                                tempAnimalActor->SetPosition(
                                    Vector2(tempAnimalActor->GetPosition().xpos + 1,
                                        tempAnimalActor->GetPosition().ypos));
                            }
                            else
                            {
                                tempAnimalActor->SetPosition(
                                    Vector2(tempAnimalActor->GetPosition().xpos - 1,
                                        tempAnimalActor->GetPosition().ypos));
                            };
                        }
                    }
                }
            }

            for (int idx = 0; idx < 3; ++idx)
            {
                if (movingIndex >= 16 + (idx * 8) && movingIndex <= 23 + (idx * 8))
                {
                    //Sleep(1);
                    int slideIndex = movingIndex - 16 - (idx * 8);
                    for (Actor* actor : actors)
                    {
                        tempRaftActor = dynamic_cast<Raft*>(actor);
                        tempAnimalActor = dynamic_cast<Animal*>(actor);
                        if (tempRaftActor != nullptr)
                        {
                            if (tempRaftActor->GetPosition().xpos >= 28 && tempRaftActor->GetPosition().xpos < 105 &&
                                tempRaftActor->GetPosition().ypos > 22 + slideIndex)
                            {
                                if (isRaftLeft)
                                {
                                    tempRaftActor->SetPosition(
                                        Vector2(tempRaftActor->GetPosition().xpos + 1,
                                            tempRaftActor->GetPosition().ypos));
                                }
                                else
                                {
                                    tempRaftActor->SetPosition(
                                        Vector2(tempRaftActor->GetPosition().xpos - 1,
                                            tempRaftActor->GetPosition().ypos));
                                }
                            }
                        }
                        else if (tempAnimalActor != nullptr)
                        {
                            if (tempAnimalActor->GetPosition().xpos >= 28 && tempAnimalActor->GetPosition().xpos < 105 &&
                                tempAnimalActor->GetPosition().ypos > 22 + slideIndex)
                            {
                                if (isRaftLeft)
                                {
                                    tempAnimalActor->SetPosition(
                                        Vector2(tempAnimalActor->GetPosition().xpos + 1,
                                            tempAnimalActor->GetPosition().ypos));
                                }
                                else
                                {
                                    tempAnimalActor->SetPosition(
                                        Vector2(tempAnimalActor->GetPosition().xpos - 1,
                                            tempAnimalActor->GetPosition().ypos));
                                }
                            }
                        }
                    }
                }
            }

            ++movingIndex;

            if (movingIndex > 55)
            {
                movingIndex = 0;
                isRaftMoving = false;

                // Check IsBestMove
                int tempRaftWolf = 0;
                int tempRaftChick = 0;
                for (int idx = 0; idx < 2; ++idx)
                {
                    if (RaftAnimal[idx] == 0)
                    {
                        ++tempRaftWolf;
                    }
                    else if (RaftAnimal[idx] == 1)
                    {
                        ++tempRaftChick;
                    }
                }

                int currentLeftWolf = 0;
                int currentLeftChick = 0;

                for (int idx = 0; idx < 3; ++idx)
                {
                    if (leftAnimals[idx])
                    {
                        ++currentLeftWolf;
                    }
                    if (leftAnimals[idx + 3])
                    {
                        ++currentLeftChick;
                    }
                }

                if (isRaftLeft)
                {
                    A_Star_Solution(currentLeftWolf + tempRaftWolf, currentLeftChick + tempRaftChick, isRaftLeft);
                }
                else
                {
                    A_Star_Solution(currentLeftWolf, currentLeftChick, isRaftLeft);
                }

                last_least_count = least_count;

                if (isRaftLeft)
                {
                    A_Star_Solution(currentLeftWolf, currentLeftChick, !isRaftLeft);
                }
                else
                {
                    A_Star_Solution(currentLeftWolf + tempRaftWolf, currentLeftChick + tempRaftChick, !isRaftLeft);
                }

                if (last_least_count == least_count + 1)
                {
                    AddActor(new Text("V", Vector2((2 + (count - 1) * 2), 14), Color::Green));
                }
                else
                {
                    AddActor(new Text("X", Vector2((2 + (count - 1) * 2), 14), Color::White));
                }

                // Unload animals
                if (RaftAnimal[0] == 0)
                {
                    for (int idx = 0; idx < 3; ++idx)
                    {
                        if (isRaftLeft)
                        {
                            if (!rightAnimals[idx])
                            {
                                Wolf* tempWolfActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempWolfActor = dynamic_cast<Wolf*>(actor);
                                    if (tempWolfActor != nullptr &&
                                        actor->GetPosition() == Vector2(84, 26))
                                    {
                                        actor->SetPosition(Vector2(115 + (idx * 8), 23));
                                        break;
                                    }
                                }

                                RaftAnimal[0] = -1;
                                rightAnimals[idx] = true;

                                AddActor(new Raft(7, Vector2(84, 26)));
                                break;
                            }
                        }

                        else
                        {
                            if (!leftAnimals[idx])
                            {
                                Wolf* tempWolfActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempWolfActor = dynamic_cast<Wolf*>(actor);
                                    if (tempWolfActor != nullptr &&
                                        actor->GetPosition() == Vector2(41, 26))
                                    {
                                        actor->SetPosition(Vector2(3 + (idx * 8), 23));
                                        break;
                                    }
                                }

                                RaftAnimal[0] = -1;
                                leftAnimals[idx] = true;

                                AddActor(new Raft(7, Vector2(41, 26)));
                                break;
                            }
                        }
                    }
                }
                else if (RaftAnimal[0] == 1)
                {
                    for (int idx = 0; idx < 3; ++idx)
                    {
                        if (isRaftLeft)
                        {
                            if (!rightAnimals[idx + 3])
                            {
                                Chick* tempChickActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempChickActor = dynamic_cast<Chick*>(actor);
                                    if (tempChickActor != nullptr &&
                                        actor->GetPosition() == Vector2(84, 26))
                                    {
                                        actor->SetPosition(Vector2(115 + (idx * 8), 26));
                                        break;
                                    }
                                }

                                RaftAnimal[0] = -1;
                                rightAnimals[idx + 3] = true;

                                AddActor(new Raft(7, Vector2(84, 26)));
                                break;
                            }
                        }
                        else
                        {
                            if (!leftAnimals[idx + 3])
                            {
                                Chick* tempChickActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempChickActor = dynamic_cast<Chick*>(actor);
                                    if (tempChickActor != nullptr &&
                                        actor->GetPosition() == Vector2(41, 26))
                                    {
                                        actor->SetPosition(Vector2(3 + (idx * 8), 26));
                                        break;
                                    }
                                }

                                RaftAnimal[0] = -1;
                                leftAnimals[idx + 3] = true;

                                AddActor(new Raft(7, Vector2(41, 26)));
                                break;
                            }
                        }
                    }
                }

                if (RaftAnimal[1] == 0)
                {
                    for (int idx = 0; idx < 3; ++idx)
                    {
                        if (isRaftLeft)
                        {
                            if (!rightAnimals[idx])
                            {
                                Wolf* tempWolfActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempWolfActor = dynamic_cast<Wolf*>(actor);
                                    if (tempWolfActor != nullptr &&
                                        actor->GetPosition() == Vector2(97, 26))
                                    {
                                        actor->SetPosition(Vector2(115 + (idx * 8), 23));
                                        break;
                                    }
                                }

                                RaftAnimal[1] = -1;
                                rightAnimals[idx] = true;

                                AddActor(new Raft(7, Vector2(97, 26)));
                                break;
                            }
                        }
                        else
                        {
                            if (!leftAnimals[idx])
                            {
                                Wolf* tempWolfActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempWolfActor = dynamic_cast<Wolf*>(actor);
                                    if (tempWolfActor != nullptr &&
                                        actor->GetPosition() == Vector2(54, 26))
                                    {
                                        actor->SetPosition(Vector2(3 + (idx * 8), 23));
                                        break;
                                    }
                                }

                                RaftAnimal[1] = -1;
                                leftAnimals[idx] = true;

                                AddActor(new Raft(7, Vector2(54, 26)));
                                break;
                            }
                        }
                    }
                }
                else if (RaftAnimal[1] == 1)
                {
                    for (int idx = 0; idx < 3; ++idx)
                    {
                        if (isRaftLeft)
                        {
                            if (!rightAnimals[idx + 3])
                            {
                                Chick* tempChickActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempChickActor = dynamic_cast<Chick*>(actor);
                                    if (tempChickActor != nullptr &&
                                        actor->GetPosition() == Vector2(97, 26))
                                    {
                                        actor->SetPosition(Vector2(115 + (idx * 8), 26));
                                        break;
                                    }
                                }

                                RaftAnimal[1] = -1;
                                rightAnimals[idx + 3] = true;

                                AddActor(new Raft(7, Vector2(97, 26)));
                                break;
                            }
                        }
                        else
                        {
                            if (!leftAnimals[idx + 3])
                            {
                                Chick* tempChickActor = nullptr;

                                for (Actor* actor : actors)
                                {
                                    tempChickActor = dynamic_cast<Chick*>(actor);
                                    if (tempChickActor != nullptr &&
                                        actor->GetPosition() == Vector2(54, 26))
                                    {
                                        actor->SetPosition(Vector2(3 + (idx * 8), 26));
                                        break;
                                    }
                                }

                                RaftAnimal[1] = -1;
                                leftAnimals[idx + 3] = true;

                                AddActor(new Raft(7, Vector2(54, 26)));
                                break;
                            }
                        }
                    }
                }
                isRaftLeft = !isRaftLeft;

                // Check gameover
                int wolvesCount = 0;
                int chicksCount = 0;
                for (int idx = 0; idx < 3; ++idx)
                {
                    if (leftAnimals[idx])
                    {
                        ++wolvesCount;
                    }
                    if (leftAnimals[idx + 3])
                    {
                        ++chicksCount;
                    }
                }

                if (wolvesCount == 0 && chicksCount == 0)
                {
                    isWin = true;
                }

                if (wolvesCount > chicksCount && chicksCount != 0)
                {
                    Chick* tempChickActor = nullptr;
                    for (int idx = 0; idx < 3; ++idx)
                    {
                        for (Actor* actor : actors)
                        {
                            tempChickActor = dynamic_cast<Chick*>(actor);
                            if (tempChickActor != nullptr &&
                                tempChickActor->GetPosition() == Vector2(3 + (idx * 8), 26))
                            {
                                tempChickActor->SetColor(Color::Red);
                            }
                        }
                    }
                    isGameOver = true;
                }

                wolvesCount = 0;
                chicksCount = 0;
                for (int idx = 0; idx < 3; ++idx)
                {
                    if (rightAnimals[idx])
                    {
                        ++wolvesCount;
                    }
                    if (rightAnimals[idx + 3])
                    {
                        ++chicksCount;
                    }
                }

                if (wolvesCount > chicksCount && chicksCount != 0)
                {
                    Chick* tempChickActor = nullptr;
                    for (int idx = 0; idx < 3; ++idx)
                    {
                        for (Actor* actor : actors)
                        {
                            tempChickActor = dynamic_cast<Chick*>(actor);
                            if (tempChickActor != nullptr &&
                                tempChickActor->GetPosition() == Vector2(115 + (idx * 8), 26))
                            {
                                tempChickActor->SetColor(Color::Red);
                            }
                        }
                    }
                    isGameOver = true;
                }
            }
        }
    }
    ProcessAddedAndDestroyedActor();
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
}

bool GameplayLevel::isRaftEmpty() const
{
    if (isRaftLeft &&
        RaftAnimal[0] == -1 && RaftAnimal[1] == -1)
    {
        return true;
    }

    return false;
}

bool GameplayLevel::isRaftFull() const
{
    if (isRaftLeft &&
        RaftAnimal[0] != -1 && RaftAnimal[1] != -1)
    {
        return true;
    }

    return false;
}

void GameplayLevel::ResetGame()
{
    for (Actor* actor : actors)
    {
        actor->Destroy();
    }

    AddActor(new Text(puzzleString_01.c_str(), Vector2(2, 1)));
    AddActor(new Text(puzzleString_02.c_str(), Vector2(4, 3)));
    AddActor(new Text(puzzleString_03.c_str(), Vector2(4, 4)));
    AddActor(new Text(puzzleString_04.c_str(), Vector2(4, 5)));
    AddActor(new Text(puzzleString_05.c_str(), Vector2(2, 7)));

    A_Star_Solution(3, 3, true);

    puzzle_least_count = least_count;
    last_least_count = least_count;

    if (puzzle_least_count == 0)
    {
        AddActor(new Text(std::to_string(puzzle_least_count).c_str(), Vector2(35, 7)));
        AddActor(new Text("회", Vector2(36, 7)));
    }
    else if (puzzle_least_count == -1)
    {
        AddActor(new Text("무한대", Vector2(35, 7)));
    }
    else
    {
        AddActor(new Text(std::to_string(puzzle_least_count).c_str(), Vector2(35, 7)));
        AddActor(new Text("회", Vector2(36 + (int)log10(puzzle_least_count), 7)));
    }

    AddActor(new Text("COUNT:", Vector2(2, 12), Color::Red));
    countText = new Text("0", Vector2(9, 12), Color::Red);
    AddActor(countText);

    AddActor(new ResetButton(Vector2(136, 1)));

    AddActor(new Wolf(Vector2(3, 23)));
    AddActor(new Wolf(Vector2(11, 23)));
    AddActor(new Wolf(Vector2(19, 23)));

    AddActor(new Chick(Vector2(3, 26)));
    AddActor(new Chick(Vector2(11, 26)));
    AddActor(new Chick(Vector2(19, 26)));

    // Add raft actor in left side
    int raft_length = 26;
    for (int idx = 0; idx < 9; ++idx)
    {
        if (idx == 4)
        {
            AddActor(new Raft(5, Vector2(36, 26)));
            AddActor(new Raft(6, Vector2(48, 26)));
            AddActor(new Raft(5, Vector2(61, 26)));

            AddActor(new Raft(7, Vector2(41, 26)));
            AddActor(new Raft(7, Vector2(54, 26)));

            ++raft_length;
            continue;
        }
        AddActor(new Raft(raft_length, Vector2(44 - (2 * idx), 22 + idx)));
        ++raft_length;
    }

    isGameOver = false;
    isWin = false;
    isRaftLeft = true;
    count = 0;
    for (int idx = 0; idx < 6; ++idx)
    {
        leftAnimals[idx] = true;
        rightAnimals[idx] = false;
    }
    for (int idx = 0; idx < 2; ++idx)
    {
        RaftAnimal[idx] = -1;
    }

    isRenderedGameOverMessage = false;
    
    ProcessAddedAndDestroyedActor();
}

// Function using the A-star algorithm to find the optimal movement history and number of movements
void GameplayLevel::A_Star_Solution(int currentWolf, int currentChick, bool isCurrentRaftLeft)
{
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    std::set<std::tuple<int, int, char>> visited;
    
    // Start state
    State start = { currentWolf, currentChick, isCurrentRaftLeft, 0, {} };

    // Target state
    State target = { 0, 0, false, 0, {} };

    pq.push(start);

    while (!pq.empty()) {
        // Getting the best status out of the priority queue(pq)
        State current = pq.top();
        pq.pop();

        // Check if the element has been visited before
        if (visited.count({ current.wolf, current.chick, current.isRaftLeft }))
        {
            continue;
        }
        // Case where the element has never been visited
        visited.insert({ current.wolf, current.chick, current.isRaftLeft });

        // Reaching the goal
        if (current.wolf == target.wolf && current.chick == target.chick && current.isRaftLeft == target.isRaftLeft) {
            least_count = current.cost;
            moves = current.move_history;
            return;
        }

        // Explore possible movements
        for (auto& move : moveAnimals) 
        {
            // Number of Wolves Updated
            int newWolf = current.isRaftLeft ? (current.wolf - move.first) : (current.wolf + move.first);
            // Number of Chicks Updated
            int newChick = current.isRaftLeft ? (current.chick - move.second) : (current.chick + move.second);
            bool newRaft = !current.isRaftLeft;

            if (newWolf >= 0 && newWolf <= 3 && newChick >= 0 && newChick <= 3 && isValidState(newWolf, newChick)) {
                State next = { newWolf, newChick, newRaft, current.cost + 1, current.move_history };
                next.move_history.push_back(move);
                pq.push(next);
            }
        }
    }

    least_count = -1;
    moves = {};
}

// Using the sum of the number of wolves and chicks left as a heuristic function
// The greater the number of wolves and chicks remaining, the more times they have to move
// The smaller the number of wolves and chicks remaining on the left side, the closer they are to the target status
int GameplayLevel::Heuristic(int wolf, int chick) {
    return wolf + chick;
}

// Function to verify that current state is in a valid state
bool GameplayLevel::isValidState(int wolf, int chick) {
    // Case where the number of wolves left on the left side is greater than the number of chicks left on the same side
    if (wolf > chick && chick > 0)
    {
        return false;
    }

    // Case where the number of wolves left on the right side is greater than the number of chicks left on the same side
    if ((3 - wolf) > (3 - chick) && (3 - chick) > 0) 
    {
        return false;
    }

    return true;
}
