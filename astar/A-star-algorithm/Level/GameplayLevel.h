#pragma once

#include "Engine/Engine.h"

#include "Level/Level.h"

#include "Actor/Text/Text.h"
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
	POINT mousePos;

	HWND consoleWindow = GetConsoleWindow();
	RECT consoleRect;

	Vector2 selectedBoardCoord;

	std::string puzzleString_01 = "����� ���Ƹ� �� 3������ ��� �� �ǳ������� �ű�� �ʹ�. �׷��� �¸��� ������ ������ ���� ������ �ִ�.";
	std::string puzzleString_02 = "* �¸񿡴� �� ���� 2������ Ż �� �ִ�.";
	std::string puzzleString_03 = "* 1���� �̻� Ÿ�� �־�� �����δ�.";
	std::string puzzleString_04 = "* ���� �⽾ ��� ���밡 ���Ƹ����� �������� ���Ƹ��� ��Ƹ����� ����.";
	std::string puzzleString_05 = "Ƚ���� ������ ������ �ִ� Ƚ���� 11ȸ";

	bool isRaftLeft = true;
};