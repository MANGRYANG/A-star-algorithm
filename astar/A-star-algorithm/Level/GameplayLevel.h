#pragma once

#include "Engine/Engine.h"

#include "Level/Level.h"

#include "Actor/Text/Text.h"
#include "Actor/Raft/Raft.h"
#include "Actor/Animal/Wolf.h"
#include "Actor/Animal/Chick.h"

#include "Math/Vector2.h"

#include <conio.h>
#include <chrono>
#include <thread>
#include <string>

class GameplayLevel : public Level
{
	// RTTI Declarations
	RTTI_DECLARATIONS(GameplayLevel, Level)

public:
	GameplayLevel();
	~GameplayLevel();

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	bool isRaftEmpty() const;
	bool isRaftFull() const;

protected:
	POINT mousePos;

	HWND consoleWindow = GetConsoleWindow();
	RECT consoleRect;

	Vector2 selectedBoardCoord;

	std::string puzzleString_01 = "늑대와 병아리 각 3마리를 모두 강 건너편으로 옮기고 싶다. 그런데 뗏목을 쓰려면 다음과 같은 조건이 있다.";
	std::string puzzleString_02 = "* 뗏목에는 한 번에 2마리만 탈 수 있다.";
	std::string puzzleString_03 = "* 1마리 이상 타고 있어야 움직인다.";
	std::string puzzleString_04 = "* 양쪽 기슭 모두 늑대가 병아리보다 많아지면 병아리는 잡아먹히고 만다.";
	std::string puzzleString_05 = "횟수에 제한은 없으나 최단 횟수는 11회";

	bool isRaftLeft;
	bool isRaftMoving = false;

	bool leftAnimals[6];
	bool rightAnimals[6];

	int RaftAnimal[2];

	int movingIndex = 0;

	int count = 0;

};