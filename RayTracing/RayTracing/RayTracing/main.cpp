#include<DxLib.h>
#include<cmath>
#include<iostream>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;
const float albedo[3] = { 1.0f, 0.7f, 0.7f };

// ���˃x�N�g����Ԃ�
Vector3 ReflectVector(const Vector3& invec, const Vector3& normal)
{
	// ���˃x�N�g���̎�
	// R = I - 2*(N�EI)N
	return (invec - normal * (2 * Dot(normal, invec)));
}


//�q���g�ɂȂ�Ǝv���āA�F�X�Ɗ֐���p�ӂ��Ă���܂���
//�ʂɂ��̊֐����g��Ȃ���΂����Ȃ��킯�ł��A����ɉ���Ȃ���΂����Ȃ��킯�ł�
//����܂���B���C�g���[�V���O���ł��Ă���΍\���܂���B

using Color = Vector3;

// �ő�l�ŏ��l�����߂�
float Clamp(float in, const float maxValue = 1.0f, const float minValue = 0.0f)
{
	return max(min(maxValue, in), minValue);
}

Color Clamp(const Color& value, const float maxValue = 255.0f, const float minValue = 0.0f)
{
	return Color(Clamp(value.x, maxValue, minValue), Clamp(value.y, maxValue, minValue), Clamp(value.z, maxValue, minValue));
}

UINT32 GetUint32ColorFromVectorColor(const Color& col)
{
	return GetColor(col.x, col.y, col.z);
}

Color GetCheckerColorFromPosition(const Position3& pos)
{
	Color col1(255, 255, 255);
	Color col2(255, 0, 0);
	auto checker = ((int)(pos.x / 40) + (int)(pos.z / 40)) % 2 == 0 ? 1 : 0;
	checker += pos.x < 0 ? 1 : 0;
	checker += pos.z < 0 ? 1 : 0;
	if (checker % 2 == 0)
	{
		return col1;
	}
	else
	{
		return col2;
	}
}

///���C(����)�Ƌ��̂̓����蔻��
///@param ray (���_����X�N���[���s�N�Z���ւ̃x�N�g��)
///@param sphere ��
///@hint ���C�͐��K�����Ƃ����ق����g���₷�����낤
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Vector3& light,const Sphere& sp, float& t) {
	//���C�����K���ς݂ł���O��Łc
	//
	//���_���狅�̒��S�ւ̃x�N�g��(����)�����܂�
	Vector3 centerVec = sp.pos - eye;
	//
	//���S���王���ւ̓��ς��Ƃ�܂������x�N�g����
	auto dot = Dot(centerVec, ray);
	//�����x�N�g���ƃx�N�g�����������āA���S����̐����������_�����߂܂�
	//����tmp��R(C�ER)�ƂȂ�
	auto V = centerVec - ray * dot;
	if (V.Magnitude() <= sp.radius)
	{
		// �\�ʂ̌�_t�����߂�
		auto w = sqrt((sp.radius * sp.radius) - (V.Magnitude() * V.Magnitude()));
		t = dot - w;
		return true;
	}
	else
	{
		return false;
	}
}

