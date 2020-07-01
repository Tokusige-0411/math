#pragma once

// ①要素はfloat型
// ②x, yの二つの要素を持つ
// ③+-のオーバーロード
// ベクトルの大きさを返す
// ベクトルを正規化する関数
// 正規化したベクトルを返す関数

struct Vector2f
{
	float x;
	float y;
	Vector2f() : x(0), y(0) {};
	Vector2f(float flx, float fly) : x(flx), y(fly) {};
	float Magnitude() const;
	void Normalize();
	Vector2f Normalized()const;
};

Vector2f operator+(const Vector2f& lval, const Vector2f& rval);
Vector2f operator-(const Vector2f& lval, const Vector2f& rval);
Vector2f operator*(const Vector2f& lval, const int scale);

struct Vector2
{
	float x;
	float y;
	Vector2() : x(0), y(0) {};
	Vector2(float inx, float iny) : x(inx), y(iny) {};
};

Vector2 operator+(const Vector2& lval, const Vector2& rval);
Vector2 operator-(const Vector2& lval, const Vector2& rval);

struct Rect
{
	float x, y, w, h;		// 左上x, 左上y, 右下x, 右下y
	float Left()const{
		return x;
	}
	float Top()const {
		return y;
	}
	float Right()const {
		return x + w;
	}
	float Bottom()const {
		return y + h;
	}
};

struct Circle
{
	Vector2 center;
	float rad;
};

