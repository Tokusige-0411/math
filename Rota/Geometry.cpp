#include <DxLib.h>
#include <math.h>
#include "Geometry.h"

Vector2f operator+(const Vector2f& lval, const Vector2f& rval)
{
	return Vector2f(lval.x + rval.x, lval.y + rval.y);
}

Vector2f operator-(const Vector2f& lval, const Vector2f& rval)
{
	return Vector2f(lval.x - rval.x, lval.y - rval.y);
}

Vector2f operator*(const Vector2f& lval, const int scale)
{
	return Vector2f(lval.x * scale, lval.y * scale);
}

float Vector2f::Magnitude() const
{
	return (float)hypot(x, y);
}

void Vector2f::Normalize()
{
	auto mag = Magnitude();
	x /= mag;
	y /= mag;
}

Vector2f Vector2f::Normalized() const
{
	auto mag = Magnitude();
	return Vector2f(x / mag, y / mag);
}

Vector2 operator+(const Vector2& lval, const Vector2& rval)
{
	return Vector2(lval.x + rval.x, lval.y + rval.y);
}

Vector2 operator-(const Vector2& lval, const Vector2& rval)
{
	return Vector2(lval.x - rval.x, lval.y - rval.y);
}
