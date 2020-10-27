#include<DxLib.h>
#include<cmath>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;

//�q���g�ɂȂ�Ǝv���āA�F�X�Ɗ֐���p�ӂ��Ă���܂���
//�ʂɂ��̊֐����g��Ȃ���΂����Ȃ��킯�ł��A����ɉ���Ȃ���΂����Ȃ��킯�ł�
//����܂���B���C�g���[�V���O���ł��Ă���΍\���܂���B

///���C(����)�Ƌ��̂̓����蔻��
///@param ray (���_����X�N���[���s�N�Z���ւ̃x�N�g��)
///@param sphere ��
///@hint ���C�͐��K�����Ƃ����ق����g���₷�����낤
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Sphere& sp ,float& t) {
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
		// ��_t�ւ̖@���x�N�g�������߂�
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
	Vector3 lightVec = light - sphere.pos;
	lightVec.Normalize();
	for (int y = 0; y < screen_height; ++y) {//�X�N���[���c����
		for (int x = 0; x < screen_width; ++x) {//�X�N���[��������
			//�@���_�ƃX�N���[�����W���王���x�N�g�������
			screenPos = { static_cast<float>(x - (screen_width / 2)), static_cast<float>((screen_height / 2) - y), 0.0f };
			ray = screenPos - eye;
			//�A���K�����Ƃ�
			ray.Normalize();
			float t = 0;
			//�BIsHitRay�֐���True�������甒���h��Ԃ�
			if (IsHitRayAndObject(eye, ray, sphere, t))
			{
				auto d = (400.0f - t) / 100.0f;
				DrawPixel(x, y, GetColor(255 * d, 255 * d, 255 * d));
			}
			//���h��Ԃ���DrawPixel�Ƃ����֐����g���B
		}
	}
}

int WINAPI WinMain(HINSTANCE , HINSTANCE,LPSTR,int ) {
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("�ȈՔł̃��C�g���ł���"));
	DxLib_Init();

	auto eye = Vector3(0, 0, 300);
	auto sphere = Sphere(100, Position3(0, 0, -100));
	auto light = Vector3(100, 100, 200);
	RayTracing(eye, sphere, light);

	WaitKey();
	DxLib_End();
}