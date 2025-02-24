#include "PCH.h"
#include "Vector2.h"

Vector2::Vector2(int xpos, int ypos)
    : xpos(xpos), ypos(ypos) {
}

Vector2 Vector2::operator+(const Vector2& other)
{
    return Vector2(xpos + other.xpos, ypos + other.ypos);
}

Vector2 Vector2::operator-(const Vector2& other)
{
    return Vector2(xpos - other.xpos, ypos - other.ypos);
}

bool Vector2::operator==(const Vector2& other)
{
    return xpos == other.xpos && ypos == other.ypos;
}

bool Vector2::operator!=(const Vector2& other)
{
    return xpos != other.xpos || ypos != other.ypos;
}