///���C�g���[�V���O
///@param eye ���_���W
///@param sphere ���I�u�W�F�N�g(���̂��������ɂ���)
void RayTracing(const Position3& eye,const Sphere& sphere, const Position3& light) {

	Position3 screenPos;
	Vector3 ray;

	// �������狅�ւ̃x�N�g��
	Vector3 lightVec = sphere.pos - light;
	lightVec.Normalize();

	// ���ʂ̖@���x�N�g��
	Plane plane(Vector3(0, 1, 0), 100);
	auto planeN = Vector3(0.0f, 1.0f, 0.0f).Normalized();

	for (int y = 0; y < screen_height; ++y) {//�X�N���[���c����
		for (int x = 0; x < screen_width; ++x) {//�X�N���[��������

			//�@���_�ƃX�N���[�����W���王���x�N�g�������
			screenPos = { static_cast<float>(x - (screen_width / 2)), static_cast<float>((screen_height / 2) - y), 0.0f };
			ray = screenPos - eye;
			//�A���K�����Ƃ�
			ray.Normalize();
			float sphereT = 0;
			//�BIsHitRay�֐���True�������甒���h��Ԃ�
			if (IsHitRayAndObject(eye, ray, lightVec, sphere, sphereT))
			{
				// t�����_P�����߂�
				auto sphereP = eye + (ray * sphereT);
				// ���̂̒��S�����_P�ւ̖@���x�N�g�������߂�
				auto sphereN = (sphereP - sphere.pos).Normalized();

				// �@���x�N�g���ƌ����x�N�g���̓��ς����ߖ��邳�Ƃ��Ĉ���
				auto diffuseB = Dot(-lightVec, sphereN);
				diffuseB = Clamp(diffuseB);


				// ���C�g���˃x�N�g��
				auto rLight = ReflectVector(lightVec, sphereN);
				// ���C�g���˃x�N�g���Ǝ����t�x�N�g���̓��ς����pow�֐���n�悷��
				auto specular = pow(Clamp(Dot(rLight, -ray)), 20);

				Color difColor1(255, 128, 128);
				Color specColor(255, 255, 255);

				difColor1 *= diffuseB;
				specColor *= specular;
				auto sphereColor = Clamp(difColor1 + specColor);

				// �����Ƌ��̖@���x�N�g�����甽�˃x�N�g�������߂�
				// �����̔��˃x�N�g��
				auto rRay = ReflectVector(ray, sphereN);
				auto rayD = Dot(rRay, planeN);

				if (rayD < 0.0f)
				{
					// ���ɓ����������W�����߂�
					auto w = Dot(sphereP, plane.N);
					auto u = Dot(-rRay, plane.N);
					auto T = (w + plane.d) / u;
					auto P = sphereP + rRay * T;
					auto planeColor = GetCheckerColorFromPosition(P);
					auto refColor = sphereColor * (1.0f- sphere.reflectivity) + planeColor * sphere.reflectivity;
					DrawPixel(x, y, GetUint32ColorFromVectorColor(refColor));
				}
				else
				{
					DrawPixel(x, y, GetUint32ColorFromVectorColor(sphereColor));
				}
			}
			else
			{
				// ���ɕ��@���x�N�g����(0, 1, 0)�Ƃ���
				// ���ʂɓ���������͎����Ɩ@���x�N�g����90�x�ȏ�
				//���h��Ԃ���DrawPixel�Ƃ����֐����g���B
				auto d = Dot(planeN, ray);
				if (d < 0.0f)
				{
					//�n�ʂɓ������Ă���
					//���ʂ̌�_P=eye+ray*t
					//t=w/u(w�͕��ʂ���̋���)
					//���ʂɋ߂Â��傫��
					//w = eye�EN, u = -ray�EN
					//t=w/u�Ƃ��������A���ʂɂ�offset(d)������
					//t = (w+d)/u
					//���Ƃ͂��������_P�����߂�
					//����t�͂S�����炢
					auto w = Dot(eye, plane.N);
					auto u = Dot(-ray, plane.N);
					auto T = (w + plane.d) / u;
					auto P = eye + ray * T;

					DrawPixel(x, y, GetUint32ColorFromVectorColor(GetCheckerColorFromPosition(P)));
				}
			}
		}
	}
}

int WINAPI WinMain(HINSTANCE , HINSTANCE,LPSTR,int ) {
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("�ȈՔł̃��C�g���ł���"));
	DxLib_Init();

	auto eye = Vector3(0, 0, 300);
	auto sphere = Sphere(100, Position3(0, 0, -100), 0.3);
	auto light = Vector3(-100, 200, 200);
	char keyState[256];
	while (ProcessMessage() != -1)
	{
		GetHitKeyStateAll(keyState);
		if (keyState[KEY_INPUT_UP])
		{
			sphere.pos.y += 5.0f;
		}
		if (keyState[KEY_INPUT_DOWN])
		{
			sphere.pos.y -= 5.0f;
		}
		if (keyState[KEY_INPUT_RIGHT])
		{
			sphere.pos.x += 5.0f;
		}
		if (keyState[KEY_INPUT_LEFT])
		{
			sphere.pos.x -= 5.0f;
		}
		if (keyState[KEY_INPUT_X])
		{
			sphere.pos.z += 5.0f;
		}
		if (keyState[KEY_INPUT_Z])
		{
			sphere.pos.z -= 5.0f;
		}
		ClsDrawScreen();
		RayTracing(eye, sphere, light);
		ScreenFlip();
	}

	WaitKey();
	DxLib_End();
}