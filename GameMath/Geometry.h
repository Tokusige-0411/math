#pragma once

struct Vector2
{
	int x;
	int y;
	Vector2() : x(0), y(0) {};
	Vector2(int inx, int iny) : x(inx), y(iny) {};
};

Vector2 operator+(const Vector2& lval, const Vector2& rval);
Vector2 operator-(const Vector2& lval, const Vector2& rval);

struct Rect
{
	int x, y, w, h;		// ç∂è„x, ç∂è„y, âEâ∫x, âEâ∫y
	int Left()const{
		return x;
	}
	int Top()const {
		return y;
	}
	int Right()const {
		return x + w;
	}
	int Bottom()const {
		return y + h;
	}
};

struct Circle
{
	Vector2 center;
	float rad;
};

