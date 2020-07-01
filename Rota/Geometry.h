#pragma once

// �@�v�f��float�^
// �Ax, y�̓�̗v�f������
// �B+-�̃I�[�o�[���[�h
// �x�N�g���̑傫����Ԃ�
// �x�N�g���𐳋K������֐�
// ���K�������x�N�g����Ԃ��֐�

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
	float x, y, w, h;		// ����x, ����y, �E��x, �E��y
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

